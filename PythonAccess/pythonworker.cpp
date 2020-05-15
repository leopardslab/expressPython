#include "PythonAccess/emb.h"
#include "pythonworker.h"

PythonWorker::PythonWorker(QObject *parent) : QObject(parent) {
    this->killed.store(EP_PROCESS_INTERRUPT_UNSET);
}

void PythonWorker::RunPython(const QString &startme, const QString &code) {
    emit StartPythonRun();
    PyImport_AppendInittab("emb", emb::PyInitEmbConnect);
    PyImport_AppendInittab("express_api", emb::PyInitApiConnection);
    Py_Initialize();
    PyImport_ImportModule("emb");

    emb::StdOutWriteType write = [this](std::string s) {
        emit this->WriteOutput(QString::fromStdString(s));
        QThread::msleep(10);
    };

    emb::IsInterruptedType isInterrupted = [this]() {
        return this->killed.load();
    };

    // Set the output writer lambda
    emb::SetStdout(write);
    // Set is interrupted checker
    emb::SetIsInterruptedCallback(isInterrupted);

    this->killed.store(EP_PROCESS_INTERRUPT_RUNNING);
    PyGILState_STATE gil = PyGILState_Ensure();
    PyRun_SimpleString(startme.toStdString().c_str());
    PyGILState_Release(gil);
    emb::ResetStdOut();
    Py_Finalize();
    emit EndPythonRun();
}


// https://stackoverflow.com/questions/1420957/stopping-embedded-python
int quit(void *) {
    PyErr_SetInterrupt();
    return -1;
}

void PythonWorker::SetInterrupt() {
    this->killed.store(EP_PROCESS_INTERRUPT_KILL);
}

void PythonWorker::StopPython() {
    QThread::msleep(10);
    PyGILState_STATE gil;
    gil = PyGILState_Ensure();
    Py_AddPendingCall(&quit, NULL);
    PyGILState_Release(gil);
    QThread::msleep(10);
}

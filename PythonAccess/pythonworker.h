#ifndef PYTHONWORKER_H
#define PYTHONWORKER_H

#include <cmath>
#include <Python.h>
#include <QObject>
#include <QThread>
#include <QAtomicInteger>

const int EP_PROCESS_INTERRUPT_RUNNING = 0;
const int EP_PROCESS_INTERRUPT_KILL = 1;
const int EP_PROCESS_INTERRUPT_UNSET = -2;

class PythonWorker : public QObject {
    Q_OBJECT
  public:
    explicit PythonWorker(QObject *parent = nullptr);
    void SetInterrupt();

  private:
    QAtomicInteger<int> killed;

  signals:
    void WriteOutput(QString result);
    void SetInput(QString txt);
    void SetOutput(QString txt);
    void SetCode(QString txt);
    void SetSearchRegex(QString txt);
    void StartPythonRun();
    void EndPythonRun();

  public slots:
    void RunPython(const QString &startme, const QString &code);
    void StopPython();
};

#endif // PYTHONWORKER_H

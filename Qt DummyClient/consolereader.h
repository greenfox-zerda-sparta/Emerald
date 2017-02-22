#ifndef CONSOLEREADER_H
#define CONSOLEREADER_H
#include <QObject>
#include <QDebug>
#include <iostream>


class ConsoleReader : public QObject {
    Q_OBJECT

  public:
    ConsoleReader(QObject* parent = 0);

  signals:
    void run();
    void inputFromCommandLine(QString);
    void finished();
    void error(QString err);
    void toAddCommand(QString);
    void toRemoveCommand(QString);

  private:
    void Quit();
    int isCommandMode;

  private slots:
    void readCommand();
    void onRun();
    void writeToConsole(QString message);
    void setCommandMode(int mode);
};

#endif // CONSOLEREADER_H

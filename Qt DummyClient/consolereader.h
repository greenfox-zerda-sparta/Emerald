#ifndef CONSOLEREADER_H
#define CONSOLEREADER_H
#include <QObject>
#include <QDebug>
#include <iostream>


class ConsoleReader : public QObject
{
    Q_OBJECT

public:
    ConsoleReader(QObject *parent = 0);

signals:
    void run();
    void inputFromCommandLine(QString);
    void finished();
    void error(QString err);

private:
    void Quit();

private slots:
    void readCommand();
    void onRun();
    void writeToConsole(QString message);

};

#endif // CONSOLEREADER_H

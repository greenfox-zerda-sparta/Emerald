#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "clientsocket.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

signals:
    void sendMessage(QString);

private slots:
    void on_ConnectButton_toggled(bool checked);

    void on_SendButton_clicked();

    void onIncomingMessage(QString message);

private:
    Ui::MainWindow *ui;
    ClientSocket *Client;
    quint16 qstringToQuint16(QString string);
    void setIncomingTextArea(QString message);
};

#endif // MAINWINDOW_H

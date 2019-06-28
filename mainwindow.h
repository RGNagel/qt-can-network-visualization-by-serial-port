#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QSerialPort>
#include <QSerialPortInfo>
#include <QMap>

#include "ecu.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    bool getTelemetryUSB(QSerialPortInfo *info);

private slots:

    //void bytesWritten(qint64 bytes);
    void handleReadyRead();

    void on_Telemetry_ECU_clicked();

    void on_terminalButton_clicked();

    void on_Battery_ECU_clicked();

    void on_Engine_ECU_clicked();

    void on_Diff_ECU_clicked();

    void on_Acc_ECU_clicked();

private:
    Ui::MainWindow *ui;
    quint16 const TELEMETRY_USB_ID_PRODUCT = 0xea60;
    QSerialPort *serial;
    //ECU ecu;
    QMap<int, ECU *> ecus;
};

#endif // MAINWINDOW_H

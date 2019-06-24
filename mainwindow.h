#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QSerialPort>
#include <QSerialPortInfo>

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

private:
    Ui::MainWindow *ui;
    quint16 const TELEMETRY_USB_ID_PRODUCT = 0xea60;
    QSerialPort *serial;
    ECU ecu;
};

class CanBus {
public:
    enum ID {
        TELEMETRY_ECU = 10,

        ENGINE_ECU = 20,
        ENGINE_ECU_TEMP = 19,
        ENGINE_ECU_ROT = 18,

        BATTERY_ECU = 30,
        BATTERY_ECU_VOLT = 29,
        BATTERY_ECU_CURR = 28,
        BATTERY_ECU_TEMP = 27,

        DIFF_ECU = 40,
        DIFF_ECU_ANG = 39,

        ACC_ECU = 50,
        ACC_ECU_G = 49,

    };
private:

    class ECU {

    };
};


#endif // MAINWINDOW_H

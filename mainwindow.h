#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QSerialPort>
#include <QSerialPortInfo>

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

private:
    Ui::MainWindow *ui;
    quint16 const TELEMETRY_USB_ID_PRODUCT = 0xea60;
    QSerialPort *serial;
};

class CanBus {
public:
    enum ID {
        ENGINE_ECU = 0,
        BATTERY_ECU = 4,
        TELEMETRY_ECU = 8,
        DIFF_ECU = 16,
        ACC_ECU = 32,
    };
private:
};

#endif // MAINWINDOW_H

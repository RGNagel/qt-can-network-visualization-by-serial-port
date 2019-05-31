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
    QSerialPortInfo * getTelemetryUSB();

private slots:

    void bytesWritten(qint64 bytes);

private:
    Ui::MainWindow *ui;
    quint16 const TELEMETRY_USB_ID_PRODUCT = 0xea60;
};

#endif // MAINWINDOW_H

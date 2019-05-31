#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QSerialPort>
#include <QSerialPortInfo>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // search telemetry usb port
    QSerialPortInfo * port;

    port = getTelemetryUSB();
    if (port) {
        //TODO: ui->port_status->setTextFormat("<html><head/><body><p><span style=\"font-weight:600; color:#8ae234;\">found!</span></p></body></html>");
        ui->port_status->setText("Found!");
        ui->textSerialPort->setEnabled(true);
        QSerialPort * serial = new QSerialPort();
        serial->setPort(*port);
        serial->setBaudRate(QSerialPort::Baud57600);
        serial->setStopBits(QSerialPort::OneStop);
        serial->setParity(QSerialPort::NoParity);
        serial->open(QIODevice::ReadWrite);
    }
    else {
        //TODO: ui->port_status->setTextFormat("<html><head/><body><p><span style=\"font-weight:600;color:#ef2929;\">Not found</span></p></body></html>");
        ui->port_status->setText("Not Found :(");
        ui->textSerialPort->setEnabled(false);
    }

}

MainWindow::~MainWindow()
{
    delete ui;
}

QSerialPortInfo * MainWindow::getTelemetryUSB()
{
    QList<QSerialPortInfo> ports = QSerialPortInfo::availablePorts();

    for (QList<QSerialPortInfo>::iterator i = ports.begin(); i != ports.end(); i++) {
        quint16 id_product = (*i).productIdentifier();
        if (id_product == MainWindow::TELEMETRY_USB_ID_PRODUCT) {

            return &(*i);
        }
    }
    return nullptr;
}

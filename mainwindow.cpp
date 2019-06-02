#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QSerialPort>
#include <QSerialPortInfo>

#include <string>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QSerialPortInfo port;
    this->serial = new QSerialPort();

    if (getTelemetryUSB(&port)) {
        ui->port_status->setText(" Found!");

        this->serial->setPort(port);
        this->serial->setBaudRate(QSerialPort::Baud57600);
        this->serial->setStopBits(QSerialPort::OneStop);
        this->serial->setParity(QSerialPort::NoParity);

        if (this->serial->open(QIODevice::ReadWrite)) {
            ui->textSerialPort->setEnabled(true);
        }
        else {
            enum QSerialPort::SerialPortError err = this->serial->error();
            QString msg;
            msg.sprintf("Opening serial failed! Error: %i.", err);
            if (err == 2)
                msg.append("Check: device permission and/or whether it is being used.");
            ui->port_status->setText(msg);
        }
    }
    else {
        ui->port_status->setText(" Not Found!");
        ui->textSerialPort->setEnabled(false);
    }
    QObject::connect(this->serial, SIGNAL(readyRead()), this, SLOT(handleReadyRead()));

}

MainWindow::~MainWindow()
{
    delete ui;
    if (this->serial->isOpen())
        this->serial->close();
    delete this->serial;
}

bool MainWindow::getTelemetryUSB(QSerialPortInfo *info)
{
    QList<QSerialPortInfo> ports = QSerialPortInfo::availablePorts();

    for (QList<QSerialPortInfo>::iterator i = ports.begin(); i != ports.end(); i++) {
        quint16 id_product = (*i).productIdentifier();
        if (id_product == MainWindow::TELEMETRY_USB_ID_PRODUCT) {
            *info = *i;
            return true;
        }
    }
    return false;
}

void MainWindow::handleReadyRead()
{
    QByteArray ba;
    qint64 bytes = this->serial->bytesAvailable();

    ba.reserve((static_cast<int>(bytes)));

    this->serial->read(ba.data(), 32);
    this->ui->textSerialPort->append(ba.data());
}

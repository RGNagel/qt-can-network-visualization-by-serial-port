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
            ui->Telemetry_ECU->setEnabled(true);
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

    // TODO: remove
    ui->Telemetry_ECU->setEnabled(true);

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

    // interprete message
    char data[8]; // max. can bus data is 8 bytes per packet
    //QString data(8);
    int std_id;

    sscanf(ba.data(), "%i,%8s", &std_id, data);

    switch (std_id) {
    case CanBus::ENGINE_ECU:
    case CanBus::ENGINE_ECU_TEMP:
    case CanBus::ENGINE_ECU_ROT:
        this->ui->Engine_ECU->setEnabled(true);
        //this->ui->
        break;
    case CanBus::BATTERY_ECU:
    case CanBus::BATTERY_ECU_VOLT:
    case CanBus::BATTERY_ECU_CURR:
    case CanBus::BATTERY_ECU_TEMP:
        this->ui->Battery_ECU->setEnabled(true);
        break;
    case CanBus::DIFF_ECU:
    case CanBus::DIFF_ECU_ANG:
        this->ui->Diff_ECU->setEnabled(true);
        break;
    case CanBus::ACC_ECU:
    case CanBus::ACC_ECU_G:
        this->ui->Acc_ECU->setEnabled(true);
        break;
    default:

        break;
    }

}

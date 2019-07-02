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
            this->ecus[ECU::TELEMETRY_ECU] = new ECU;
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
    // next line: "The program has unexpectedly finished."
    //for (QMap<int, ECU *>::iterator i = this->ecus.begin(); i != this->ecus.end(); ++i)
    //    delete &i;
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

/* handle data from serial port */
void MainWindow::handleReadyRead()
{
    QByteArray ba;
    qint64 bytes = this->serial->bytesAvailable();

    ba.reserve((static_cast<int>(bytes)));

    this->serial->read(ba.data(), 32);

    if (this->ui->textSerialPort->isEnabled())
        this->ui->textSerialPort->append(ba.data());

    // interprete message

    uint16_t data;
    uint16_t std_id;

    sscanf(ba.data(), "id=%hu,data=%hu", &std_id, &data);

    /*
     * we create ECU class for each ECU unity. Within each ECU Unity we can have many variables (ECU::Variable)
     *
     * std id can match either a ECU unity or a ECU variable
     * ECU unities has the following pattern:
     * x0 e.g. 10, 20, 30, 40, 50... (divisible by 10)
    */

    uint16_t ecu_unity_id = std_id;

    if ((ecu_unity_id % 10) != 0)
        ecu_unity_id = (((std_id + 10)/10))*10; // get the ecu unity pattern from its variable

    // first we check if ecu unity class was NOT previously created
    if (this->ecus.find(ecu_unity_id) == ecus.end())
        this->ecus[ecu_unity_id] = new ECU;

    // if this std_id is a variable from a ecu unity
    if (std_id != ecu_unity_id) {

        ECU * ecu_unity = this->ecus[ecu_unity_id];

        // check if this variable was NOT previously created
        if (ecu_unity->vars.find(std_id) == ecu_unity->vars.end()) {
            ecu_unity->vars[std_id] = new ECU::Variable;
            ecu_unity->newPlot(std_id);
        }

        ecu_unity->vars[std_id]->addData(data);

    }

    switch (ecu_unity_id) {
    case ECU::ENGINE_ECU:

        this->ui->Engine_ECU->setEnabled(true);
        break;

    case ECU::BATTERY_ECU:

        this->ui->Battery_ECU->setEnabled(true);
        break;

    case ECU::DIFF_ECU:

        this->ui->Diff_ECU->setEnabled(true);
        break;

    case ECU::ACC_ECU:

        this->ui->Acc_ECU->setEnabled(true);
        break;

    default:

        break;
    }

}

void MainWindow::on_terminalButton_clicked()
{
    if (this->ui->textSerialPort->isEnabled()) {
        this->ui->textSerialPort->setEnabled(false);
        this->ui->terminalButton->setText("Start Terminal");
    }
    else {
        this->ui->textSerialPort->setEnabled(true);
        this->ui->terminalButton->setText("Stop Terminal");
    }

}

void MainWindow::on_Telemetry_ECU_clicked()
{
    this->ecus[ECU::TELEMETRY_ECU]->show();
}

void MainWindow::on_Battery_ECU_clicked()
{
    this->ecus[ECU::BATTERY_ECU]->show();
}

void MainWindow::on_Engine_ECU_clicked()
{
    this->ecus[ECU::ENGINE_ECU]->show();
}

void MainWindow::on_Diff_ECU_clicked()
{
    this->ecus[ECU::DIFF_ECU]->show();
}

void MainWindow::on_Acc_ECU_clicked()
{
    this->ecus[ECU::ACC_ECU]->show();
}

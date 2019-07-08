#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QSerialPort>
#include <QSerialPortInfo>

#include <string>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QSerialPortInfo port;
    this->serial = new QSerialPort();
    this->stats = new Stats();

    if (getTelemetryUSB(&port)) {
        ui->port_status->setText(" Found!");

        this->serial->setPort(port);
        this->serial->setBaudRate(QSerialPort::Baud57600);
        this->serial->setStopBits(QSerialPort::OneStop);
        this->serial->setParity(QSerialPort::NoParity);

        if (this->serial->open(QIODevice::ReadWrite)) {
            ui->textSerialPort->setEnabled(true);
            ui->Telemetry_ECU->setEnabled(true);
            //this->ecus[ECU::TELEMETRY_ECU] = new ECU;
            //this->ecu_telemetry = new
        }
        else {
            enum QSerialPort::SerialPortError err = this->serial->error();
            QString msg;
            msg.sprintf("Opening serial failed! Error: %i.", err);
            if (err == 2)
                msg.append("Check: device permission and/or its usage.");
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

    if (this->writeToTerminal)
        this->ui->textSerialPort->append(ba.data());

    // interprete message

    // if not 'unsigned int' var then sscanf crashes
    unsigned int data[4];
    unsigned int std_id;

    // this is the pattern of data sent by ecu telemetry
    int filled = sscanf(ba.data(), "%4x,%4x,%4x,%4x,%4x\r\n", &std_id, &data[0], &data[1], &data[2], &data[3]);

    if (filled == 5) {
        // handle valid data

        /*
         * we create ECU class for each ECU unity. Within each ECU Unity we can have many variables (ECU::Variable)
         *
         * std id can match either a ECU unity or a ECU variable
         * ECU unities has the following pattern:
         * x0 e.g. 10, 20, 30, 40, 50... (divisible by 10)
        */

        uint16_t ecu_unity_id = ECU::get_ecu_unity_id(std_id);

        // first we check if ecu unity class was NOT previously created
        if (this->ecus.find(ecu_unity_id) == ecus.end()) {
            QPushButton *bt = this->getQPushButton(ecu_unity_id);
            if (bt)
                this->ecus[ecu_unity_id] = new ECU(nullptr, bt);
        }
        ECU * ecu_unity = this->ecus[ecu_unity_id];

        // if this std_id is a variable from a ecu unity. it should always be true because ecu unities don't send data
        if (std_id != ecu_unity_id) {

            // check if this variable was NOT previously created
            if (ecu_unity->vars.find(std_id) == ecu_unity->vars.end()) {
                QLabel *label = this->getQLabel(std_id);
                if (label) {
                    ecu_unity->vars[std_id] = new ECU::Variable(label);
                    ecu_unity->newPlot(std_id);
                }
            }

            // add data
            ecu_unity->vars[std_id]->addData(data[0]);
        }

        ecu_unity->fillStats();

        this->stats->valid_data_inc();
        this->stats->setEcusOnline(this->ecus.size());


    }
    else {
        // data not recognized
        this->stats->invalid_data_inc();
    }

}

QLabel * MainWindow::getQLabel(int std_id)
{
    switch(std_id) {
        case ECU::ACC_ECU_G_X:
            return this->ui->acc_g_force_x;
        case ECU::ACC_ECU_G_Y:
            return this->ui->acc_g_force_y;
        case ECU::ACC_ECU_G_Z:
            return this->ui->acc_g_force_z;
        case ECU::DIFF_ECU_ANG:
            return this->ui->diff_angle;
        case ECU::BATTERY_ECU_TEMP:
            return this->ui->bat_temp;
        case ECU::BATTERY_ECU_CURR:
            return this->ui->bat_curr;
        case ECU::BATTERY_ECU_VOLT:
            return this->ui->bat_volt;
        case ECU::ENGINE_ECU_ROT_L:
            return this->ui->eng_rot_l;
        case ECU::ENGINE_ECU_ROT_R:
            return this->ui->eng_rot_r;
        case ECU::ENGINE_ECU_TEMP_L:
            return this->ui->eng_temp_l;
        case ECU::ENGINE_ECU_TEMP_R:
            return this->ui->eng_temp_r;
    }

    return nullptr;
}
QPushButton * MainWindow::getQPushButton(int std_id)
{
    switch(std_id) {
        case ECU::ACC_ECU:
            return this->ui->Acc_ECU;
        case ECU::ENGINE_ECU:
            return this->ui->Engine_ECU;
        case ECU::BATTERY_ECU:
            return this->ui->Battery_ECU;
        case ECU::DIFF_ECU:
            return this->ui->Diff_ECU;
    }

    return nullptr;
}

void MainWindow::on_terminalButton_clicked()
{
    if (this->writeToTerminal) {
        // disable
        this->ui->terminalButton->setText("Start Terminal");
        this->writeToTerminal = false;
    }
    else {
        // enable
        this->ui->terminalButton->setText("Stop Terminal");
        this->writeToTerminal = true;
    }
}

void MainWindow::on_Telemetry_ECU_clicked()
{
    this->stats->show();
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

void MainWindow::on_label_22_linkActivated(const QString &link)
{

}

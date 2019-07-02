#include "ecu.h"
#include "ui_ecu.h"

ECU::ECU(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::ECU)
{
    ui->setupUi(this);
}

ECU::~ECU()
{
    delete ui;
}

void ECU::newPlot(uint16_t variable_id)
{
    QCustomPlot *plot = new QCustomPlot;
    this->vars[variable_id]->setPlot(plot);
    this->ui->plots->addWidget(plot);
}

void ECU::Variable::setPlot(QCustomPlot *plot)
{
    static uint8_t block_ctr = 0;
    if (!block_ctr)
        this->plot = plot;
    block_ctr++;
}

void ECU::Variable::addData(uint16_t data)
{
    this->data.push_back(data);
    this->ts = QDateTime::currentDateTime();
}

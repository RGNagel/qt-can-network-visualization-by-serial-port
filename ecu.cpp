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
    plot->addGraph();

    this->vars[variable_id]->setPlot(plot);
    this->ui->plots->addWidget(plot);
}

void ECU::Variable::addData(uint16_t data)
{
    this->data.push_back(data);
    this->ts = QDateTime::currentDateTime();

    this->plot->graph(0)->addData(this->x_axis++, data);
    this->plot->graph(0)->rescaleAxes();
}

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

    plot->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    // x axis as time
    QSharedPointer<QCPAxisTickerTime> timeTicker(new QCPAxisTickerTime);
    timeTicker->setTimeFormat("%h:%m:%s");
    plot->xAxis->setTicker(timeTicker);

    plot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);

    this->vars[variable_id]->setPlot(plot);
    this->ui->plots->addWidget(plot);
}

void ECU::Variable::addData(uint16_t data)
{
    //this->ts = QDateTime::currentDateTime();

    static QTime time(QTime::currentTime()); // only once for all objects calling this method

    double x = (time.elapsed() + time.msecsSinceStartOfDay()) / 1000;

    this->plot->graph(0)->addData(x, data);

    // make key axis range scroll with the data (at a constant range size of 20):

    this->plot->xAxis->setRange(x, 20, Qt::AlignRight);
    this->plot->graph(0)->rescaleKeyAxis();
    this->plot->replot();
}

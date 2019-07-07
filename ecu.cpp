#include "ecu.h"
#include "ui_ecu.h"

ECU::ECU(QWidget *parent, QPushButton *button) :
    QFrame(parent),
    ui(new Ui::ECU)
{
    ui->setupUi(this);
    if (button == nullptr)
        qDebug() << "button is nullpointer!";
    else
        button->setEnabled(true);

    this->button = button;
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
    // stats
    this->last_ts = QTime::currentTime();
    this->qty_data++;
    this->online = true;
    this->label->setEnabled(true);

    // plot
    static QTime time(QTime::currentTime()); // only once for all objects calling this method
    double x = (time.elapsed() + time.msecsSinceStartOfDay()) / 1000;

    this->plot->graph(0)->addData(x, data);
    // make key axis range scroll with the data (at a constant range size of 20):
    this->plot->xAxis->setRange(x, 20, Qt::AlignRight);
    this->plot->graph(0)->rescaleKeyAxis();
    this->plot->replot();
}

void ECU::fillStats()
{
    int data_qty = 0;
    QTime tmp_time = QTime(0,0,0);

    for (QMap<unsigned int, ECU::Variable *>::iterator i = this->vars.begin(); i != this->vars.end(); ++i) {
        ECU::Variable * var = *i;

        data_qty += var->getDataQty();

        if (var->getLastTs().msecsSinceStartOfDay() > tmp_time.msecsSinceStartOfDay())
            tmp_time = var->getLastTs();
    }

    this->ui->qty_data->setNum(data_qty);
    this->ui->qty_var->setNum(this->vars.size());
    this->ui->last_data_ts->setText(tmp_time.toString(Qt::TextDate));
}

#include "stats.h"
#include "ui_stats.h"

Stats::Stats(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::Stats)
{
    ui->setupUi(this);
}

Stats::~Stats()
{
    delete ui;
}

void Stats::valid_data_inc() {
    this->valid_data++;
    this->ui->valid_data->setNum(this->valid_data);

    this->ui->raw_data->setNum(this->getTotalData());

    this->setLastValidData(QTime::currentTime());
}

void Stats::invalid_data_inc() {
    this->invalid_data++;
    this->ui->invalid_data->setNum(this->invalid_data);

    this->ui->raw_data->setNum(this->getTotalData());

    this->setLastInvalidData(QTime::currentTime());
}

void Stats::setEcusOnline(int n) {
    this->ecus_online = n;
    this->ui->ecus_online->setNum(n);
}

void Stats::setLastValidData(QTime time) {
    this->last_valid_data = time;
    QString format = time.toString(Qt::TextDate); // HH:mm:ss
    this->ui->valid_data_ts->setText(format);
}

void Stats::setLastInvalidData(QTime time) {
    this->last_invalid_data = time;
    QString format = time.toString(Qt::TextDate); // HH:mm:ss
    this->ui->invalid_data_ts->setText(format);
}

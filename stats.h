#ifndef STATS_H
#define STATS_H

#include <QFrame>
#include <QTime>

namespace Ui {
class Stats;
}

class Stats : public QFrame
{
    Q_OBJECT

public:
    explicit Stats(QWidget *parent = nullptr);
    ~Stats();
    void valid_data_inc();
    void invalid_data_inc();
    void setEcusOnline(int n);
    inline int getTotalData() { return this->valid_data + this->invalid_data; }

private:
    Ui::Stats *ui;
    int valid_data;
    int invalid_data;
    QTime last_valid_data;
    QTime last_invalid_data;
    int ecus_online;

    void setLastInvalidData(QTime time);
    void setLastValidData(QTime time);
};

#endif // STATS_H

#ifndef ECU_H
#define ECU_H

#include <QFrame>
#include <QTime>
#include <QMap>
#include "qcustomplot.h"
#include <QDebug>

namespace Ui {
class ECU;
}

class ECU : public QFrame
{
    Q_OBJECT

public:
    explicit ECU(QWidget *parent = nullptr, QPushButton *button = nullptr);
    ~ECU();
    enum ID {
        TELEMETRY_ECU = 10,
        ENGINE_ECU	= 20,
        ENGINE_ECU_TEMP_L = 13,
        ENGINE_ECU_TEMP_R = 15,
        ENGINE_ECU_ROT_R = 17,
        ENGINE_ECU_ROT_L = 19,

        BATTERY_ECU	= 30,
        BATTERY_ECU_VOLT = 29,
        BATTERY_ECU_CURR = 28,
        BATTERY_ECU_TEMP = 27,

        DIFF_ECU = 40,
        DIFF_ECU_ANG = 39,

        ACC_ECU = 50,
        ACC_ECU_G_X = 49,
        ACC_ECU_G_Y = 48,
        ACC_ECU_G_Z	= 47,
    };

    class Variable {
    public:
        Variable(QLabel *label)
        {
            if (label == nullptr)
                qDebug() << "label is nullpointer!";
            this->label = label;
        }
        void addData(uint16_t data);
        inline void setPlot(QCustomPlot *plot) { this->plot = plot; }
        inline int getDataQty() { return this->qty_data; }
        inline QTime getLastTs() { return this->last_ts; }

    private:
        QTime last_ts;
        QCustomPlot *plot;
        int qty_data = 0;
        bool online = false;
        QLabel *label;
    };

    QMap<unsigned int, Variable *> vars;
    uint64_t packets = 0;

    void newPlot(uint16_t variable_id);
    inline QPushButton * getButton() { return this->button; }
    // get the ecu unity pattern from its variable
    static inline unsigned int get_ecu_unity_id(unsigned int std_id)
    {
        if ((std_id % 10) == 0)
            return (((std_id + 10)/10))*10;
        return std_id;
    }
    void fillStats();

private:
    Ui::ECU *ui;
    QPushButton *button;
};

#endif // ECU_H

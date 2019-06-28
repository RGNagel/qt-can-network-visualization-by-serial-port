#ifndef ECU_H
#define ECU_H

#include <QFrame>
#include <QTime>
#include <QMap>

namespace Ui {
class ECU;
}

class ECU : public QFrame
{
    Q_OBJECT

public:
    explicit ECU(QWidget *parent = nullptr);
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
        QList<uint16_t> data;
        QTime ts;
    };

    QMap<int, Variable *> vars;

private:
    Ui::ECU *ui;
};

#endif // ECU_H

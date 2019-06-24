#ifndef ECU_H
#define ECU_H

#include <QFrame>

namespace Ui {
class ECU;
}

class ECU : public QFrame
{
    Q_OBJECT

public:
    explicit ECU(QWidget *parent = nullptr);
    ~ECU();

private:
    Ui::ECU *ui;
};

#endif // ECU_H

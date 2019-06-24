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

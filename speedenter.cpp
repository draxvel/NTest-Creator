#include "speedenter.h"
#include "ui_speedenter.h"

SpeedEnter::SpeedEnter(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SpeedEnter)
{
    ui->setupUi(this);

    //Форма з фіксованими розмірами
    this->setFixedSize(this->geometry().width(),this->geometry().height());
}

SpeedEnter::~SpeedEnter()
{
    delete ui;
}

void SpeedEnter::on_pushButtonCreateTest_clicked()
{
    w.SaveAsDB();
}

void SpeedEnter::on_pushButtonEditTest_clicked()
{
    w.OpenDB();
}

void SpeedEnter::on_pushButtonStudents_clicked()
{
    st.show();
}

void SpeedEnter::on_pushButtonAbout_clicked()
{
    a.show();
}

void SpeedEnter::on_pushButtonStudents_3_clicked()
{
    r.show();
}

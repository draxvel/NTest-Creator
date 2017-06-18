#include "result.h"
#include "ui_result.h"
#include <QTextStream>
#include <QFile>
#include <QFileDialog>
#include <QMessageBox>
#include <QByteArray>

Result::Result(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Result)
{
    ui->setupUi(this);
}

Result::~Result()
{
    delete ui;
}

void Result::on_pushButton_clicked()
{
    QString fileName =  QFileDialog::getOpenFileName(
                this,
                tr("Result"),                   //title
                "С:\\",                             //default folder
                "NTest Results (*.ntkR)"            //check only database files
                );

    if(!fileName.isEmpty())
        {
            QFile file(fileName);
            file.open(QIODevice::ReadOnly | QIODevice::Text);
            QTextStream in (&file);
            in.setCodec("UTF-8");
            in>>results;

            results = decodeStr(results);
            ui->textBrowser->setText(results);
        }
        else
        {
            QMessageBox msgBox;
            msgBox.setText("Файл не відкрито!");
            msgBox.exec();
    }
}

QString Result::decodeStr(const QString &str)
{
    QByteArray arr = QByteArray::fromBase64(str.toLatin1());
    for(int i =0; i<arr.size(); i++)
        arr[i] =arr[i] ^ key;

    return QString::fromUtf8(arr);
}

void Result::closeEvent(QCloseEvent *event)
{
    ui->textBrowser->clear();
}

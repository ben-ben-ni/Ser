#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);



    Serial=new QSerialPort(this);

    connect(Serial,SIGNAL(readyRead()),this,SLOT(readOnlyAll()));



}

Widget::~Widget()
{
    delete ui;
}

void Widget::on_pushButton_clicked()   //串口初始化
{
    if(Serial==nullptr) return;
    if(Serial->isOpen()){
        Serial->clear();
        Serial->close();
    }
    Serial->setPortName(ui->port->currentText());  //设置端口号

    if(!Serial->open(QIODevice::ReadWrite)){      //端口是否被占用
        ui->link->setText(Serial->portName()+"打开失败");
        return;
    }

    Serial->setBaudRate(ui->baudRate->currentText().toInt());  //设置波特率

    int data=ui->dataBits->currentText().toInt();    //设置数据位
    switch(data){
    case 8:
        Serial->setDataBits(QSerialPort::Data8);
        break;
    case 7:
        Serial->setDataBits(QSerialPort::Data7);
        break;
    case 6:
        Serial->setDataBits(QSerialPort::Data6);
        break;
    case 5:
        Serial->setDataBits(QSerialPort::Data5);
        break;
    default:
        Serial->setDataBits(QSerialPort::Data8);
        break;
    }

    int stop=ui->StopBits->currentText().toInt();   //设置停止位
    switch(stop){
    case 1:
        Serial->setStopBits(QSerialPort::OneStop);
        break;
    case 2:
        Serial->setStopBits(QSerialPort::TwoStop);
        break;
    }

    QString parity=ui->check->currentText();     //设置校验位
    if("无校验"==parity){
        Serial->setParity(QSerialPort::NoParity);
    }else if("偶校验"==parity){
        Serial->setParity(QSerialPort::EvenParity);
    }else if("奇校验"==parity){
        Serial->setParity(QSerialPort::OddParity);
    }

    Serial->setFlowControl(QSerialPort::NoFlowControl);  //设置流控制

    ui->link->setText(Serial->portName()+"连接成功");
}

void Widget::on_pushButton_2_clicked()
{
    Serial->clear();
    Serial->close();
    ui->link->setText(Serial->portName()+"断开连接");
}

void Widget::on_pushButton_4_clicked()
{
    ui->writeText->clear();
}

void Widget::on_pushButton_3_clicked()
{
    Serial->write(ui->writeText->text().toLocal8Bit().data());
}

void Widget::readOnlyAll()
{
    QByteArray Data = Serial->readAll();
    ui->readText->appendPlainText(QString(Data));
}

void Widget::on_pushButton_5_clicked()
{
    QStringList SerialPortName;
    foreach(const QSerialPortInfo &info,
            QSerialPortInfo::availablePorts()){
        SerialPortName << info.portName();
    }  //遍历可用端口
    ui->port->clear();
    ui->port->addItems(SerialPortName);
}

void Widget::on_pushButton_6_clicked()
{
    ui->readText->clear();
}

#include "widget.h"
#include "ui_widget.h"
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QDebug>
#include <QTimer>
#include <QTime>

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    setWindowTitle("ImageBrowser");

    resize(850,550);
    ui->pushButton_cmclose->hide();

    pix2=QPixmap(640,480);
    pix2.fill(Qt::white);


    ratio=1;
    status="0";
    CurrentIndex=1;
    itoa(CurrentIndex,tempfile,10);

    filename=":/file/";
    filename.append(tempfile).append(".jpg");

    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
    {
        qDebug() << "Name        : " << info.portName();
        qDebug() << "Description : " << info.description();
        qDebug() << "Manufacturer: " << info.manufacturer();

        // Example use QSerialPort
        QSerialPort serial;
        serial.setPort(info);
        if (serial.open(QIODevice::ReadWrite))
        {
            ui->comboBox->addItem(info.portName());
            serial.close();
        }
    }
}

Widget::~Widget()
{
    delete ui;
}

void Widget::on_pushButton_cmopen_clicked()
{
    my_serialport= new QSerialPort();
    qDebug()<<ui->comboBox->currentText();
    my_serialport->setPortName(ui->comboBox->currentText());
    my_serialport->open(QIODevice::ReadWrite);
    qDebug()<<ui->lineEdit->text().toInt();
    my_serialport->setBaudRate(ui->lineEdit->text().toInt());
    my_serialport->setDataBits(QSerialPort::Data8);
    my_serialport->setParity(QSerialPort::NoParity);
    my_serialport->setStopBits(QSerialPort::OneStop);
    my_serialport->setFlowControl(QSerialPort::NoFlowControl);


    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(updateAA()));
    timer->start(1000);

    ui->pushButton_cmopen->hide();
    ui->pushButton_cmclose->show();


}

void Widget::updateAA()
{
   requestData = my_serialport->readAll();

//status="3";

      if(requestData!= NULL)
    {
          status=requestData;
          if(status=="1"){
              filename.clear();
              CurrentIndex=(CurrentIndex+1+6)%6;
              filename=":/file/";
              itoa(CurrentIndex,tempfile,10);
              filename.append(tempfile).append(".jpg");
          }
          if(status=="2"){
              filename.clear();
              CurrentIndex=(CurrentIndex-1+6)%6;
              filename=":/file/";
              itoa(CurrentIndex,tempfile,10);
              filename.append(tempfile).append(".jpg");
          }

          if(status=="3"){
              if(ratio<32){
              ratio=2*ratio;
              }
          }
          if(status=="4"){
              ratio=ratio/2;
          }
        qDebug() <<requestData;


        ui->textEdit->append(requestData);
//       update();
    }


    requestData.clear();


   update();

}

void Widget::on_pushButton_send_clicked()
{
    QByteArray TxData = "a";
    my_serialport->write(TxData);
}

void Widget::on_pushButton_cmclose_clicked()
{
    my_serialport->close();
    ui->pushButton_cmclose->hide();
    ui->pushButton_cmopen->show();
}

void Widget::paintEvent(QPaintEvent *) {

    QPainter painter(this);
    //painter.drawPixmap(200,10,pix1);
    painter.drawPixmap(200,20,pix2);

   QPainter p2(&pix2);
   QPixmap temp;

   if(status=="0"){
   temp.load(filename);
   p2.drawPixmap(0,0,640,480,temp);
}

   if(status=="1"){
       pix2.fill(Qt::white);
       p2.translate(0,0);


       temp.load(filename);
       p2.drawPixmap(0,0,640,480,temp);
       }
   if(status=="2"){
       pix2.fill(Qt::white);
       p2.translate(0,0);
       temp.load(filename);
       p2.drawPixmap(0,0,640,480,temp);
       }
   if(status=="3"){

       if(ratio<32){
       pix2.fill(Qt::white);
       p2.translate(0,0);
       temp.load(filename);
       qreal width=temp.width();
       qreal height=temp.height();

       temp.scaled(width*ratio,height*ratio,Qt::KeepAspectRatioByExpanding);
       p2.drawPixmap(0,0,width*ratio,height*ratio,temp);

}

       }
   if(status=="4"){
       pix2.fill(Qt::white);
       p2.translate(0,0);
       temp.load(filename);
       qreal width=temp.width();
       qreal height=temp.height();
       temp.scaled(width*ratio,height*ratio,Qt::KeepAspectRatioByExpanding);
       p2.drawPixmap(0,0,width*ratio,height*ratio,temp);
   }


}

void Widget::on_pushButton_clear_clicked()
{
    ui->textEdit->clear();
}

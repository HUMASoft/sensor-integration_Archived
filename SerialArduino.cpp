#include "SerialArduino.h"

SerialArduino::SerialArduino()
{
//    QApplication a(argc, argv);
        arduino_is_available = false;
        arduino_port_name = "";
        port = new QSerialPort;


        //Parte # 2,buscar puertos con los identificadores de Arduino
        qDebug() << "Number of available ports: " << QSerialPortInfo::availablePorts().length();
        foreach(const QSerialPortInfo &serialPortInfo, QSerialPortInfo::availablePorts()){
            qDebug() << "Has vendor ID: " << serialPortInfo.hasVendorIdentifier();
            if(serialPortInfo.hasVendorIdentifier() && serialPortInfo.portName()=="ttyACM0"){
                qDebug() << "Port: " << serialPortInfo.portName();
                qDebug() <<"\n";
                qDebug() << "Vendor ID: " << serialPortInfo.vendorIdentifier();
                qDebug() << "Has Product ID: " << serialPortInfo.hasProductIdentifier();
                qDebug() << "Product ID: " << serialPortInfo.productIdentifier();
             }
        }


        foreach(const QSerialPortInfo &serialPortInfo, QSerialPortInfo::availablePorts()){
            if(serialPortInfo.hasVendorIdentifier() && serialPortInfo.hasProductIdentifier()){
               if(serialPortInfo.vendorIdentifier() == arduino_uno_vendor_id){
                 if(serialPortInfo.productIdentifier() == arduino_MEGA){
                   if(serialPortInfo.portName()=="ttyACM0") {
                    arduino_port_name=serialPortInfo.portName();
                    arduino_is_available = true;
                   }
                 }
               }
            }
         }

        if(arduino_is_available && arduino_port_name=="ttyACM0"){
            // open and configure the serialport
            port->setPortName(arduino_port_name);
            port->open(QIODevice::ReadWrite);
            port->setBaudRate(QSerialPort::Baud9600);
            port->setDataBits(QSerialPort::Data8);
            port->setParity(QSerialPort::NoParity);
            port->setStopBits(QSerialPort::OneStop);
            port->setFlowControl(QSerialPort::NoFlowControl);
            //QObject::connect(arduino, SIGNAL(readyRead()), this, SLOT(readSerial()));
    }


}

long SerialArduino::ReadSensor(double &incli, double &orien)
{

    if(port->isReadable()){
        dataread = port->readLine();
        if(dataread!="\n"){
            //qDebug() <<"Dato total:"<<dataread;
            serialBuffer = QString::fromStdString(dataread.data());
            data1=serialBuffer;
            data2=serialBuffer;
            for (x=0; x <= serialBuffer.size(); x++)
            {
                if (serialBuffer[x]==','){
                    data1=data1.remove(x,serialBuffer.size());
                    data2=data2.remove(0,x+1);}
            }
            psi=data1.toFloat();
            theta=data2.toFloat();
            qDebug() <<"Angulo psi:  "<< psi;
            qDebug() <<"Angulo theta:  "<< theta <<endl;
        }
    }

return 0;
}

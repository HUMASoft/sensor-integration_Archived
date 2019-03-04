#include "SerialArduino.h"

using namespace std;




SerialArduino::SerialArduino()
{
//    QApplication a(argc, argv);
        arduino_is_available = false;
        arduino_port_name = "";
        port = new QSerialPort;
        dataSensor.resize(20);
        oriString.resize(20);
        incliString.resize(20);

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


long SerialArduino::readSensor(float &incli, float &orien)
{

//    //Ask for inclination value
    port->write("i",1);
//    //wait the data
    port->waitForReadyRead(-1);
    if(port->isReadable())
    {

        for (int i=0;i<dataSensor.size();i++)
         {
             // waitForReadyRead(security factor * bits/byte * ms/s / port->baudRate())
             port->waitForReadyRead(1.2*8*1000/port->baudRate());
             port->getChar(&dataSensor[i]);
             //Data read line
             if (dataSensor[i]== '\n') break;
         }

        incliString=dataSensor;
        oriString=dataSensor;
        //Find ',' in data sensor to divide in incl and orient
        for (int i=0;i<dataSensor.size();i++)
         {
        if (dataSensor[i]==','){
            incliString=incliString.erase(i,dataSensor.size());
            oriString=oriString.erase(0,i+1);
            }
         }

        //Identify data between incl and orient
        if (incliString[0]== 'i' || oriString[0]== 'o'){
        incliString=incliString.erase(0,1);
        incli = stof(incliString);
        oriString=oriString.erase(0,1);
        orien = stof(oriString);
    }

    }
    return 0;

}

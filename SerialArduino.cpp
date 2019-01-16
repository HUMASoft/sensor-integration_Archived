#include "SerialArduino.h"

using namespace std;

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
            port->setBaudRate(QSerialPort::Baud38400);
            port->setDataBits(QSerialPort::Data8);
            port->setParity(QSerialPort::NoParity);
            port->setStopBits(QSerialPort::OneStop);
            port->setFlowControl(QSerialPort::NoFlowControl);
            //QObject::connect(arduino, SIGNAL(readyRead()), this, SLOT(readSerial()));
    }


}

long SerialArduino::ReadSensor(double &incli, double &orien)
/*{
    port->waitForReadyRead(100);
    if(port->isReadable()){
        dataread = port->readLine();
        if(dataread!="\n"){
//            qDebug() <<"Dato total:"<<dataread;
            serialBuffer = QString::fromStdString(dataread.data());
            data1=serialBuffer;
            data2=serialBuffer;
            for (x=0; x <= serialBuffer.size(); x++)
            {
                if (serialBuffer[x]==','){
                    data1=data1.remove(x,serialBuffer.size());
                    data2=data2.remove(0,x+1);}
            }

            orien=data1.toFloat();
            incli=data2.toFloat();

//            cout << "ReadSensor" << orien << endl << endl;
//            qDebug() <<"Angulo psi:  "<< psi;
//            qDebug() <<"Angulo theta:  "<< theta <<endl;
        }
    }

return 0;
}*/
{

    if (port->waitForReadyRead(1))
    {
        if(port->isReadable()){
            dataread = port->readLine();
            if(dataread!="\n"){
                //qDebug() <<"Dato total:"<<dataread;
                serialBuffer = QString::fromStdString(dataread.data());
                data1=serialBuffer;
                data2=serialBuffer;
                //qDebug() <<"data2:  "<< data2;
                for (x=0; x <= serialBuffer.size(); x++)
                {
                    if (serialBuffer[x]==','){
                        data1=data1.remove(x,serialBuffer.size());
                        data2=data2.remove(0,x+1);}
                }
//                qDebug() <<"1:  "<< data1;
//                qDebug() <<"2:  "<< data2;

                theta=data1.toFloat();
                phi=data2.toFloat();
                orien=theta;
                incli=phi;

//                            cout << "ReadSensor" << orien << endl << endl;

//                            qDebug() <<"Angulo theta:  "<< theta;
//                            qDebug() <<"Angulo psi:  "<< data2 <<endl;
            }
        }
    }
    else
    {
        orien=theta;
        incli=phi;
    }
return 0;
}

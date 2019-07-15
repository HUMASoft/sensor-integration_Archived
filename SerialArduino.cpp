#include "SerialArduino.h"

using namespace std;




SerialArduino::SerialArduino()
{
//    QApplication a(argc, argv);
//    QApplication a(0, 0);
//    QCoreApplication app();
        arduino_is_available = false;
        arduino_port_name = "";
        port = new QSerialPort;
        dataSize = 20;
        dataSensor.resize(dataSize);
        oriString.resize(dataSize);
        incliString.resize(dataSize);

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
//                   if(serialPortInfo.portName()=="ttyACM0")
                   {
                    arduino_port_name=serialPortInfo.portName();
                    cout << "arduino_port_name: " << arduino_port_name.toUtf8().constData() << endl;
                    arduino_is_available = true;
                   }
                 }
               }
            }
         }

//        if(arduino_is_available && arduino_port_name=="ttyACM0"){
            // open and configure the serialport
            port->setPortName(arduino_port_name);

            if (port->open(QIODevice::ReadWrite))
            {
                cout << "port->open" << endl;
                arduino_is_available = true;
            }
            else
            {
                arduino_is_available = false;
                cout << "port->error. Check if user is in dialout group (sudo usermod -a -G dialout <user>)." << endl;

            }

//            port->setBaudRate(QSerialPort::Baud9600);
            cout << "setBaudRate:" << port->setBaudRate(QSerialPort::Baud115200);
            cout << ", setDataBits: " << port->setDataBits(QSerialPort::Data8);
            cout << ", setParity: " << port->setParity(QSerialPort::NoParity);
            cout << ", setStopBits: " << port->setStopBits(QSerialPort::OneStop);
            cout << ", setFlowControl: " << port->setFlowControl(QSerialPort::NoFlowControl) << endl;
            //QObject::connect(arduino, SIGNAL(readyRead()), this, SLOT(readSerial()));
//    }


}


long SerialArduino::readSensor(float &incli, float &orien)
{

//    //Ask for inclination value
    port->write("i",1);
//    //wait the data
    //This read should not block more than a second.
//    if(!port->waitForReadyRead(1000)) return -1;
    // waitForReadyRead(security factor * data string size * bits/byte * ms/s / port->baudRate())
    if (!port->waitForReadyRead(1.2*8*1000/port->baudRate())) return -1;

//    if( port->isReadable())
    {
        port->readLine(dataarray,dataSize);

//        for (int i=0;i<dataSize;i++)
//        {
////            port->waitForReadyRead(1.2*8*1000/port->baudRate());
//            // waitForReadyRead(security factor * bits/byte * ms/s / port->baudRate())
//            if (!port->waitForReadyRead(1000*1.2*8*1000/port->baudRate()))
//            {
//                cout << "Port tiemout!!!" << endl;
//                cerr << "Port tiemout!!!" << endl;
//                return -1;
//            }

//            //Data read line
//            port->getChar(&dataSensor[i]);
//            if (dataSensor[i]== '\n') break;
//        }

        dataSensor = string(dataarray);
        incliString=dataSensor;
        oriString=dataSensor;
        //Find ',' in data sensor to divide in incl and orient
        for (int i=0;i<dataSensor.size();i++)
        {
            if (dataSensor[i]==',')
            {
                incliString=incliString.erase(i,dataSensor.size());
                oriString=oriString.erase(0,i+1);
            }
        }

        //Identify data between incl and orient
        if (incliString[0]== 'i' || oriString[0]== 'o')
        {
            if (incliString[1] < '0' || incliString[1] > '9')
            {
                cout << "Wrong incliString!!! ->" << incliString[1] << endl;
                cerr << "Wrong incliString!!! ->" << incliString[1] << endl;
                return -1;

            }
            else
            {
                //remove the initial letter (i/o)
                incliString=incliString.erase(0,1);
                incli = stof(incliString);
            }
            if (oriString[1] < '0' || oriString[1] > '9')
            {
                cout << "Wrong oriString: " << oriString[1] << endl;
                cerr << "Wrong oriString: " << oriString[1] << endl;
                return -1;

            }
            else
            {
                //remove the initial letter (i/o)
                oriString=oriString.erase(0,1);
                orien = stof(oriString);
            }

        }


    }
//    else //
//    {
//        return -1;
//    }
    return 0;

}

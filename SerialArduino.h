#ifndef SERIALARDUINO_H
#define SERIALARDUINO_H

#include <iostream>
#include <string>

#include <QSerialPort>
#include <QSerialPortInfo>
#include <QDebug>
#include <QtCore>

using namespace std;

class SerialArduino
{
public:
    SerialArduino();

    long readSensor(float &incli, float &orien);


private:
    QSerialPort *port;
    static const quint16 arduino_uno_vendor_id = 9025;
    static const quint16 arduino_MEGA = 66;
    QString arduino_port_name;
    bool arduino_is_available;
    QByteArray dataread;
    QByteArray datareadInc;
    QByteArray datareadOri;
    QString serialBuffer, data1, data2;
    string oriString, incliString;
    string dataSensor;

    int x;
    float theta, phi;
};

#endif // SERIALARDUINO_H

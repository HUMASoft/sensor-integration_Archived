#ifndef SERIALARDUINO_H
#define SERIALARDUINO_H

#include <iostream>

#include <QSerialPort>
#include <QSerialPortInfo>
#include <QDebug>

class SerialArduino
{
public:
    SerialArduino();
    long ReadSensor(double &incli, double &orien);

private:
    QSerialPort *port;

    static const quint16 arduino_uno_vendor_id = 9025;
    static const quint16 arduino_MEGA = 66;
    QString arduino_port_name;
    bool arduino_is_available;
    QByteArray dataread;
    QString serialBuffer, data1, data2;
    int x;
    float theta, psi;
};

#endif // SERIALARDUINO_H

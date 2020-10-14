#ifndef SERIALCOMM_H
#define SERIALCOMM_H

#include <boost/asio.hpp> // include boost
#include <boost/asio/serial_port.hpp>
#include <iostream>
#include <string>

using namespace std;
using namespace boost::asio;
//using namespace boost;

// These are the values our port needs to connect
#ifdef _WIN64
// windows uses com ports, this depends on what com port your cable is plugged in to.
const char *PORT = "COM3";
#else
// *nix com ports
    const char *PORT = "/dev/ttyUSB0";
#endif


class SerialComm
{
public:
    SerialComm(string portName = PORT);

    //read functions
    long ReadLine(string & out_str);

private:

    io_context io; //Active I/0 Functions
    serial_port* port; //Creation of the object

    boost::system::error_code error;
    boost::asio::streambuf buffer;

    string temp_s;


};

#endif // SERIALCOMM_H

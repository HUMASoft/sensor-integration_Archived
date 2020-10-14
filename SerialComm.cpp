#include "SerialComm.h"

SerialComm::SerialComm(string portName)
{

    port = new boost::asio::serial_port(io);

    port->open(portName);
    port->set_option(boost::asio::serial_port_base::baud_rate(9600));

    cout << "init ok" << endl;

}

long SerialComm::ReadLine(string &out_str)
{

    boost::asio::read_until( *port, buffer, "\n", error );

    std::istream str(&buffer); //Transform our info buffer into a string
    std::getline(str, out_str); //Copy of the info from our buffer to our new string

    return 0;

}

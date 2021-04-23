#ifndef IMU3DMGX510_HPP
#define IMU3DMGX510_HPP

#define CAL_LOOPS 5 //number of initial calibration attempts
#define T_OUT 3.0
#define T_WAIT 0.1

#include <iostream>
#include <sstream>
#include <string>
#include <math.h>
#include "attitude_estimator.h"
#include <tuple>
#include <boost/algorithm/hex.hpp>
using namespace boost::algorithm;

#include "SerialComm.h"
using namespace std;
using namespace stateestimation;

class IMU3DMGX510
{

public:

    IMU3DMGX510(string portName = "/dev/ttyUSB0", int new_freq = 100); //Constructor

     // -------- Initialization of the IMU. Implementation in imu3dmgx510.cpp --------

    bool check(); //This funcion checks if our imu is active
    bool set_freq(int); //This funcion will set the freq of our IMU
    bool calibrate(); //This func will get initial offsets to correct future measures

     // -------- Configuration of the IMU. Implementation in imu3dmgx510.cpp --------

    bool set_IDLEmode();  //This function sets our device into IDLE mode
    bool set_streamon(); //This function enable stream
    bool set_streamoff(); //This function unenable stream
    bool set_reset(); //This function resets the device
    bool set_devicetogetgyroacc(); //This function configure our device to give us gyro(x,y,z) and acc(x,y,z)
    bool set_devicetogetgyro(); //This function configure our device to give us gyro(x,y,z)

    // -------- Getting data of the IMU (Polling and Streaming). Implementation in imu3dmgx510.cpp --------

    std::tuple <float, float, float> get_gyroPolling();
    double* get_euleranglesPolling();

   //This methods are developed to plot specified numbres of samples on Matlab
    //We will get a vector to be copy pasted in Matlab to plot it
    std::tuple <double*,double*,double*> get_gyroStreaming (int); //This funcion gives us gyro data
    std::tuple <double*,double*,double,double> get_euleranglesStreaming (int); //This funcion gives us pitch and roll, and both initial pitch offset and initial roll offset

    //Both following methods are done to make our imu start sending data
    //They can be included in a loop
    double* EulerAngles();
    double* GyroData();

private: //Attributes

    string portResponse;

    union ulf
    {
        unsigned long ul;
        float f;
    };

    SerialComm port; //DO NOT try to invoce the constructor here
    AttitudeEstimator estimador;

    //Initial offset
    double rolloffset;
    double pitchoffset;

    //Setting of GyroBias
    double bx = -0.002786;
    double by = -0.001833;
    double bz = -0.001066;

    //Defaults gains used
    double Kp = 2.2;
    double Ti = 2.65;
    double KpQuick = 10;
    double TiQuick = 1.25;

    //Frequency of our imu
    double freq;
    double period;

    //Concrete data packets used by IMU3DMGX510
    std::string idle = "\x75\x65\x01\x02\x02\x02\xe1\xc7";
    string respuestacorrectaidle = ("\x75\x65\x01\x04\x04\xF1\x02\x00\xD6\x6C");
    std::string imudata1 = "\x75\x65\x0c\x07\x07\x08\x01\x01\x05\x03\xe8\xee\x04";
    std::string imudata100 = ("\x75\x65\x0c\x07\x07\x08\x01\x01\x05\x00\x0a\x0d\x20");
    std::string imudata1000 = ("\x75\x65\x0c\x07\x07\x08\x01\x01\x05\x00\x01\x04\x17");
    std::string reset = "\x75\x65\x01\x02\x02\x7e\x5d\x43";
    std::string respuestacorrectareset = ("\x75\x65\x01\x04\x04\xF1\x7e\x00\x52\x64");
    std::string baudratenew = ("\x75\x65\x0c\x07\x07\x40\x01\x00\x03\x84\x00\xbc\x64");
    std::string gyracc1 = ("\x75\x65\x0c\x0a\x0a\x08\x01\x02\x04\x03\xe8\x05\x03\xe8\xe4\x0b");
    std::string gyracc50 = ("\x75\x65\x0c\x0a\x0a\x08\x01\x02\x04\x00\x14\x05\x00\x14\x36\xd2");
    std::string gyracc100 = ("\x75\x65\x0c\x0a\x0a\x08\x01\x02\x04\x00\x0a\x05\x00\x0a\x22\xa0");
    std::string gyracc500 = ("\x75\x65\x0c\x0a\x0a\x08\x01\x02\x04\x00\x02\x05\x00\x02\x12\x78");
    std::string gyracc1000 = ("\x75\x65\x0c\x0a\x0a\x08\x01\x02\x04\x00\x01\x05\x00\x01\x10\x73");
    std::string respuestacorrectaajustes = ("\x75\x65\x0c\x04\x04\xF1\x08\x00\xE7\xBA");
    std::string streamon = "\x75\x65\x0c\x05\x05\x11\x01\x01\x01\x04\x1a";
    std::string streamoff = ("\x75\x65\x0c\x05\x05\x11\x01\x01\x00\x03\x19");
    std::string respuestacorrectastreamonoff = ("\x75\x65\x0c\x04\x04\xF1\x11\x00\xf0\xcc");
    std::string polling = ("\x75\x65\x0c\x04\x04\x01\x00\x00\xef\xda");
};

#endif // IMU3DMGX510_HPP

#include "SerialComm.h"

SerialComm::SerialComm(string portName)
{

    port = new boost::asio::serial_port(io);
    port->open(portName);
    port->set_option(boost::asio::serial_port_base::baud_rate(115200));
    if (port->is_open()){
         cout << "Port " << portName << " has been correctly initialized" << endl;
    }else{
        cout << "Port " << portName << " can't be opened" << endl;
    }

}

string SerialComm::ReadLine()
{
    string reading;
    //We'll read data till standar final carriage \n
    boost::asio::read_until( *port, buffer, "\n", error );
    std::istream str(&buffer); //Transform our info buffer into a string
    std::getline(str, reading); //Copy of the info from our buffer to our new string
//    cout << "Read data: " << reading << endl;
    return reading;

}

char SerialComm::ReadChar(){

    char a;
    //This function will read a single char
    boost::asio::read(*port,boost::asio::buffer(&a, 1));
//    cout << "Read char: " << a << ", en int es: "<< int(a)<< endl;
    return a;
}

string SerialComm::ReadNumberofChars(int size){

    //This function will read a concrete number of chars
    char a;
    string reading;
    for (int i=0; i<= size-1;i++){
    boost::asio::read(*port,boost::asio::buffer(&a, 1));
    reading += a;
    }
//    cout << "El conjunto de los " << size << " charts leidos es: " << reading << endl;

    return reading;
}

string SerialComm::ReadUntill(char a){

    string reading;
    boost::asio::read_until(*port, buffer, a, error );
    std::istream str(&buffer); //Transform our info buffer into a string
    std::getline(str, reading); //Copy of the info from our buffer to our new string
//    cout << "Read data: " << reading << endl;

    return reading;
}

int SerialComm::CheckLine(string checkline){

    string deviceanswerr;
    int flag1=0;
    int flag2=0;
    int flag4=0;
    int flag3=0;
    char deviceanswer_desc;
    char deviceanswer_length;
    char a;


    //We use this func to read a line and to compare it with the one we want
    //If they match, func will return 1.

    do{
        do{
            //Reset of some variables to avoid infinite loops
            deviceanswerr.clear();
            flag1=0;
            flag2=0;
            flag4=0;
            flag3=0;

            do{

            boost::asio::read(*port,boost::asio::buffer(&a, 1));
            switch (a) {
            case 'u':{
                flag1=1;
                deviceanswerr+=a;
                break;}
            case 'e':{
                if (flag1==1){
                    flag2=1;
                    deviceanswerr+=a;
                }else{
                    flag1=0;
                    deviceanswerr+=a;
                }
                break;}
            default:{
                deviceanswerr.clear();
                break;}
            }
        }while (flag2==0);

        boost::asio::read(*port,boost::asio::buffer(&deviceanswer_desc, 1));
        deviceanswerr +=deviceanswer_desc;
        boost::asio::read(*port,boost::asio::buffer(&deviceanswer_length, 1));
        deviceanswerr +=deviceanswer_length;

        if (int(deviceanswer_length) != int(checkline.at(4))){
            flag3=0;
        }else{
            flag3=1;

        }

        }while (flag3==0);

        for (int i=0; i<= ((int)deviceanswer_length+1); i++){
            boost::asio::read(*port,boost::asio::buffer(&a, 1));
            deviceanswerr +=a;
        }

        if (deviceanswerr==checkline){
            flag4=1;
//            cout << "La cadena es correcta" << endl;

        }else{
            deviceanswerr.clear();
            flag4=0;
//            cout << "La cadena no es correcta" << endl;

        }
    }while(flag4==0);
    return flag4;
}

long SerialComm::WriteLine(string in_str){

    //This function will write a specified data packet via serial comm
    //We want to be sure that the data packet we're sending to our device ends up with \n

    int flagcomprobacion=0;
    string check;
    string a;
    string b;
    string finalcarriage = "\\n";

    for (unsigned int i=0 ; i<=in_str.size()-1;i++){

        //We read every single char of the send string
        //We need to find final carriage. If we find so, 'flagcomprobacion' is set to 1.
        //Final carriage is \n.

        if (i>0){
        b = in_str.at(i);
        a = in_str.at(i-1);
        check = a+b;
        }

        if (check == finalcarriage){
             flagcomprobacion=1;
         }
    }

    //If we don't, final carriage will be add at the end of the string to apply standard communication protocol.
    if (flagcomprobacion==0){
        in_str+='\n';
    }

    //Finally, we sent our string with final carriage added
    boost::asio::write(
                *port,
                boost::asio::buffer(in_str.c_str(), in_str.size()),
                boost::asio::transfer_at_least(in_str.size()),
                error
                );

    return 0;

}
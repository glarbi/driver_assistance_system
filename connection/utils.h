
//#include <stdio.h>  // for extern periphirique
//#include <fstream>  // fstream to read write create files
#include <iostream>
#include <typeinfo>
#include <./SerialPort.h>
#include <unistd.h>
#include "SerialStream.h"



#define MAX_SIZE 5

using namespace LibSerial;
using namespace std;

class connection
{
  public:
	connection(string);
	~connection();

	SerialPort serial_port;

	void sendData(string);   // send a string data
	void sendData(char);   // send a string data
	string reciveData();
};

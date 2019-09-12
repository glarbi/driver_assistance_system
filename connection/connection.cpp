#include "utils.h"

connection::connection(string port_name )
{
        	char x[13]= "/dev/ttyACM0"; // for usb device
	//char x[13] = "/dev/rfcomm4";

        cout << "Port name (ttyACM0, rfcomm4) :" << endl;
        string po ; 
	    cin >> po;
        
        string tmp = "/dev/" + po ;

        /*char x[13];
	cin >> x;
	char *tmp = (char *)x;
	*/
/*vector<std::__cxx11::string> por = serial_port.GetAvailableSerialPorts();
	for (int i = 0; i < por.size(); i++)
		cout << por[i] << endl;*/

	try
	{
		//serial_port.Open(port_name);
		serial_port.Open(tmp);
		cout << "connection established" << endl;
	}

	catch (const OpenFailed &)
	{
		std::cerr << "The serial port did not open correctly." << std::endl;
	}

	serial_port.SetBaudRate(BaudRate::BAUD_9600);			  // Set the baud rate of the serial port.
	serial_port.SetCharacterSize(CharacterSize::CHAR_SIZE_8); // Set the number of data bits.

	// serial_port.SetFlowControl(FlowControl::FLOW_CONTROL_NONE); // Turn off hardware flow control.
	//serial_port.SetParity(Parity::PARITY_NONE); // Disable parity.
	//serial_port.SetStopBits(StopBits::STOP_BITS_1); // Set the number of stop bits.
	

  
}

connection::~connection()
{  
	sendData('1');      //turn galaxy led on red
	serial_port.Close(); 
	cout << "connection has closed" << endl;
	serial_port.~SerialPort();
}

void connection::sendData(string data )
{  
   if (serial_port.IsOpen()){
        for (int i = 0 ; i < data.length();i++ ){
	         char s = data[i];
			 cout << s << endl;
	         serial_port.WriteByte(s);  	// Write the data to the serial port.
	         serial_port.DrainWriteBuffer(); // Wait until the data has actually been transmitted.
	    }
	   	serial_port.WriteByte('\n');	// clean arduino buffer
   }
      
}

void connection::sendData(char data )
{  
   if (serial_port.IsOpen()){
       
	    serial_port.WriteByte(data);  	// Write the data to the serial port.
        serial_port.DrainWriteBuffer(); // Wait until the data has actually been transmitted.    
	   	serial_port.WriteByte('\n');	// clean arduino buffer
   }
      
}


string connection::reciveData()
{   
	    string v ="";
        size_t ms_timeout = 250;  // Specify a timeout value (in milliseconds).      
        DataBuffer read_buffer ;
        try
        {
            serial_port.Read(read_buffer, 0, ms_timeout);   // Read as many bytes as are available during the timeout period.
        }
        catch (const ReadTimeout &)
        {
            for (size_t i = 0; i < read_buffer.size(); i++)
            {
                v += read_buffer.at(i)  ;
            }
        }
          
    return v ;
}

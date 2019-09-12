#include <SoftwareSerial.h>
SoftwareSerial BT(1, 0);   // pin 12 is broken

byte alert    = 0;
char  protocol = 0 ; // A empty protocol for each eteration

void loop()
{
  //if (Serial.available())
    //protocol = Serial.read();
  if (BT.available())
    protocol = BT.read();
 
  switch (protocol)
  {
    case 'Q': shutdown(); digitalWrite(10, HIGH);  BT.println("go left   " );    break;
    case 'S': shutdown(); digitalWrite(13, HIGH);  BT.println("go right  " );    break;
    case 'R': shutdown(); BT.println("go forward" );    break;
    //-----------------------------------------------------------------------------------------------------
    //AutoPilot ON
    case 'A': digitalWrite(4, HIGH); BT.println("AutoPilot ON"   );shutdown();  digitalWrite(13, HIGH); delay(1000); shutdown();  break;
    //AutoPilot OFF
    case 'a': digitalWrite(4, LOW) ; BT.println("AutoPilot OFF"  );shutdown();  digitalWrite(10, HIGH); delay(1000); shutdown();  break;
    //------------------------------------------------------------------------------------------------------
    case 'E': digitalWrite(9, HIGH );  BT.println("contact on  ");      break;  // start the char  protocol = 0 ; // A empty protocol for each eteration
    case 'e': digitalWrite(9, LOW  );  BT.println("engine stop ");      break;  // stop  the engine

    case 'w': starter (1000, 2637, 400) ;  BT.println("starter on");   break;  // turn starter for 1000 ms
    //-------------------------------------------------------------------------------------------------------
    case '0':  analogWrite (3, 0   );  break;  // galaxy led
    case '1':  analogWrite (3, 100 );  break;
    case '2':  analogWrite (3, 255 );  break;
    case '3':  digitalWrite(4, HIGH);  break;  // blue led
    case '4':  digitalWrite(4, LOW );  break;
    //------------------------------------------------------------------------------------------------------
    case '8':  alert = 1;    break;  // sleeping alert
    case '9':  alert = 0;    break;
  }

  if  (alert == 1)
    bip(500);
}

//  'R' forward   'T' back   'Q' left   'S' right


void setup()
{
  //Serial.begin(9600);
  BT.begin(9600);

  pinMode(2,  OUTPUT);  // baf
  pinMode(3,  OUTPUT);  // galaxy led
  pinMode(4,  OUTPUT);  // blue led

  pinMode(13, OUTPUT); // whells
  pinMode(10, OUTPUT); // whells

  pinMode(9, OUTPUT) ; // contact
  pinMode(11, OUTPUT); // starter

  analogWrite(3, 100); // switch galaxy to red

  if (BT.available())
  BT.println("Hello from Arduino");

}

void starter (int delaay, int tonne, int delton) {

  for (int x = 0 ; x < 3 ; x++ )
  {
    tone(2, tonne, delton);
    delay(delton) ;
  }
  digitalWrite(4, HIGH );//should be  11
  delay(delaay) ;
  digitalWrite(4, LOW );

}

void bip(int del) {
  tone(2, 2637, 400);
  delay(del);           // release time for tone

}

void shutdown() {
  digitalWrite(13, LOW);
  digitalWrite(10, LOW );

}

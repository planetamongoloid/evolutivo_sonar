/*
  UDPSendReceive.pde:
 This sketch receives UDP message strings, prints them to the serial port
 and sends an "acknowledge" string back to the sender
 
 A Processing sketch is included at the end of file that can be used to send 
 and received messages for testing with a computer.
 
 created 21 Aug 2010
 by Michael Margolis
 
 This code is in the public domain.
 */


#include <SPI.h>         // needed for Arduino versions later than 0018
#include <Ethernet.h>
#include <EthernetUdp.h>         // UDP library from: bjoern@cs.stanford.edu 12/30/2008

const int pingPin = 7;
char buf[12];
float valor_array[10] = {
  290,290,290,290,290,290,290,290,290,290};
long cm_last;
long error;
// Enter a MAC address and IP address for your controller below.
// The IP address will be dependent on your local network:
byte mac[] = {  
  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
IPAddress ip(192, 168, 1, 33);
byte remoto[] = {
  192, 168, 1, 15 };
unsigned int remotoPort = 6000;
unsigned int localPort = 8888;      // local port to listen on

// buffers for receiving and sending data
char packetBuffer[UDP_TX_PACKET_MAX_SIZE]; //buffer to hold incoming packet,
char  ReplyBuffer[] = "acknowledged";       // a string to send back

int VENTANA = 13;
// An EthernetUDP instance to let us send and receive packets over UDP
EthernetUDP Udp;

void setup() {
  // start the Ethernet and UDP:
  Ethernet.begin(mac,ip);
  Udp.begin(localPort);

  Serial.begin(9600);
}

void loop() {
  // establish variables for duration of the ping, 
  // and the distance result in inches and centimeters:
  long duration, inches, cm;

  // The PING))) is triggered by a HIGH pulse of 2 or more microseconds.
  // Give a short LOW pulse beforehand to ensure a clean HIGH pulse:
  pinMode(pingPin, OUTPUT);
  digitalWrite(pingPin, LOW);
  delayMicroseconds(2);
  digitalWrite(pingPin, HIGH);
  delayMicroseconds(5);
  digitalWrite(pingPin, LOW);
  // The same pin is used to read the signal from the PING))): a HIGH
  // pulse whose duration is the time (in microseconds) from the sending
  // of the ping to the reception of its echo off of an object.
  pinMode(pingPin, INPUT);
  duration = pulseIn(pingPin, HIGH);
  cm = microsecondsToCentimeters(duration);
  
  error= cm;
  // hago un map porque hemos tenido que reducir el recorrido del carril tenemos menos longitud
  // el valor 200 se podria hacer variable, incluir un potenciometro?
  cm = map(cm,5,143,0,290);
  
  valor_array[0] = valor_array[1];
  valor_array[1] = valor_array[2];
  valor_array[2] = valor_array[3];
  valor_array[3] = valor_array[4];

  valor_array[4] = valor_array[5];
  valor_array[5] = valor_array[6];
  valor_array[6] = valor_array[7];
  valor_array[7] = valor_array[8];
  valor_array[8] = valor_array[9];
  // guardo en la ultima posicion
  valor_array[9] = cm;
  // check de el valor anterior , le pongo una ventana de valores para evitar flicket
  // en el inicio hay que llevar la pantalla hasta el principio porque empieza el array con valor 290
  // si cuando arrancas el mac esta la pantalla en el medio obviamente el valor es mayor que la ventana y siempre quedaria en 290
  if(valor_array[9] > valor_array[8]+VENTANA || valor_array[9] < valor_array[8]-VENTANA){
      
    valor_array[9] = valor_array[8];
  }
  cm = round((valor_array[0] + valor_array[1] + valor_array[2] + valor_array[3] + valor_array[4] + valor_array[5] + valor_array[6] + valor_array[7] + valor_array[8] + valor_array[9]) / 10);
  //Serial.println("enviando");
 

  Serial.print(cm);
  Serial.print("cm--");
  Serial.print(error);
  Serial.print("++++");

  Serial.print(valor_array[0]);
  Serial.print(",");
  Serial.print(valor_array[1]);
  Serial.print(",");
  Serial.print(valor_array[2]);
  Serial.print(",");
  Serial.print(valor_array[3]);
  Serial.print(",");
  Serial.print(valor_array[4]);
  Serial.print(",");
  Serial.print(valor_array[5]);
  Serial.print(",");
  Serial.print(valor_array[6]);
  Serial.print(",");
  Serial.print(valor_array[7]);
  Serial.print(",");
  Serial.print(valor_array[8]);
  Serial.print(",");
  Serial.print(valor_array[9]);

  Serial.println();
  if(cm != cm_last){
    cm_last = cm;
    // send a reply, to the IP address and port that sent us the packet we received
    Udp.beginPacket(remoto, remotoPort);
    Udp.write(itoa(cm,buf,10));
    //Udp.write("5");
    Udp.endPacket();
  }

  delay(50);
}
long microsecondsToCentimeters(long microseconds)
{
  // The speed of sound is 340 m/s or 29 microseconds per centimeter.
  // The ping travels out and back, so to find the distance of the
  // object we take half of the distance travelled.
  return microseconds / 29 / 2;
}








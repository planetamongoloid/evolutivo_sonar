#include <SPI.h>         // needed for Arduino versions later than 0018
#include <Ethernet.h>
#include <EthernetUdp.h>         // UDP library from: bjoern@cs.stanford.edu 12/30/2008
// Enter a MAC address and IP address for your controller below.
// The IP address will be dependent on your local network:
char buf[12];
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
// An EthernetUDP instance to let us send and receive packets over UDP
EthernetUDP Udp;

int ultraSoundSignalPins[] = {
  7,8}; // Front Left,Front, Front Right, Rear Ultrasound signal pins
char *pingString[] = {
  "S1 ","S2 "}; // just something to print to indicate direction
float valor_array1[10] = {
  3,3,3,3,3,3,3,3,3,3
};
float valor_array2[10] = {
  3,3,3,3,3,3,3,3,3,3
};
int VENTANA = 3;
int size1;
void setup()
{
  size1 = 10;
  Ethernet.begin(mac,ip);
  Udp.begin(localPort);
  Serial.begin(9600);
}

//Ping function
unsigned long ping(int index)
{
  unsigned long echo;

  pinMode(ultraSoundSignalPins[index], OUTPUT); // Switch signalpin to output
  digitalWrite(ultraSoundSignalPins[index], LOW); // Send low pulse
  delayMicroseconds(2); // Wait for 2 microseconds
  digitalWrite(ultraSoundSignalPins[index], HIGH); // Send high pulse
  delayMicroseconds(5); // Wait for 5 microseconds
  digitalWrite(ultraSoundSignalPins[index], LOW); // Holdoff
  pinMode(ultraSoundSignalPins[index], INPUT); // Switch signalpin to input
  digitalWrite(ultraSoundSignalPins[index], HIGH); // Turn on pullup resistor
  echo = pulseIn(ultraSoundSignalPins[index], HIGH); //Listen for echo
  //return (echo / 58.138) * .39; //convert to CM then to inches
  return (echo / 58.138);
}
float getMedia(int i,float valor){
  float media = 0;
  float media2 = 0;
  for(int j=0; j < size1 ;j++){
    if(j < size1-1){
      if(i<1){
        valor_array1[j] = valor_array1[j+1];
      }
      else{
        valor_array2[j] = valor_array2[j+1];
      }
     
    }//EL ULTIMO VALOR
    else{
      if(i<1){
        //if(valor_array1[9] > valor_array1[8]+VENTANA || valor_array1[9] < valor_array1[8]-VENTANA){
          valor_array1[size1-1] = valor;
        //}
      }
      else{
        //if(valor_array2[9] > valor_array2[8]+VENTANA || valor_array2[9] < valor_array2[8]-VENTANA){
          valor_array2[size1-1] = valor;
        //}
      }
      //Serial.print(valor_array1[size1-1]);
    }
    media = media + valor_array1[j];
    media2 = media2 + valor_array2[j];
    //Serial.println(media2);
//    Serial.print(",");
  }
  //Serial.println();
  //Serial.print(valor_array1[9]);
  if(i==0){
    return round(media/10);
  }else{
    return round(media2/10);
  }
  
}

void loop()
{
  unsigned long ultrasoundValue;
  unsigned long send1,send2,send1_last,send2_last;
  float media,media2;
  for(int i=0; i < 2; i++){
    ultrasoundValue = ping(i);
    if(i==0){
      media = getMedia(i,ultrasoundValue);
    }else{
      media2 = getMedia(i,ultrasoundValue);
    }
    //Serial.print(media2);
    //Serial.print("...");
//    //Serial.print(pingString[i]);
    //Serial.print(ultrasoundValue);
    //Serial.print(",");
//    if(i==0){
//      //Serial.print("cm, ");    
//    }
//    else{
//      Serial.print("cm, "); 
//    }
//    delay(50);
      
  }
  for(int i=0;i<10;i++){
    Serial.print(valor_array2[i]);
    Serial.print(",");
  }
  Serial.print("-----");
  send1 = map(media,9,145,0,145);
  send2 = map(media2,7,150,290,145);
  Serial.print(media);
  Serial.print(",");
  Serial.print(media2);
//  Serial.print("->");
//  Serial.print(ceil(media2));
  Serial.print("--map->");
  Serial.print(send1);
  Serial.print(",");
  Serial.print(send2);
  Serial.println();
  //Serial.print("la media1 ");
  //Serial.println(media);
  // send a reply, to the IP address and port that sent us the packet we received
    Udp.beginPacket(remoto, remotoPort);
    if(media<=145){
      if(send1 != send1_last){
        send1_last = send1;
        Udp.write(itoa(send1,buf,10));
      }
    }else{
      if(send2 != send2_last){
        send2_last = send2;
        Udp.write(itoa(send2,buf,10));
      }
    }
    //Udp.write("5");
    Udp.endPacket();
  delay(50);
  
} 




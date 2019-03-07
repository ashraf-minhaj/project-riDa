#include <String.h>

float t=0.0;
int x = 40;

void setup()
{
  Serial.begin(9600);               // the GPRS baud rate   
  //Serial.begin(9600);    // the GPRS baud rate 
  
  delay(1000);
}
 
void loop()
{
      t=analogRead(A2);
      t=t*0.4887;  
      delay(2);          
     // SendX();

      delay(20000);
  
  //if (mySerial.available())
    //Serial.write(mySerial.read());

 //connect to gprs network
  Serial.println("AT+CGATT?");
  delay(1000);


  //deactivate the gprs pdp context
  Serial.println("AT+CIPSHUT");
  delay(1000);

  //may be not for sim808
  //mySerial.println("AT+CIPSTATUS");
  //delay(2000);

  //stat single IP connection
  Serial.println("AT+CIPMUX=0");
  delay(2000);

  //Start task and set APN, user name, password 
  Serial.println("AT+CSTT=\"internet\""); //start task and setting the APN,
  delay(1000);

  //add max response time
  Serial.println("AT+CIICR"); //bring up wireless connection
  delay(3000);

  //Get Local IP Address (ciicr is needed first)
  Serial.println("AT+CIFSR"); //get local IP adress
  delay(2000);

  //single IP connection (+CIPMUX=0) 
  Serial.println("AT+CIPSPRT=0");
  delay(3000);
 
  Serial.println("AT+CIPSTART=\"TCP\",\"api.thingspeak.com\",\"80\"");//start up the connection
  delay(6000);
 
  

 //
  Serial.println("AT+CIPSEND");//begin send data to remote server
  delay(4000);

  //the jamuna
  String str="GET http://api.thingspeak.com/update?api_key=ZR24DI7PV2TN2Q87&field1=" + String(t) + "&field2=" + String(x);
  Serial.println(str);//begin send data to remote server
  delay(4000);
  

  Serial.println((char)26);//sending
  delay(7000);//waitting for reply, important! the time is base on the condition of internet 
  Serial.println();
 
  
 
  Serial.println("AT+CIPSHUT");//close the connection
  delay(100);

  x = x + 1;
  if(x > 48)
  {
    x = 40;
  }
  
} 


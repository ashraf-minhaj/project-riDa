/*
 * Project rida (river data)
 * by Ashraf Minhaj 2018
 * ashraf_minhaj@yahoo.com
 */
#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
//define the parameters for the IFTTT
#define HOST "maker.ifttt.com"

//float temp; 

const char* ssid = "SS-ID";
const char* password = "PASSWORD";

//int led = ;
int temS = 36;
float sval = 0;
float v2 = 302;

WiFiClientSecure client;

void setup() 
{
  //pinMode(led,OUTPUT);
  Serial.begin(9600);
  WiFi.begin(ssid, password);
  Serial.println("");
  Serial.print("Waiting to connect to WiFi... ");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  delay(1000);


  
}


void loop() 
{
  sval = analogRead(temS);
  //sval = (analogRead(A0) * 330.0) / 1023.0;
/*  if (client.connected())
  {
    client.stop();
  }

  client.flush();
  */
  if (client.connect(HOST,443)) 
  {
    //digitalWrite(led,HIGH);
    //delay(100);
    Serial.println("Sending Data to sheet");
                                       // build the HTTP request
    String toSend = "GET /trigger/";
    toSend += "The Jamuna";
    toSend += "/with/key/";
    toSend += "API_KEY"; //api key here
    toSend += "?value1=";
    toSend += "Temp:+=+";
    toSend += sval;
    toSend += "+Turbidity+=+";
    toSend += v2;
    toSend += "+pH+=+";
    toSend += "7";
    toSend += " HTTP/1.1\r\n";
    toSend += "Host: ";
    toSend += HOST;
    toSend += "\r\n";
    toSend += "Connection: close\r\n\r\n";
    client.print(toSend);

  client.flush();
  client.stop();
  }
  /*delay(1000);
  client.flush();
  client.stop();  
  //digitalWrite(led,LOW); 
  */
  delay(5000);
  
  

}



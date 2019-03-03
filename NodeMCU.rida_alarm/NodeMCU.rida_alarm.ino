/***  System ***
 *  
 *  Developed by: Ashraf Minhaj
 *  Contact: ashraf_minhaj@yahoo.com
 */

#include<ESP8266WiFi.h>  //include ESP8266WiFi library
WiFiClient client;       //name the client

WiFiServer server(80);   //server begin at 80

const char* ssid = "LIBRARY";              //WiFi Name     
const char* password = "baiustlib"; //WiFi password

//ip = 199.168.43.162     Enter this kind of IP address of your NODEMCU on the APP
//your IP address will be different


String data = "";

void setup()
{

 pinMode(5, OUTPUT);
  Serial.begin(115200);   //Serial communication for debugging
  server.begin();    //Initialize Server Communication
}

void loop()
{
  client = server.available();

  if(!client) return;

  data = checkClient();     //check for data

  Serial.println(data);    //ignore -for debugging only

  if(data == "o")
  {
    digitalWrite(5, HIGH);
  }

  else if(data == "s")
  {
    digitalWrite(5, LOW);
  }
  
 
}

String checkClient(void)
{
  while (!client.available()) delay(1);
  String request = client.readStringUntil('\r');
  request.remove(0,5);
  request.remove(request.length()-9,9);
  return request;
}


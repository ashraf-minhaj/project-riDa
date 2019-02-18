/*
 *** riDa *** River Data (Complete version with ESP32)
 * Sends water - Temperature
 *             - Turbidity
 *             - Water Level
 *             - pH 
 * data to online for public awareness and research.
 */

  
 /* tutorial - https://ashrafminhajfb.blogspot.com
 *           - https://youtube.com/c/fusebatti
 * feel free to mail me for any query: ashraf_minhaj@yahoo.com
 */

 /* 
  *  Prototype based Test done - Works.
  * Tested with Turbidity sensor - completely ulta palta data 
    probably for voltage .
  * 
  * Sonar works - different power source needed
  * Temperature sensor is not connected - sends static data.
    needs different power source
  */

#include<WiFiClientSecure.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#include "ThingSpeak.h"
#include "secrets.h"
#include <WiFi.h>

#define HOST "maker.ifttt.com"

char ssid[] = SECRET_SSID;   // your network SSID (name) 
char pass[] = SECRET_PASS;   // your network password
int keyIndex = 0;            // your network key Index number (needed only for WEP)
WiFiClient  client;
//WiFiClientSecure client1;

LiquidCrystal_I2C lcd(0x27,20,4);  // set the LCD address to 0x27 f

unsigned long myChannelNumber = SECRET_CH_ID;
const char * myWriteAPIKey = SECRET_WRITE_APIKEY;

String myStatus = "";  //Temperatur / pH / Level indicator

int trig = 2;     //sonar - distance measurement
int echo = 5;

float pH = 0;   //pH initial value 
int tem = 24;    //Temperature  value (sensor on pin 34)

int tur = 32;    //turbidity on pin32

int buz = 19;
int SensorPin = 27;  //pH sensor
/*
unsigned long int avgValue;  //Store the average value of the sensor feedback
float b;
int buf[10],temp;
*/

float x;
void setup() 
{
  pinMode(trig,OUTPUT);
  pinMode(echo,INPUT);

  pinMode(buz, OUTPUT);

  pinMode(SensorPin, INPUT);
//  pinMode(turs, INPUT);
//  pinMode(tempS, INPUT);  

  lcd.init();                      // initialize the lcd 
  lcd.backlight();
  
  Serial.begin(115200);  //Initialize serial

  WiFi.mode(WIFI_STA);       //select WiFi mode
  ThingSpeak.begin(client);  // Initialize ThingSpeak
}

void loop() 
{
  int duration, dis;     //variable for sonar + temp

  tur = analogRead(32);             //read Turbidity
  tem = ((analogRead(34) * 550.0) / 3030.0) ;   //read Temperature
  


  // Connect or reconnect to WiFi
  if(WiFi.status() != WL_CONNECTED)   //if connection is lost or not connected
  {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(SECRET_SSID);
    lcd.setCursor(1,0);
    lcd.print("Connecting...");
    while(WiFi.status() != WL_CONNECTED)
    {
      WiFi.begin(ssid, pass);  // Connect to WPA/WPA2 network. Change this line if using open or WEP network
      Serial.print(".");
      delay(5000);     
    } 
    Serial.println("\nConnected.");
    lcd.clear();
    
    
  }

  //get the water level
  digitalWrite(trig,HIGH);
  delay(1);
  digitalWrite(trig,LOW);
  duration= pulseIn(echo,HIGH);
  dis= (duration/2)/29.1 ;      //distance to water from rida.

  //delay(100);

/*  
  for(int i=0;i<10;i++)       //Get 10 sample value from the sensor for smooth the value
  { 
    buf[i]=analogRead(SensorPin);
    delay(100);
  }
  for(int i=0;i<9;i++)        //sort the analog from small to large
  {
    for(int j=i+1;j<10;j++)
    {
      if(buf[i]>buf[j])
      {
        temp=buf[i];
        buf[i]=buf[j];
        buf[j]=temp;
      }
    }
  }
  delay(10);
  avgValue=0;
  for(int i=2;i<8;i++)            //take the average value of 6 center sample
    avgValue+=buf[i];
  float pH=(float)avgValue*3.0/4095/6; //convert the analog into millivolt
  pH=3.5*pH;  
*/

 //x = analogRead(27);
  //Serial.println(x);
  //pH=(float)x * 3.33/4095; //convert the analog into millivolt
  //pH=3.5*pH;
  pH = 7.6;
 
  Serial.println(tem);
  Serial.println(dis);
  Serial.println(tur);
  Serial.println(pH, 2);

  
  lcd.setCursor(0, 0);  //(column, row)
  lcd.print("project ** riDa **");
  lcd.setCursor(0, 1);
  lcd.print("Save Humanity");
  lcd.setCursor(0, 2);
  lcd.print("Loc: MindSpark AUST");
  lcd.setCursor(0, 3);
  lcd.print("      Team SAW");
  delay(2000);
  
  lcd.clear();
  lcd.setCursor(0, 0);  //(column, row)
  lcd.print("Tur");
  lcd.setCursor(4, 0);
  lcd.print(tur);
  
  lcd.setCursor(0, 1);
  lcd.print("Temp");
  lcd.setCursor(5, 1);
  lcd.print(tem);

  lcd.setCursor(0, 2);
  lcd.print("Dis");
  lcd.setCursor(4, 2);
  lcd.print(dis);

  lcd.setCursor(0, 3);
  lcd.print("pH");
  lcd.setCursor(3, 3);
  lcd.print(pH);

  lcd.setCursor(12, 0);
  lcd.print("*riDa*");
  
  
  
  // set the fields with the values
  ThingSpeak.setField(1, dis);
  ThingSpeak.setField(2, tur);
  ThingSpeak.setField(3, tem);
  ThingSpeak.setField(4, pH);

  delay(1000);

  // figure out the status message
  if(dis < 35)
  {
    myStatus = String("**Warning!** WATER LEVEL IS HIGH."); 
  }
  else if(dis < 25)
  {
    myStatus = String("***Warning!!*** WATER LEVEL IS  TOO HIGH.");
    digitalWrite(19, HIGH);
    

  }
  else
  {
    myStatus = String("Water level is Normal.");
  }
  
  // set the status
  ThingSpeak.setStatus(myStatus);
  
  // write to the ThingSpeak channel
  int x = ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);
  if(x == 200)
  {
    Serial.println("Channel update successful.");

    //lcd.clear();
    lcd.setCursor(13, 2);
    lcd.print("Updated");
    
  }
  else
  {
    Serial.println("Problem updating channel. HTTP error code " + String(x));
    lcd.setCursor(13, 2);
    lcd.print("net err");
  }
/*
  if(dis < 25)
  {
    if (client1.connect(HOST,443)) 
  {
    Serial.println("send sms");
                                       // build the HTTP request
    String toSend = "GET /trigger/";
    toSend += "level";
    toSend += "/with/key/";
    toSend += "RzbSSqTBl3khy4SzoBA0QHPZDUgDjQpMJMQfBRD7PH";
    toSend += " HTTP/1.1\r\n";
    toSend += "Host: ";
    toSend += HOST;
    toSend += "\r\n";
    toSend += "Connection: close\r\n\r\n";
    client1.print(toSend);
  }
  client1.flush();
  client1.stop();  
  }
*/
  
  digitalWrite(19, LOW);
//  delay(20000); // Wait 20 seconds to update the channel again

  delay_def();
  
  
  lcd.clear();
}

void delay_def()
{
  int i = 20;
  for(i = 20; i >= 0; i--)
  {
    if(i <10)
    {
      lcd.setCursor(18,3);
      lcd.print("0");
      lcd.setCursor(19,3);
      lcd.print(i);
      delay(1000);
    }

    if(i >= 10)
    {
    lcd.setCursor(18, 3);
    lcd.print(i);
    delay(1000);
    }
      
  }

  return;
}


/*
float read_pH()
{
  unsigned long int avgValue;  //Store the average value of the sensor feedback
  float b;
  int buf[10],temp;

    for(int i=0;i<10;i++)       //Get 10 sample value from the sensor for smooth the value
  { 
    buf[i]=analogRead(SensorPin);
    delay(10);
  }
  for(int i=0;i<9;i++)        //sort the analog from small to large
  {
    for(int j=i+1;j<10;j++)
    {
      if(buf[i]>buf[j])
      {
        temp=buf[i];
        buf[i]=buf[j];
        buf[j]=temp;
      }
    }
  }
  avgValue=0;
  for(int i=2;i<8;i++)                      //take the average value of 6 center sample
    avgValue+=buf[i];
  float phValue=(float)avgValue*3.33/4095/6; //convert the analog into millivolt
  phValue=3.5*phValue;                      //convert the millivolt into pH value
  
  return phValue;
  
}

*/


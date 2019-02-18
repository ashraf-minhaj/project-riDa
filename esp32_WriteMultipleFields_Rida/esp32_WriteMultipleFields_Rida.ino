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

#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#include "ThingSpeak.h"
#include "secrets.h"
#include <WiFi.h>

char ssid[] = SECRET_SSID;   // your network SSID (name) 
char pass[] = SECRET_PASS;   // your network password
int keyIndex = 0;            // your network key Index number (needed only for WEP)
WiFiClient  client;

LiquidCrystal_I2C lcd(0x27,20,4);  // set the LCD address to 0x27 f

unsigned long myChannelNumber = SECRET_CH_ID;
const char * myWriteAPIKey = SECRET_WRITE_APIKEY;

String myStatus = "";  //Temperatur / pH / Level indicator

int trig = 2;     //sonar - distance measurement
int echo = 5;

float pH = 6.7;   //pH initial value - since no pH sensor connected
int temp = 24;    //Temperature  value (sensor on pin 34)

int tur = 32;    //turbidity on pin32

int buz = 19;

void setup() 
{
  pinMode(trig,OUTPUT);
  pinMode(echo,INPUT);

  pinMode(buz, OUTPUT);
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
  temp = ((analogRead(34) * 500.0) / 4095.0) ;   //read Temperature


  // Connect or reconnect to WiFi
  if(WiFi.status() != WL_CONNECTED)   //if connection is lost or not connected
  {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(SECRET_SSID);
    while(WiFi.status() != WL_CONNECTED)
    {
      WiFi.begin(ssid, pass);  // Connect to WPA/WPA2 network. Change this line if using open or WEP network
      Serial.print(".");
      delay(5000);     
    } 
    Serial.println("\nConnected.");
    
  }

  //get the water level
  digitalWrite(trig,HIGH);
  delay(1);
  digitalWrite(trig,LOW);
  duration= pulseIn(echo,HIGH);
  dis= (duration/2)/29.1 ;      //distance to water from rida.
 
 
  Serial.println(temp);
  Serial.println(dis);
  Serial.println(tur);

  
  lcd.setCursor(0, 0);  //(column, row)
  lcd.print("project ** riDa **");
  lcd.setCursor(0, 1);
  lcd.print("Britannia University");
  lcd.setCursor(0, 2);
  lcd.print("ready for robo fest.");
  lcd.setCursor(0, 3);
  lcd.print("dev by Ashraf Minhaj");
  delay(2000);
  
  lcd.clear();
  lcd.setCursor(0, 0);  //(column, row)
  lcd.print("Tur");
  lcd.setCursor(4, 0);
  lcd.print(tur);
  
  lcd.setCursor(0, 1);
  lcd.print("Temp");
  lcd.setCursor(5, 1);
  lcd.print(temp);

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
  ThingSpeak.setField(3, temp);
  ThingSpeak.setField(4, pH);

  delay(1000);

  // figure out the status message
  if(dis < 30)
  {
    myStatus = String("**Warning!** WATER LEVEL IS HIGH."); 
  }
  else if(dis < 20)
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
  }

  pH += 0.10; //increment pH by 0.10 to show graph
  if(pH > 8.5)
  {
    pH = 6.7;
  }
  
  digitalWrite(19, LOW);
//  delay(20000); // Wait 20 seconds to update the channel again

  delay_def();
  
  
  lcd.clear();
}

void delay_def()
{
  int i = 0;
  for(i = 0; i <= 30; i++)
  {
    lcd.setCursor(18, 3);
    lcd.print(i);
    delay(1000);
      
  }

  return;
}


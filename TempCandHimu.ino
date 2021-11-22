#include <LiquidCrystal_I2C.h>
#include "DHT.h"
#define DHTPIN 2
#define DHTTYPE DHT11

LiquidCrystal_I2C lcd(0x3F, 16, 2);  // I2C address 0x27, 16 column and 2 rows
DHT dht(DHTPIN, DHTTYPE);

//
const int analogInPin = A0; // กำหนดขา input เซ็นเซอร์
const int Relay = 2; // กำหนดขา input รีเลย์
int sensorValue = 0; // ตัวแปรค่า Analog
int outputValue = 0; // ตัวแปรสำหรับ Map เพื่อคิด %
//
int ledPin5 = 5; // YES
int ledPin6 = 6; // NO

void setup()
{
  dht.begin();     // initialize the sensor
  lcd.init();      // initialize the lcd
  lcd.backlight(); // open the backlight 
  //
  Serial.begin(9600);
  pinMode(Relay, OUTPUT); // กำหนด รีเลย์เป็น Output
  //
  pinMode(ledPin5, OUTPUT); // sets the pin5 as output
  pinMode(ledPin6, OUTPUT); // sets the pin6 as output
}

void loop()
{
  //
  sensorValue = analogRead(analogInPin);
  outputValue = map(sensorValue, 0, 1023, 100, 0);
  //
  float humi  = dht.readHumidity();    // read humidity
  float tempC = dht.readTemperature(); // read temperature
  //

  Serial.print("Temp: ");
  Serial.print(tempC);
  Serial.println(" C");
  Serial.print("Soil Humi: ");
  Serial.print(outputValue);
  Serial.println(" %");
  Serial.println();
  
  //
  delay(2000); // wait a few seconds between measurements

  lcd.clear();
  // check if any reads failed
  if (isnan(humi) || isnan(tempC)) {
    lcd.setCursor(0, 0);
    lcd.print("Failed");
  } else {
    for (byte i =0;i <= 15; i++)
    {
    lcd.setCursor(15 - i, 0);  // start to print at the first row
    lcd.print("Temp: ");
    lcd.print(tempC);     // print the temperature
    lcd.print((char)223); // print ° character
    lcd.print("C   ");

    lcd.setCursor(15 - i, 1);  // start to print at the second row
    lcd.print("Soil Humi: ");
    lcd.print(outputValue);      // print the humidity
    lcd.print("%  ");
    delay(100);
    }
  }

  // check LED
  if (tempC < 30.00 && outputValue < 50 ){
    digitalWrite(ledPin5,HIGH);
    digitalWrite(ledPin6,LOW);
  }
  else{
    digitalWrite(ledPin5,LOW);
    digitalWrite(ledPin6,HIGH);
  }
  
}

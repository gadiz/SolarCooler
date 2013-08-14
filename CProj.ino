
#include <arduino.h>
#include <Thermister.h>
#include <TimerOne.h>
#include <LiquidCrystal.h>
#include <LCDKeypad.h>
#include "myLCD.h"

int led = 13;
#define RELAY1  2//Water tap  
#define RELAY2  3//Pump
long timerInterval = 100000;
// the setup routine runs once when you press reset:
void setup() {                
  // initialize the digital pin as an output.
  Serial.begin(9600);
  pinMode(led, OUTPUT);     
  pinMode(RELAY1, OUTPUT);
  digitalWrite(RELAY1,HIGH); // Close Relay
  pinMode(RELAY2, OUTPUT);
  digitalWrite(RELAY2,HIGH); // Close Relay
// So that we can sample temperature and print result every 5 seconds
  lcd.setup();
  Timer1.initialize(timerInterval); // set a timer of length 100000 microseconds (or 0.1 sec - or 10Hz => the led will blink 5 times, 5 cycles of on-and-off, per second)
  Timer1.attachInterrupt(sampleTemp); // attach the service routine here
  
}
int thermistorPin = A1;
float lastCels=0;
float thermReading = 0;

thermister therm(thermistorPin,thermister::T_CELSIUS,3977.0f , 298.15f , 10000.0f ,10000.0f);
// the loop routine runs over and over again forever:
  void openEvery(int time)
{
      digitalWrite(RELAY1,HIGH);
      delay(time);  
      digitalWrite(RELAY1,LOW);
      delay(time);
}

int sampleCount = 0;
long timerCount = 0;
int numOpens = 0;
int totalDuration = 0;
void sampleTemp() // This is called via a timer interupt every 5 seconds
{
  timerCount += timerInterval;// 1/10 of a second
  if (timerCount > (timerInterval * 50)){// Every 5 seconds
    thermReading = therm.Temperature();  
    digitalWrite(led, HIGH);
      //delay(1000); 
    //float tmpCel = therm.Temperature();
    Serial.print(thermReading);
    Serial.println(",");
    //digitalWrite(led, LOW);
    lcd.updateData(thermReading , numOpens , totalDuration);
    //lcd.printTemp(thermReading);
    timerCount = 0;
  }else{
    keyPadInstruct ki = lcd.keyPadInterrupt();
    if (ki == KUPDATE){
      lcd.updateData(thermReading , numOpens , totalDuration);
    }
    if (ki == KOPENMANUAL){
      OpenWater();
      delay(250000);
      CloseWater();
    }
  }
}
void CloseWater()
{
  digitalWrite(RELAY1,HIGH);// Close water tap
  delay(200);
  digitalWrite(RELAY2,HIGH);// Turn off Pump
  //Serial.println("Closing Relay");
}

void OpenWater()
{
  digitalWrite(RELAY2,LOW);// Turn on Pump
  delay(250);
  digitalWrite(RELAY1,LOW);// Open Water tap
  //Serial.println("Openning Relay");
}

#define OPEN_TIME 5000
#define MIN_PERIOD 120000  // two minute delay
int resetCount = 0;
void loop() { 
  if (resetCount > 10000){ // to gather daily statistics, assume a 14 hour pause from 18:00 to 08:00 at least. Intervals of 5 seconds
    numOpens = 0;
    totalDuration = 0;
    resetCount = 0;
  }
  CloseWater();
  if (thermReading > 39){
    OpenWater();
    numOpens++;
    int addopen = 0;
    if (thermReading > 41.0f)
      addopen = 1500;
    totalDuration = totalDuration + (OPEN_TIME + addopen)/1000;
    delay(OPEN_TIME + addopen); // Open for 8 seconds
    CloseWater();
    delay(MIN_PERIOD);// Wait 2.5 minutes at least
  }else{
    delay(5001);// wait 5 seconds before next temp check
    resetCount++;
  }
 }


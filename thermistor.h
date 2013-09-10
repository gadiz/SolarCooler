#ifndef __Thermister__
#define __Thermister__

#include <math.h>
// enumarating 3 major temperature scales

// manufacturer data for episco k164 10k thermistor
// simply delete this if you don't need it
// or use this idea to define your own thermistors
#define EPISCO_K164_10k 4300.0f,298.15f,10000.0f  // B,T0,R0

// Temperature function outputs float , the actual 
// temperature
// Temperature function inputs
// 1.AnalogInputNumber - analog input to read from 
// 2.OuputUnit - output in celsius, kelvin or fahrenheit
// 3.Thermistor B parameter - found in datasheet 
// 4.Manufacturer T0 parameter - found in datasheet (kelvin)
// 5. Manufacturer R0 parameter - found in datasheet (ohms)
// 6. Your balance resistor resistance in ohms  
class thermistor{
public:
	enum {
	  T_KELVIN=0,
	  T_CELSIUS,
	  T_FAHRENHEIT
	};

  thermistor(int AnalogInputNumber,int OutputUnit,float B,float T0,float R0,float R_Balance);
  float Temperature();
private:
	int m_analogInputNumber;
	int m_outputUnit;
	float m_Thermistor_B;
	float m_Thermistor_T0;
	float m_Thermistor_R0;
	float m_Thermistor_R_Balance;
};
// example of use #1
// reading from analog input 1, using episco k164 definition
// and 10k balance, getting result in celsius

/*void setup() {
 Serial.begin(9600);
}

void loop() {

 Serial.println("*************************");
 Serial.println("10k Balance");
 Serial.println(Temperature(1,T_CELSIUS,EPISCO_K164_10k,10000.0f));
 Serial.println("*************************");

 delay(500);
}
*/
//example of use #2
// using numbers instead of episco k164 definition
// this time reading from analog input 2
// getting result in fahrenheit

/*void setup() {
 Serial.begin(9600);
}

void loop() {

 Serial.println("*************************");
 Serial.println("10k Balance");
 Serial.println(Temperature(2,T_FAHRENHEIT,4300.0f,298.15f,10000.0f,10000.0f));
 Serial.println("*************************");

 delay(500);
}*/

#endif

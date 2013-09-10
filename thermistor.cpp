  #include "arduino.h"
  #include "thermistor.h"
//#define EPISCO_K164_10k 4300.0f,298.15f,10000.0f  // B,T0,R0

// B = 3977
// T0 = 298.15f
// R0 = 10000 ohm

// Temperature function outputs float , the actual 
// temperature
// Temperature function inputs
// 1.AnalogInputNumber - analog input to read from 
// 2.OuputUnit - output in celsius, kelvin or fahrenheit
// 3.Thermistor B parameter - found in datasheet 3977
// 4.Manufacturer T0 parameter - found in datasheet (kelvin)
// 5. Manufacturer R0 parameter - found in datasheet (ohms)
// 6. Your balance resistor resistance in ohms   
  thermistor::thermistor(int AnalogInputNumber,int OutputUnit ,float B,float T0,float R0,float R_Balance)
  {
	m_outputUnit = OutputUnit;
	m_analogInputNumber = AnalogInputNumber;
	m_Thermistor_B = B;
	m_Thermistor_T0 = T0;
	m_Thermistor_R0 = R0;
	m_Thermistor_R_Balance = R_Balance;
  }
  
  float thermistor::Temperature()
  {
    float R,T;
    R=1024.0f*m_Thermistor_R_Balance/(1024.0f-float(analogRead(m_analogInputNumber)))-m_Thermistor_R_Balance;
    T=1.0f/(1.0f/m_Thermistor_T0+(1.0f/m_Thermistor_B)*log(R/m_Thermistor_R0));
    switch(m_outputUnit) {
      case T_CELSIUS :
        T-=273.15f;
      break;
      case T_FAHRENHEIT :
        T=9.0f*(T-273.15f)/5.0f+32.0f;
      break;
      default:
      break;
    };
  
    return T;
  }

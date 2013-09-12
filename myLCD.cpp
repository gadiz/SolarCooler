  #include "arduino.h"
  #include "MyLCD.h"
  myLCD lcd;
  /*#define KEYPAD_NONE -1
#define KEYPAD_RIGHT 0
#define KEYPAD_UP 1
#define KEYPAD_DOWN 2
#define KEYPAD_LEFT 3
#define KEYPAD_SELECT 4
*/
//#include <LCD4Bit_mod.h> 
//create object to control an LCD.  
//number of lines in display=1
//LCD4Bit_mod lcd = LCD4Bit_mod(2);
//LiquidCrystal lcd(8, 11, 9, 4, 5, 6,7);
// extern LCDKeypad lcd;//(8, 11, 9, 4, 5, 6,7);
byte degree[8] = {
  B00110,
  B01001,
  B01001,
  B00110,
  B00000,
  B00000,
  B00000,
};

myLCD::myLCD()
{
  m_Mode = MTEMP;
  digitalWrite(10,LOW);
  keyDown = false;
}

void myLCD::setup()
{
  // set up the LCD's number of columns and rows: 
  m_lcd.begin(16, 2);
  // Print a message to the LCD.
  m_lcd.print("PVCooler 1.0");
  m_lcd.setCursor(0,2);
  m_lcd.print("Processing...");
  m_lcd.createChar(0, degree);
  delay(2000);  
 // int backLight = 13;    // pin 13 will control the backlight
 //  pinMode(backLight, OUTPUT);
 // digitalWrite(backLight, LOW); // turn backlight on. Replace 'HIGH' with 'LOW' to turn it off.

}
void myLCD::updateData(float thermReading , int numOpens , int totalDuration , int ldr)
{
  switch (m_Mode){
    case MTEMP:
       printTemp(thermReading);
      break;
    case MNUMOPENS:
       printNumOpens(numOpens);
      break;
    case MDURATION:
      printTotalDuration(totalDuration);
      break;
      case MLDR:
      printLDR(ldr);
      break;
    default:
      break;
  }
 
}

keyPadInstruct myLCD::keyPadInterrupt()// Called every 0.1 seconds , return true if immediate update required
{
  if (keyDown)
    return KNONE;
  keyDown = true;
  HandleBackLight();
  keyPadInstruct rc = KNONE;
  int md = m_Mode;
  int kbutton = m_lcd.button();
  switch (kbutton){
    case KEYPAD_NONE:
      break;
    case KEYPAD_SELECT:
     md = (md +1) % 4;
     m_Mode = (mode)md;
     rc = KUPDATE;
      break;
      case KEYPAD_LEFT:
      //backLightOff();
      break;
      case KEYPAD_RIGHT:
      rc = KOPENMANUAL;
      break;
    default:
      break;
  }
  waitReleaseButton();
  keyDown = false;
  return rc;
}
int onCount = 0;
bool offCount = true;
void myLCD::HandleBackLight()
{
  if (onCount > 20000){
      backLightOff();
      offCount = false;
  }
  if (offCount){
    onCount += 100;
  }
  int kbutton = m_lcd.button();
  if (kbutton == KEYPAD_SELECT || kbutton == KEYPAD_LEFT || kbutton == KEYPAD_RIGHT || kbutton == KEYPAD_UP || kbutton == KEYPAD_DOWN){
     backLightOn();
      onCount = 0;
      offCount = true;
  }
}
void myLCD::wait(unsigned long milSec) //  do this to avoid calling dalay during timer callback
{
  unsigned long milBegin = millis();
  unsigned long mil = milBegin;
  do{
    mil = millis();
  }while (mil - milBegin < milSec);
}
int myLCD::waitButton()
{
  int buttonPressed; 
  waitReleaseButton();
  m_lcd.blink();
  while((buttonPressed=m_lcd.button())==KEYPAD_NONE)
  {
  }
  delay(50);  
  m_lcd.noBlink();
  m_lcd.setCursor(0,1);
  m_lcd.write("Pressed");//buttonPressed);//==KEYPAD_SELECT?1:' ');
  return buttonPressed;
}

void myLCD::waitReleaseButton()
{
  delay(50);
  while(m_lcd.button()!=KEYPAD_NONE)
  {
  }
  delay(50);
}

void myLCD::backLightOff()
{
  pinMode(10,OUTPUT);// Switch backlight off
}
void myLCD::backLightOn()
{
  pinMode(10,INPUT);// Switch backlight off
}

char *ftoa(char *a, double f, int precision)
{
  long p[] = {0,10,100,1000,10000,100000,1000000,10000000,100000000};
  
  char *ret = a;
  long heiltal = (long)f;
  itoa(heiltal, a, 10);
  while (*a != '\0') a++;
  *a++ = '.';
  long desimal = abs((long)((f - heiltal) * p[precision]));
  itoa(desimal, a, 10);
  return ret;
}

void myLCD::printTemp(float thermReading)
{
  clearLine(2);
  char buff[20];
  m_lcd.print("Temp = ");
  m_lcd.print(ftoa(buff, thermReading, 2));
  m_lcd.write((byte)0);
  m_lcd.print("C");
}
 void myLCD::clearLine(int ln)
 {
    m_lcd.setCursor(0,ln);
    m_lcd.print("                ");
    m_lcd.setCursor(0,ln);
 }
 void myLCD::printNumOpens(int numOpens)
 {
   clearLine(2);
   m_lcd.print("Num Opens = ");
   m_lcd.print(numOpens);
 }
 void myLCD::printLDR(int ldr)
 {
    clearLine(2);
   m_lcd.print("LDR = ");
   m_lcd.print(ldr);
 }
 void myLCD::printTotalDuration(int totalDuration)
 {
   clearLine(2);
   m_lcd.print("Duration = ");
   m_lcd.print(totalDuration);
   m_lcd.print(" Sec");
 }

  

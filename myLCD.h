
  #include <arduino.h>
  #include <LiquidCrystal.h>
  #include <LCDKeypad.h>
  enum mode{MTEMP =0, MNUMOPENS = 1,MDURATION = 2}; 
  enum keyPadInstruct{KNONE = 0, KUPDATE =1 , KOPENMANUAL =2};
  class myLCD
  {
   public:
    myLCD();
    void setup();
    void backLightOff();
    void backLightOn();
     void printTemp(float thermReading);
     void printNumOpens(int numOpens);
     void printTotalDuration(int totalDuration);
     int waitButton();
     void waitReleaseButton();
     // void keyPadHandle();
     keyPadInstruct keyPadInterrupt();
     void updateData(float thermReading , int numOpens , int totalDuration);
   // blink()
   private:
   void clearLine(int ln);
   void HandleBackLight();
   LCDKeypad m_lcd;
    
     mode m_Mode;
     bool keyDown;
   
  };
  extern myLCD lcd;

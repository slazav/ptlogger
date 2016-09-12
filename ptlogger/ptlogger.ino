#define USE_DISP 1
#define USE_CARD 1
#define USE_PRES 1
#define USE_TEMP 1
#define USE_RTC 1
#define NORMAL_MODE  1

#define BTNA 9
#define BTNB 6
#define BTNC 5
#define GLED  8
#define YLED  13
#define CARD  7
#define CARD_CS  4
#define OLED_RESET 4

#define ADDR_OLED 0x3C
#define ADDR_RTC    0x68
#define ADDR_PRES  0x77
#define ADDR_TEMP 0x18

#define MAXDISPCOUNT 60

#include <Arduino.h>

// include headers and define variables for all components:

// cf card:
#if USE_CARD
#include <SPI.h>
#include <SD.h>
File dataFile;
bool cfmode = false;
#endif

// display:
#if USE_DISP
#include "Adafruit_GFXm.h"
#include "Adafruit_SSD1306m.h"
Adafruit_SSD1306 display(OLED_RESET);
int16_t  dispcount=MAXDISPCOUNT; // counter for display-on state
#endif

// output functions for both display and cf:
void ppf(const char *txt, float f, uint8_t w){
#if USE_DISP
  display.print(txt);
  display.print(f,w);
#endif
#if USE_CARD
  if (dataFile){
     dataFile.print(" ");
     dataFile.print(f,w);
  }
#endif
}
void ppi(int f, uint8_t w, const char *txt){
  for (; w>0; w--){
    #if USE_DISP
    display.print( ( f/int(pow(10, w-1)))%10 );
    #endif
    #if USE_CARD
    dataFile.print( ( f/int(pow(10, w-1)))%10 );
    #endif
  }
  #if USE_DISP
  display.print(txt);
  #endif
  #if USE_CARD
  dataFile.print(txt);
  #endif
}

// thermometer:
#if USE_TEMP
#include <Wire.h>
#endif

// clock:
#if USE_RTC
#include <Wire.h>
static uint8_t bcd2bin (uint8_t val) { return val - 6 * (val >> 4); }
static uint8_t bin2bcd (uint8_t val) { return val + 6 * (val / 10); }
#endif

// pressure sensor:
#if USE_PRES
#include <Adafruit_BMP085_U.h>
Adafruit_BMP085_Unified PRESS;
float p0=101300;
#endif

/**********************************************/
// setup function
/**********************************************/
void setup()   {
  pinMode(BTNA, INPUT_PULLUP);
  pinMode(BTNB, INPUT_PULLUP);
  pinMode(BTNC, INPUT_PULLUP);
  pinMode(CARD, INPUT_PULLUP);
  pinMode(GLED, OUTPUT);
  pinMode(YLED, OUTPUT);

#if USE_RTC
  Wire.begin();
#endif

#if USE_TEMP
  Wire.begin();
   Wire.beginTransmission(ADDR_TEMP);
   Wire.write((byte)0x08);
   Wire.write((byte)0x03); // resolution 0.0625
   Wire.endTransmission();
#endif

#if USE_PRES
  PRESS.begin(BMP085_MODE_ULTRAHIGHRES);
//  PRESS.begin(BMP085_MODE_ULTRALOWPOWER);
#endif

#if USE_DISP
  display.begin(SSD1306_SWITCHCAPVCC, ADDR_OLED);
  display.display(); // show logo
#endif

#if USE_CARD
#endif
}

/**********************************************/
// loop function
/**********************************************/

#if NORMAL_MODE
void loop() {
  delay(1000);

  #if USE_DISP
    if (digitalRead(BTNB)==LOW) {
      if (dispcount)  dispcount=0;
      else dispcount = MAXDISPCOUNT;
    }
    if (dispcount) dispcount--;
  #endif

  #if USE_CARD
    if (digitalRead(BTNA)==LOW)   cfmode = !cfmode;
    if (cfmode)  {
       SD.begin(CARD_CS);
       dataFile = SD.open("data.txt", FILE_WRITE);
    }
   #endif

#if USE_DISP
  display.clearDisplay();
  display.setCursor(0,0);
#endif

 
  // get all information
  //time
  #if USE_RTC
  Wire.beginTransmission(ADDR_RTC);
  Wire.write((byte)0);
  Wire.endTransmission();
  Wire.requestFrom(ADDR_RTC, 7);
  uint8_t ss = bcd2bin(Wire.read() & 0x7F);
  uint8_t mm = bcd2bin(Wire.read());
  uint8_t hh = bcd2bin(Wire.read());
  Wire.read();
  uint8_t d = bcd2bin(Wire.read());
  uint8_t m = bcd2bin(Wire.read());
  uint16_t y = bcd2bin(Wire.read()) + 2000;
  // print
  ppi(y,4,"/");
  ppi(m,2,"/");
  ppi(d,2," ");
  ppi(hh,2,":");
  ppi(mm,2,":");
  ppi(ss,2,"");
    #if USE_DISP
    display.println();
    #endif
  #endif

  // pressure + temperature + altitude
  #define NAVRGS 20
  int i;
  float tmp;
  #if USE_PRES
  float p=0,  h, tp=0;  
  for (i=0; i<NAVRGS; i++){
    PRESS.getPressure(&tmp);
    p+=tmp;
    PRESS.getTemperature(&tmp);
    tp+=tmp;
  }
  p/=NAVRGS;
  tp/=NAVRGS;
     
  //PRESS.getPressure(&p);
  //PRESS.getTemperature(&tp);
  if (digitalRead(BTNC)==LOW)  p0=p;
  h = 44330.0 * (1.0 - pow(p /p0 , 0.1903));
  // print
  ppf("P ", p, 0);
  ppf("  H ", h, 2);
    #if USE_DISP
    display.println();
    #endif
  ppf("Tp ", tp, 2);
    #if USE_DISP
    display.println();
    #endif
  #endif

  //temperature (read register 0x05 from address ADDR_TEMP)
  #if USE_TEMP
  float tt=0;
  for (i=0; i<NAVRGS; i++){
    Wire.beginTransmission(ADDR_TEMP);
    Wire.write((uint8_t) 0x05);
    Wire.endTransmission();
    Wire.requestFrom((uint8_t)ADDR_TEMP, (uint8_t)2);
    int16_t tti = Wire.read();
    tti <<= 8;
    tti|= Wire.read();
    // convert to float
    tmp= float(tti & 0x0FFF)/16.0;
    if (tti & 0x1000) tmp-= 256;
    tt+=tmp;
  }
  tt/=NAVRGS;
  ppf(" ", tt, 2);
  #endif

#if USE_CARD
  if (dataFile) {
    #if USE_DISP
      display.setCursor(64,16);
      display.print("CF");
    #endif
    dataFile.println(); 
    dataFile.close();
  }
#endif

#if USE_DISP
  if (dispcount==0) display.clearDisplay();
  display.display();
#endif
}



#else
  // set time (use predefined values, set on button A press)
  void loop() {
   delay(1);
   if (digitalRead(BTNA)==LOW){

   uint8_t ss = 0;
   uint8_t mm = 36;
   uint8_t hh = 23;
   uint8_t d = 4;
   uint8_t m = 9;
   uint16_t y = 16;

    Wire.beginTransmission(ADDR_RTC);
    Wire.write((byte)0); // start at location 0
    Wire.write(bin2bcd(ss));
    Wire.write(bin2bcd(mm));
    Wire.write(bin2bcd(hh));
    Wire.write(bin2bcd(0));
    Wire.write(bin2bcd(d));
    Wire.write(bin2bcd(m));
    Wire.write(bin2bcd(y));
    Wire.endTransmission();

    // read i2c register
    Wire.beginTransmission(ADDR_RTC);
    Wire.write((byte)0xFF);
    Wire.endTransmission();
    Wire.requestFrom(ADDR_RTC, (byte)1);
    uint8_t statreg =  Wire.read() & ~0x80; // flip OSF bit

    // write i2c register
    Wire.beginTransmission(ADDR_RTC);
   Wire.write((byte)0xFF);
   Wire.write((byte)statreg);
   Wire.endTransmission();

      digitalWrite(YLED, HIGH);
      delay(200);
      digitalWrite(YLED, LOW);
   }
}
#endif


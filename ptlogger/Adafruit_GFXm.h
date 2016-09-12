#ifndef _ADAFRUIT_GFXM_H
#define _ADAFRUIT_GFXM_H

#include "Arduino.h"
#include "Print.h"

class Adafruit_GFXm : public Print {

 public:

  Adafruit_GFXm(int16_t w, int16_t h); // Constructor

  // This MUST be defined by the subclass:
  virtual void drawPixel(int16_t x, int16_t y, uint16_t color) = 0;

  void drawChar(int16_t x, int16_t y, unsigned char c);
  void setCursor(int16_t x, int16_t y);

  virtual size_t write(uint8_t);
  int16_t height(void) const;
  int16_t width(void) const;

  // get current cursor position (get rotation safe maximum values, using: width() for x, height() for y)
  int16_t getCursorX(void) const;
  int16_t getCursorY(void) const;

 protected:
  const int16_t
    WIDTH, HEIGHT;   // This is the 'raw' display w/h - never changes
  int16_t
    _width, _height, // Display w/h as modified by current rotation
    cursor_x, cursor_y;
};


#endif // _Adafruit_GFXmM_H

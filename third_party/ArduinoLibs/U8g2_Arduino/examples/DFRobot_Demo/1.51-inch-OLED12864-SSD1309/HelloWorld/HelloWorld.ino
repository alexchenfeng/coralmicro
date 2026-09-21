/*!
 * @file HelloWorld.ino
 * @brief 简单又经典的Hello World!显示
 * @n U8G2支持多种大小的字体，此demo只是选取1种字体大小进行显示"Hello World!"
 * @n U8G2字体GitHub连接：https://github.com/olikraus/u8g2/wiki/fntlistall
 * 
 * @copyright  Copyright (c) 2010 DFRobot Co.Ltd (http://www.dfrobot.com)
 * @licence     The MIT License (MIT)
 * @author [Ivey](Ivey.lu@dfrobot.com)
 * @maintainer [Fary](feng.yang@dfrobot.com)
 * @version  V1.0
 * @date  2019-10-15
 * @url https://github.com/DFRobot/U8g2_Arduino
*/
#include <Arduino.h>
#include <U8g2lib.h>

#include <SPI.h>

/*
 * Display hardware IIC interface constructor
 *@param rotation：U8G2_R0 Not rotate, horizontally, draw direction from left to right
           U8G2_R1 Rotate clockwise 90 degrees, drawing direction from top to bottom
           U8G2_R2 Rotate 180 degrees clockwise, drawing in right-to-left directions
           U8G2_R3 Rotate clockwise 270 degrees, drawing direction from bottom to top
           U8G2_MIRROR Normal display of mirror content (v2.6.x version used above)
           Note: U8G2_MIRROR need to be used with setFlipMode().
 *@param reset：U8x8_PIN_NONE Indicates that the pin is empty and no reset pin is used
 * Display hardware SPI interface constructor
 *@param  Just connect the CS pin (pins are optional)
 *@param  Just connect the DC pin (pins are optional)
 *
*/
#if defined ARDUINO_SAM_ZERO
#define OLED_DC  7
#define OLED_CS  5
#define OLED_RST 6
/*ESP32 */
#elif defined(ESP32)
#define OLED_DC  D2
#define OLED_CS  D6
#define OLED_RST D3
/*ESP8266*/
#elif defined(ESP8266)
#define OLED_DC  D4
#define OLED_CS  D6
#define OLED_RST D5
/*AVR series board*/
#else
#define OLED_DC  2
#define OLED_CS  3
#define OLED_RST 4
#endif
U8G2_SSD1309_128X64_NONAME2_1_4W_HW_SPI u8g2(/* rotation=*/U8G2_R0, /* cs=*/ OLED_CS, /* dc=*/ OLED_DC,/* reset=*/OLED_RST);




void setup(void) {
  u8g2.begin();
  u8g2.setFontPosTop();
}

void loop(void) {
  u8g2.firstPage();   
  do
  {
    u8g2.clearBuffer();          // clear the internal memory
    u8g2.setFont(u8g2_font_t0_17b_tr  );  // choose a suitable font
    u8g2.drawStr(0,5,"Hello World!");  // write something to the internal memory
    u8g2.sendBuffer();          // transfer internal memory to the display
  } while( u8g2.nextPage() );
  delay(2000);
}
/*!
 * @file Language.ino
 * @brief Display multiple languages in U8G2
 * @n A demo for displaying “hello world！” in Chinese, English, Japanese
 * @n U8G2 Font GitHub Link：https://github.com/olikraus/u8g2/wiki/fntlistall
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
  u8g2.begin();    //init
  u8g2.enableUTF8Print();		// Enable UTF8 support for Arduino print（）function.
}

void loop(void) 
{
  /*@brief Set font direction  of all strings setFontDirection(uint8_t dir)
   *@param dir=0，rotate 0 degree
                 dir=1，rotate 90 degrees
                 dir=2，rotate 180 degrees
                 dir=3，rotate 270 degrees
   *@param When completed font setting, re-set the cursor position to display normally. Refer to API description for more details.
  */
  u8g2.setFontDirection(0); 
  /*
   * firstPage Change the current page number position to 0 
   * Revise content in firstPage and nextPage, re-render everything every time
   * This method consumes less ram space than sendBuffer
  */
  u8g2.firstPage();
  do {
    /*
     *The font takes up a lot of memory, so please use it with caution. Get your own Chinese encode for displaying only several fixed words.
     *Display by drawXBM or use controller with larger memory
     *Chinese Font：require a controller with larger memory than Leonardo  
     *Japanese Font：require a controller with larger memory than UNO
     *Korean Font：Arduino INO files of the current version do not support for displaying Korean, but it can displayed properly on the Screen
    */
    u8g2.setFont(u8g2_font_HelvetiPixelOutline_tr);
    u8g2.setCursor(/* x=*/0, /* y=*/15);    //Define the cursor of print function, any output of the print function will start at this position.
    u8g2.print("Hello World!");
    u8g2.setFont(u8g2_font_unifont_t_chinese2);
    u8g2.setCursor(0, 35);
    u8g2.print("你好世界");		// Chinese "Hello World" 
    u8g2.setFont(u8g2_font_b10_t_japanese1);
    u8g2.setCursor(0, 50);
    u8g2.print("こんにちは世界");
  } while ( u8g2.nextPage() );
  delay(1000);
}
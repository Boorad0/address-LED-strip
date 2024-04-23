#include "FastLED.h" 

#define NUM_LEDS 50
#define PIN 13
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27,16,2);
byte bright = 50; // яркость светодиодов
byte rate = 0;       // скорость эффектов
byte w = 1;          // переключение режимов
byte baza = 0;     // изменение оттенка

boolean button1WasUp = true;   // переменные для считывания нажатия на кнопки


CRGB leds[NUM_LEDS];

void setup() {
  lcd.init();                     
  lcd.backlight();
  pinMode(A3, INPUT_PULLUP);   // подключаем тактовые кнопки
  FastLED.addLeds <WS2812, PIN, GRB>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.setBrightness(bright);
}

void knopka () {
  boolean button1IsUp = digitalRead(A3);   // узнаем текущее состояние кнопок
 

  // если кнопка 1 была нажата, добавляем яркость (не больше 250)
  
  if (button1WasUp && !button1IsUp) {
    delay(10);
    button1IsUp = digitalRead(A3);
    if (!button1IsUp) { w++; if (w > 7) { w = 1; }
    }
  }
  button1WasUp = button1IsUp; // запоминаем состояние кнопки 3
}

void loop() {
  // первый режим (радуга)
  while (w == 1) {
    knopka(); // узнаем состояние кнопок
    lcd.setCursor(0, 1);
    lcd.print("Rainbow");
    for (int i = 0; i < NUM_LEDS; i++) {
      leds[i] = CHSV(baza+ i * 5, 255, 255);
    }
    baza++;
    FastLED.setBrightness(bright);
    FastLED.show();
    delay(50 - rate);
  }

  // второй режим (палитра)
  while (w == 2) {
    knopka(); // узнаем состояние кнопок
    lcd.setCursor(0, 1);
    lcd.print("Palitra");
    static uint8_t starthue = 0;
    fill_rainbow( leds + 5, NUM_LEDS - 5, --starthue, rate);
    uint8_t secs = (millis() / 1000) % (rate * 2);
    if (secs < rate) { FastLED.setTemperature( Tungsten100W ); leds[0] = Tungsten100W; }
    else { FastLED.setTemperature( OvercastSky ); leds[0] = OvercastSky; }
    FastLED.setBrightness(bright);
    FastLED.show();
  }

  // третий режим (конфетти)
  while (w == 3) {
    knopka(); // узнаем состояние кнопок
    lcd.setCursor(0, 1);
    lcd.print("Konfeti");
    fadeToBlackBy(leds, NUM_LEDS, (rate + 1) * 2);
    int pos = random16(NUM_LEDS);
    leds[pos] += CHSV(baza++ + random8(64), 200, 255);
    FastLED.setBrightness(bright);
    FastLED.show();
  }

  // четвертый режим (бегущий огонек)
  while (w == 4) {
    knopka(); // узнаем состояние кнопок
    lcd.setCursor(0, 1);
    lcd.print("Begushiy ogonek");
    fadeToBlackBy(leds, NUM_LEDS, (rate + 1) * 2);
    int pos = beatsin16(13, 0, NUM_LEDS - 1);
    leds[pos] += CHSV(baza++, 255, 192);
    FastLED.setBrightness(bright);
    FastLED.show();
  }

  // пятый режим (циклон)
  while (w == 5) {
    for (int i = 0; i < NUM_LEDS; i++) {
      leds[i].nscale8(250);
      }
    for (int i = 0; i < NUM_LEDS; i++) {
      knopka(); // узнаем состояние кнопок
      lcd.setCursor(0, 1);
    lcd.print("chiklon");
      leds[i] = CHSV(baza++, 255, 255);
      FastLED.setBrightness(bright);
      FastLED.show();
      delay(rate);
    }
  }

  // шестой режим (фокус)
  while (w == 6) {
    knopka(); // узнаем состояние кнопок
    lcd.setCursor(0, 1);
    lcd.print("fokus");
    fadeToBlackBy(leds, NUM_LEDS, (rate + 1) * 2);
    for (int i = 0; i < 8; i++) {
      leds[beatsin16(i + 7, 0, NUM_LEDS - 1)] |= CHSV(baza+=16, 200, 255);
    }
    FastLED.setBrightness(bright);
    FastLED.show();
    delay(10);
  }

  // седьмой режим (радуга с мерцанием)
  while (w == 7) {
    knopka(); // узнаем состояние кнопок
    fill_rainbow( leds, NUM_LEDS, baza++, 7);
    if (random8() < 80) { leds[ random16(NUM_LEDS) ] += CRGB::White; }
    FastLED.setBrightness(bright);
    FastLED.show();
    delay(50 - rate);
  }
}

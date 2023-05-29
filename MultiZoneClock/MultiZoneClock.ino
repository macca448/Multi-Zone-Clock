/*
  Copyright (C) 2023  R D McCLEERY    29th May 2023

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU Affero General Public License as published
  by the Free Software Foundation, either version 3 of the License, or
  any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU Affero General Public License for more details.

  You should have received a copy of the GNU Affero General Public License
  along with this program.  If not, see <https://www.gnu.org/licenses/>.

  Contacts:
  Email   : macca448@gmail.com
  Facebook: https://www.facebook.com/macca448
  GitHub  : https://github.com/macca448
  Pastebin: https://pastebin.com/u/macca-nz

  
  SKETCH FUNCTIONS:
  Threaded sketch with All Display functions on core1 and all time conversions on core0
  System time is set to UTC/GMT with the Arduino "TimeLib" library <https://github.com/PaulStoffregen/Time>
  Time conversions and strings are done using "tmelements"
  SSD1309 SPI Display using the "ThingsPulse" driver and graphics library  <https://github.com/ThingPulse/esp8266-oled-ssd1306>
  NOTE: You can drive the SSD1309 with the SSD1306 display driver

  The three clocks are UTC/GMT (Zulu), USA Eastern,  Austrailian Eastern
  
*/

#define RAN_MIN    13              // Random Minimum for NTP Update timing
#define RAN_MAX    27              // Random Maximum for NTP Update timing
#define SCREEN_OFF 4               // Screen Timeout in Minutes

#include <WiFi.h> 
#include <TimeLib.h>
#include <SPI.h>
#include "SSD1306Spi.h"
#include "OLEDDisplayUi.h"
#include "images.h"
#define WAKE  0                     // This is the BOOT/FLASH button on a Dev Board
#define RES   27
#define DC    4
#define CS    5
#define FPS   60
SSD1306Spi display(RES, DC, CS);
OLEDDisplayUi ui ( &display );

#define ON  true
#define OFF false

const char* ssid     = "MillFlat_El_Rancho";
const char* password = "140824500925";
int screenW = 128;
int screenH = 64;
int clockCenterX = screenW / 2;
int clockCenterY = ((screenH - 16) / 2) + 16; 
int clockRadius = 23;

uint8_t screenTimeOut = 0;
bool press = OFF, btnState, lastBtnState = ON, screen = ON;
uint32_t previousTime = 0;

struct ZONES{
  char utcWday[12];
  char utcDate[10];
  char aueWday[12];
  char aueDate[10];
  int aueHour;
  char useWday[12];
  char useDate[10];
  int useHour;
  uint8_t utcLastHour;
  uint8_t utcLastMin;
  uint8_t minCounter;
  uint8_t randomMin;
  long epoch;
  int32_t lastEpoch = 1609459200;       // 1st Jan 2021 00:00:00. Will change every update.
  bool update;
}zones;

#include "screens.h"
#include "conversions.h"

bool ntpUpdate(void){
  time_t now = time(nullptr);
  now = time(NULL);
  int32_t epoch = 0;
  uint8_t timeout = 0;
    while(epoch < zones.lastEpoch){
        epoch = time(&now);
        timeout++;
        delay(500);
          if(timeout >= 60){
            return false;
          }
    }
  setTime(epoch);                                     //Sets UTC time Globably with TimeLib
  zones.lastEpoch = epoch;
  zones.randomMin = random(RAN_MIN, RAN_MAX);
  zones.utcLastHour = hour();
  zones.utcLastMin = minute();
  delay(500);
  return true;
}

void loop2(void *pvParameters){    // Core 1 loop - User tasks - Display
  while (1){
    int remainingTimeBudget;
    if(screen){  
      remainingTimeBudget = ui.update();
    }else{
      remainingTimeBudget = 1;
    }
    if (remainingTimeBudget > 0) {
      btnState = digitalRead(WAKE);

      if(btnState != lastBtnState){
        press = ON;
        previousTime = millis();
      }
      
      if(millis() - previousTime >= 50){
        if(press){
          if(btnState == 0){
            if(!screen){
              screen = ON;
              screenTimeOut = 0;
              display.resetDisplay();
              display.displayOn();
              ui.enableAutoTransition();
              ui.setAutoTransitionForwards();
            }else{
              screen = OFF;
              display.clear();
              display.displayOff();
            }
          }
          press = OFF;
        }
      }
      if(screen){
        delay(remainingTimeBudget);
      }
      lastBtnState = btnState;
    }
  }
}

void loop1(void *pvParameters){    // Core 0 loop - User tasks - Time
  while (1){      
    
    if(hour() != zones.utcLastHour){
      zones.update = true;
      zones.utcLastHour = hour();
    }
    
    if(zones.update){
      zones.epoch = now();
      getUtcWday(zones.utcWday);
      getUtcDate(zones.utcDate);
      getAueWday(zones.aueWday);
      getAueDate(zones.aueDate);
      zones.aueHour = getAueHour();
      getUseWday(zones.useWday);
      getUseDate(zones.useDate);
      zones.useHour = getUseHour();
      zones.update = false;
    }
    
    if(minute() != zones.utcLastMin){
      zones.minCounter++;
      screenTimeOut++;
      zones.utcLastMin = minute();
        if(zones.minCounter >= zones.randomMin){
          zones.minCounter = 0;
          bool success = ntpUpdate();
            if(!success){
              ESP.restart();
            }
        }
        if(screenTimeOut >= SCREEN_OFF){
          screen = OFF;
          display.clear();
          display.displayOff();
        }
    }
    delay(1);
  }
}

void setup() {
  randomSeed(analogRead(A0));
  pinMode(WAKE, INPUT_PULLUP);
  ui.setTargetFPS(FPS);
  ui.setActiveSymbol(activeSymbol);
  ui.setInactiveSymbol(inactiveSymbol);
  ui.setIndicatorPosition(TOP);
  ui.setIndicatorDirection(LEFT_RIGHT);
  ui.setFrameAnimation(SLIDE_LEFT);
  ui.setFrames(frames, frameCount);
  ui.setOverlays(overlays, overlaysCount);
  //ui.setTimePerFrame(5000);					//If you want to change from default
  //ui.setTimePerTransition(500);				//If you want to change from default
  ui.init();
  display.flipScreenVertically();
  configTime(0, 0, "pool.ntp.org", "time.nist.gov");
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  WiFi.setAutoReconnect(true);

  bool success = ntpUpdate();
    if(!success){
        ESP.restart();
    }
  
  xTaskCreatePinnedToCore(loop2, "loop2", 4096, NULL, 1, NULL, 1);
  xTaskCreatePinnedToCore(loop1, "loop1", 4096, NULL, 1, NULL, 0);
  
  zones.update = true;
}

void loop() {}

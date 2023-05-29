# Multi-Zone-Clock

### Hardware List
  * ESP32 Devkit V1 (ESP-WROOM-32)
  * SSD1309 SPI 2.4inch OLED

### Libraries
  * WiFi (ESP board profile)
  * SPI (ESP board profile)
  * [TimeLib](https://github.com/PaulStoffregen/Time)
  * [ThingPulse ESP OLED](https://github.com/ThingPulse/esp8266-oled-ssd1306) (for SSD1306 & SH1106 I2C and SPI displays)

### How the Sketch Works

The sketch task's have been `threaded` with the `display` tasks on `core1` and all the time conversions on `core0`

Once connected to the WiFi network we use the standard `time` tools to get the current UTC `UNIX Time`. 

This `epoch` value is used to set the `TimeLib` library to `UTC/GMT` time so we now have `time` variables globably.

Last task in setup is to run some conversion class's to establish our three time zones. The standard `time` tools are used for this also. 

Because the three `timezones` have the same `minute` and `second` we only need to convert `hours` and `day` and this task is set to run on every hour roll.

`NTP Org's` Terms of Service suggest randomizing `NTP updates`. We have done this using a `random` minute value. It's set quite small for testing but should be fine set to say `RAN_MIN 64` and `RAN_MAX 73` or maybe even longer.

There are `Seven (7)` display screens. The first is all three `times` prefixed with `Zone & Day`. The next page is a `UTC` Analog Clock then `UTC` as a Digital Clock. This repeats for the other two `timezones` then the loop starts over.

I have added the ability to change the page `display time` and `trasition time` if the default doesn't suit.

The `framerate` is backed off to `30fps` so we can have a `WAKE` button.

The `display` timesout and sleeps after `4 minutes`. Make sure you set the right button.

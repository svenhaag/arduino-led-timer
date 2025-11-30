# Arduino LED Timer

Visually displaying kids the reaming time before we have to leave the house is the goal of this project.

A simple RGB LED strip is used for displaying the _timeline_. It starts with a fully lit strip, where the top LEDs are green, the middle LEDs are yellow and the bottom LEDs are red. As time passes the LEDs will go off from top to bottom. Finally, when time is up, the whole strip flashes red for some time.

The boards wifi capabilities are used to retrieve the current time from a time server (via NTP). This time is then provided to the boards real-time clock (RTC). The time server is queried regularly to update the boards RTC.

The on-board [LED matrix](https://docs.arduino.cc/tutorials/uno-r4-wifi/led-matrix/) is used to display the current time (hours and minutes).

# Required Equipment

* [Arduino Uni R4 WIFI](https://store.arduino.cc/products/uno-r4-wifi)
* [WS2812B RGB LED Strip](https://www.amazon.de/dp/B0BTV9TD36) - 1m 144 LEDs in this case
* Power Supply DC 5V ~8A, e.g. Mean Well LPV-60-5
* [Jump wires](https://en.wikipedia.org/wiki/Jump_wire) to connect Arduino to LED strip
* [Arduino IDE](https://www.arduino.cc/en/software/) for programming

# Third Party Software

Inspirations for this project were found on the website from [Hartmut Waller](https://hartmut-waller.info/arduinoblog/arduino-uno-r4-wifi/#htoc-1).

This project uses the following third party projects:

* NTP: https://github.com/arduino-libraries/NTPClient/
* FastLED: https://fastled.io/docs/index.html
* Logger: https://github.com/thijse/Arduino-Log/
* RTC: https://docs.arduino.cc/tutorials/uno-r4-wifi/rtc/ & https://github.com/arduino/ArduinoCore-renesas/tree/1.5.1/libraries/RTC
* WiFiS3: https://docs.arduino.cc/tutorials/uno-r4-wifi/wifi-examples/ & https://github.com/arduino/ArduinoCore-renesas/tree/1.5.1/libraries/WiFiS3
* LED Matrix: https://docs.arduino.cc/tutorials/uno-r4-wifi/led-matrix/ & https://github.com/arduino/ArduinoCore-renesas/tree/1.5.1/libraries/Arduino_LED_Matrix

# Configuration

Many aspects can be configured. Each aspect has its own config file.

## LED Config

This project uses [FAST LED](https://fastled.io/docs/index.html).

Configuration is done in the `LedStripConfig.h` file.

Use `TIMER_LEDS`, to configure the number of LEDs in the strip.

## Timer Config

Use the `TimerConfig.h` file to set-up your timers.

Each timer must be provided as a time range, using two instances of `RTCTime`.
Notice, `RTCTime` contains more information than needed. Only the day-of-week, hour, minute and second are relevant.

So, if you like to have a timer starting every Monday on 7:05am and ends 7:35am, set it up like this:

```cpp
// RTCTime(day, month, year, hours, minutes, seconds, DayOfWeek, SaveLight)
{
  RTCTime(0, Month::JANUARY, 0, 7, 5, 0, DayOfWeek::MONDAY, SaveLight::SAVING_TIME_INACTIVE),
  RTCTime(0, Month::JANUARY, 0, 7, 35, 0, DayOfWeek::MONDAY, SaveLight::SAVING_TIME_INACTIVE)
}
```

## WiFi Config

The WiFi credentials are configured in the `WifiConfig.h` file.

Please ensure it is used in the projects `.ino` file: `#include "WifiConfig.h"`

## Flash Config

The `LedFlashConfig.h` can be used to:

* Disable the flash-mode
* Define time in milliseconds for _on_ and _off_ state
* Define the total duration the flash-mode is active

## Timezone Config

See the `TimerConfig.h` to configure your time-zone.
Per default, it is set to `Europe/Berlin`.

Technically, the time-zone is set for the whole system, using `setenv("TZ", TIME_ZONE, 1)` and `tzset()`.
The actual values can be figured out under: https://github.com/nayarsystems/posix_tz_db/blob/master/zones.csv

# Development

If connection to the board fails, it could be due to missing permissions. You can temporarily permit yourself via: `sudo chmod a+rw /dev/ttyACM0`

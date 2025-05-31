#include <ArduinoLog.h>
#include <RTC.h>
#include <WiFiS3.h>
#include <NTPClient.h>
#include <Arduino_LED_Matrix.h>
#include <FastLED.h>
#include <uICAL.h>
#include "LedMatrixDigits.h"
#include "WifiConfig.h"
#include "LedStripConfig.h"
#include "LedFlashConfig.h"
#include "TimeRange.h"
#include "TimerConfig.h"

CRGB leds[TIMER_LEDS];

ArduinoLEDMatrix matrix;

WiFiUDP wifiUdp;
NTPClient ntp(wifiUdp);

// Remember if LEDs are turned off.
// Probably prevents wear-down?
bool ledsBlack = false;

// unsigned int max 4294967295
//unsigned int rtcUpdateTimeout = 6 * 3600000; // 6 hours * 60 mins * 60 secs * 1000 millis
unsigned int rtcUpdateTimeout = 10000;  // millis FIXME increase number
unsigned int rtcUpdateLoopCount = 0;
unsigned int loopDelay = 990;
unsigned int loopsRequiredForRtcUpdate = rtcUpdateTimeout / loopDelay;

// How frequently the NTP client can update its time, i.e.
// milliseconds that must have been passed since the last update.
// Defaults to 60000.
const uint32_t MIN_NTP_UPDATE_INTERVAL = 10000;

void setup() {
  // Start serial port connection
  Serial.begin(9600);
  while (!Serial) { ; }

  // LOG_LEVEL_FATAL, LOG_LEVEL_ERROR, LOG_LEVEL_WARNING,
  // LOG_LEVEL_NOTICE, LOG_LEVEL_TRACE, LOG_LEVEL_VERBOSE
  Log.begin(LOG_LEVEL_VERBOSE, &Serial);

  RTC.begin();

  initWifi();

  initNtpClient();

  updateRtcTime();

  matrix.begin();

  // Init FastLED
  FastLED.addLeds<LED_TYPE, DATA_PIN, COLOR_ORDER>(leds, TIMER_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.setBrightness(BRIGHTNESS);
  clearLed();
}

void loop() {

  // only update once and a while
  Log.verboseln("rtcUpdateLoopCount=%d, loopsRequiredForRtcUpdate=%d", rtcUpdateLoopCount, loopsRequiredForRtcUpdate);
  if (rtcUpdateLoopCount >= loopsRequiredForRtcUpdate) {
    if (updateRtcTime()) {
      rtcUpdateLoopCount = 0;
    }
  }
  rtcUpdateLoopCount++;

  RTCTime currentTime = getRtcTime();
  displayTimeInMatrix(currentTime);

  // iterate through date-time ranges
  bool timeRangeFound = false;
  for (TimeRange timeRange : timeRanges) {

    const char* strStartTime = timeRange.toString();
    //Log.traceln("Checking time range: %s", strStartTime);

    // Check if current time is within the range and same day of week
    if (isWithinTimeRange(currentTime, timeRange.start, timeRange.end)) {
      Log.noticeln("Current time is within range: %s", strStartTime);
      timeRangeFound = true;

      // Calculate remaining time
      int remainingSeconds = calculateRemainingSeconds(currentTime, timeRange.end);
      if (remainingSeconds > 0) {
        int hours = remainingSeconds / 3600;
        int minutes = (remainingSeconds % 3600) / 60;
        int seconds = remainingSeconds % 60;

        Log.noticeln("Remaining time: %d hours, %d minutes, %d seconds", hours, minutes, seconds);

        // Display remaining seconds on LEDs
        int totalSeconds = timeRange.end.getUnixTime() - timeRange.start.getUnixTime();
        displayRemainingSeconds(remainingSeconds, totalSeconds);

        // Exit loop if a range was found
        break;
      } else {
        Log.traceln(F("Time range has just ended"));
        timeRangeFound = false;
      }
    }

    if (isWithinFlashRange(currentTime, timeRange.end)) {
      Log.infoln(F("Current time is within flash range: %s"), strStartTime);
      flash();
    }
  }

  if (!timeRangeFound) {
    Log.traceln(F("Current time is NOT within any of the specified time-ranges"));
    clearLed();
  }

  delay(loopDelay);
}

/* Use the NTP client to set RTC. */
bool updateRtcTime() {
  if (ntp.update()) {
    Log.noticeln(F("NTP time updated"));

    // FIXME use timezone data
    // https://github.com/ropg/ezTime -> original. old.
    // https://github.com/TheGroundZero/ezTime -> fork but not much different
    // https://github.com/bxparks/AceTime -> huge!
    // or https://github.com/JChristensen/Timezone -> easy. small. but depends on PaulStoffregen/Time which has lots of bugs!
    int offsetSeconds = 7200;

    // if (ntp.summerTime()) {
    //   Log.noticeln(F("Summertime +2h"));
    //   offsetSeconds = 7200;
    // } else {
    //   Log.noticeln(F("Wintertime +1h"));
    //   offsetSeconds = 3600;
    // }

    RTCTime timeToSet = RTCTime(ntp.getEpochTime() + offsetSeconds);
    if (RTC.setTime(timeToSet)) {
      Log.noticeln("RTC time updated to: %s", timeToSet.toString().c_str());
      return true;
    } else {
      Log.errorln(F("Failed to update RTC time!"));
      return false;
    }
  }

  Log.errorln(F("Failed to update NTP time!"));
  return false;
}

/*
  Calculate if the date-time range has ended,
  but is still within the date-time range for flashing.
*/
bool isWithinFlashRange(RTCTime currentTime, RTCTime end) {
  int32_t remainingSeconds = calculateRemainingSeconds(currentTime, end);
  //Log.traceln("flash remaining seconds: %d", remainingSeconds);
  return remainingSeconds <= 0 && remainingSeconds > -FLASH_DURATION_SECONDS;
}

/* Get current time from RTC module */
RTCTime getRtcTime() {
  RTCTime currentTime;
  RTC.getTime(currentTime);
  return currentTime;
}

/* Turn off all LEDs */
void clearLed() {
  if (!ledsBlack) {
    fill_solid(leds, TIMER_LEDS, CRGB::Black);
    FastLED.show();
    ledsBlack = true;
  }
}

/* Fill all LEDs with color */
void fill_solid(CRGB color) {
  fill_solid(leds, TIMER_LEDS, color);
  FastLED.show();
  ledsBlack = false;
}

/* Flash all LEDs */
void flash() {
  Log.noticeln(F("Flashing for %d times"), FLASH_TIMES);

  for (short flashed = 0; flashed < FLASH_TIMES; flashed++) {
    fill_solid(FLASH_COLOR);
    delay(FLASH_ON_MS);
    clearLed();
    delay(FLASH_OFF_MS);
  }
}

/* Initialize NTP client with German timezone data */
void initNtpClient() {
  // CEST: Central European Summertime
  // Begin european summer time on the last sunday of March 2nd GMT + 2h
  ntp.ruleDST("CEST", Last, Sun, Mar, 2, 120);

  // CET: Central European Time
  // Begin european standard time on the last sunday of October 3rd GMT + 1h
  ntp.ruleSTD("CET", Last, Sun, Oct, 3, 60);
  
  ntp.setUpdateInterval(MIN_NTP_UPDATE_INTERVAL);
  ntp.begin();
}

/* Connect to WiFi */
void initWifi() {
  Log.notice("Connecting to %s ", ssid);
  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Log.notice(".");
  }
  Log.noticeln(F("\nWiFi connected"));
}

/*
  Check if current time is within the date-time range.
  Also compares the day of week.
*/
bool isWithinTimeRange(RTCTime current, RTCTime start, RTCTime end) {
  // Check if day of week matches
  if (current.getDayOfWeek() != start.getDayOfWeek() || current.getDayOfWeek() != end.getDayOfWeek()) {
    return false;
  }

  // Convert to seconds since midnight for comparison
  int32_t currentSeconds = current.getHour() * 3600 + current.getMinutes() * 60 + current.getSeconds();
  int32_t startSeconds = start.getHour() * 3600 + start.getMinutes() * 60 + start.getSeconds();
  int32_t endSeconds = end.getHour() * 3600 + end.getMinutes() * 60 + end.getSeconds();

  // Check if current time is within range
  return (currentSeconds >= startSeconds && currentSeconds <= endSeconds);
}

/* Calculate remaining seconds */
int calculateRemainingSeconds(RTCTime current, RTCTime end) {
  int currentSeconds = current.getHour() * 3600 + current.getMinutes() * 60 + current.getSeconds();
  int endSeconds = end.getHour() * 3600 + end.getMinutes() * 60 + end.getSeconds();
  return endSeconds - currentSeconds;
}

/*
  Display remaining time on LED strip.
  Start by displaying the bottom 30 LEDs in red,
  the next 37 LEDs in yellow and the remaining 77 LEDs in green.
  Switch off LEDs from top to bottom depending on remaining time.
*/
void displayRemainingSeconds(int remainingSeconds, int totalSeconds) {
  // array index from 0-143
  fill_solid(leds, 30, CRGB::Red);          // 0-29
  fill_solid(leds + 30, 37, CRGB::Yellow);  // 30-66
  fill_solid(leds + 67, 77, CRGB::Green);   // 67-143
  ledsBlack = false;

  // Map remainingSeconds to number of LEDs (e.g. max 144 LEDs for up to 3600 seconds/1 hour)
  int litLeds = map(remainingSeconds, 0, totalSeconds, 0, TIMER_LEDS);
  Log.traceln("Lit LEDs: %d", litLeds);

  // Switch off LEDs based on remaining seconds
  for (int i = TIMER_LEDS - 1; i >= litLeds; i--) {
    leds[i] = CRGB::Black;
  }

  FastLED.show();
}

/* Display current time in Matrix */
void displayTimeInMatrix(RTCTime currentTime) {
  String hour = (String)currentTime.getHour();
  if (hour.length() == 1) {
    hour = "0" + hour;
  }

  String minutes = (String)currentTime.getMinutes();
  if (minutes.length() == 1) {
    minutes = "0" + minutes;
  }

  Log.traceln("Matrix: %s:%s", hour.c_str(), minutes.c_str());

  setDigit(first, digits[hour.substring(0, 1).toInt()]);
  setDigit(second, digits[hour.substring(1).toInt()]);
  setDigit(third, digits[minutes.substring(0, 1).toInt()]);
  setDigit(fourth, digits[minutes.substring(1).toInt()]);

  rotateFrame();

  matrix.renderBitmap(currentFrame, NO_OF_ROWS, NO_OF_COLS);
}

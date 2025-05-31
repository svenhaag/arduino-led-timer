// Enable or disable flash mode
const boolean FLASH_ENABLED = true;

// Total duration for flashing
const short FLASH_DURATION_SECONDS = 180;

// Milliseconds the LEDs are turned on during flash mode
const short FLASH_ON_MS = 400;

// Milliseconds the LEDs are turned off during flash mode
const short FLASH_OFF_MS = 350;

// Pre-calculate how many times we have to flash.
// Instead of providing a flash duration in seconds, this
// can be used to define the number of flashes.
// One flash means one on-off cycle.
const short FLASH_TIMES = FLASH_DURATION_SECONDS * 1000 / (FLASH_ON_MS + FLASH_OFF_MS);

// Define the flash color
const CRGB FLASH_COLOR = CRGB::Red;
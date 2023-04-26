#include <Arduino.h>
#include <TMCStepper.h>

#define EN_PIN 38           // Enable
#define DIR_PIN 55          // Direction
#define STEP_PIN 54         // Step
#define CS_PIN 42           // Chip select
#define DRIVER_ADDRESS 0b00 // TMC2209 Driver address according to MS1 and MS2

#define R_SENSE 0.11f // Match to your driver
                      // SilentStepStick series use 0.11
                      // UltiMachine Einsy and Archim2 boards use 0.2
                      // Panucatt BSD2660 uses 0.1
                      // Watterott TMC5160 uses 0.075

// Select your stepper driver type
TMC2130Stepper driver(CS_PIN, R_SENSE); // Hardware SPI
// TMC2130Stepper driver(CS_PIN, R_SENSE, SW_MOSI, SW_MISO, SW_SCK); // Software SPI

void setup()
{
  pinMode(EN_PIN, OUTPUT);
  pinMode(STEP_PIN, OUTPUT);
  pinMode(DIR_PIN, OUTPUT);
  digitalWrite(EN_PIN, LOW); // Enable driver in hardware

  // Enable one according to your setup
  SPI.begin(); // SPI drivers

  driver.begin();          //  SPI: Init CS pins and possible SW SPI pins
  driver.toff(5);          // Enables driver in software
  driver.rms_current(600); // Set motor RMS current
  driver.microsteps(16);   // Set microsteps to 1/16th

  driver.en_pwm_mode(true);   // Toggle stealthChop on TMC2130/2160/5130/5160
  driver.pwm_autoscale(true); // Needed for stealthChop
}

bool shaft = false;

void loop()
{
  // Run 5000 steps and switch direction in software
  for (uint16_t i = 5000; i > 0; i--)
  {
    digitalWrite(STEP_PIN, HIGH);
    delayMicroseconds(160);
    digitalWrite(STEP_PIN, LOW);
    delayMicroseconds(160);
  }
  shaft = !shaft;
  driver.shaft(shaft);
}
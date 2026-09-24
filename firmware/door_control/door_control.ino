/*
  IoT Smart Door Lock & Visitor Monitoring System

  ELECTRIC DOOR LOCK CONTROL

  Function:
  Controls an electric door lock through a relay/control module.

  Commands through Serial Monitor:
      OPEN  -> unlocks the door
      CLOSE -> locks the door

  IMPORTANT:
  The relay GPIO and relay logic level below are configurable.
  Do not connect an electric lock directly to an ESP32 GPIO.
*/


#include <Arduino.h>


// =====================================================
// HARDWARE CONFIGURATION
// =====================================================

// GPIO connected to the relay/control input.
// Change this if your actual hardware uses another pin.
#define LOCK_RELAY_PIN 26


// -----------------------------------------------------
// Relay logic
// -----------------------------------------------------
//
// Many relay modules are ACTIVE LOW:
//
// LOW  = relay ON
// HIGH = relay OFF
//
// If your relay works the opposite way, change these.
#define RELAY_ON  LOW
#define RELAY_OFF HIGH


// =====================================================
// DOOR LOCK FUNCTIONS
// =====================================================

void lockDoor() {

  /*
     For a typical electric lock system,
     the relay returns to the locked state.
  */

  digitalWrite(
    LOCK_RELAY_PIN,
    RELAY_OFF
  );

  Serial.println("Door: LOCKED");
}


void unlockDoor() {

  /*
     Activate the relay/control output
     to operate the electric lock.
  */

  digitalWrite(
    LOCK_RELAY_PIN,
    RELAY_ON
  );

  Serial.println("Door: UNLOCKED");
}


// =====================================================
// SETUP
// =====================================================

void setup() {

  Serial.begin(115200);

  // Configure relay control pin
  pinMode(
    LOCK_RELAY_PIN,
    OUTPUT
  );

  // Start in locked state
  lockDoor();

  Serial.println();
  Serial.println(
    "================================="
  );

  Serial.println(
    " SMART ELECTRIC DOOR LOCK"
  );

  Serial.println(
    " CONTROL MODULE"
  );

  Serial.println(
    "================================="
  );

  Serial.println();
  Serial.println(
    "Available commands:"
  );

  Serial.println(
    "OPEN  - Unlock door"
  );

  Serial.println(
    "CLOSE - Lock door"
  );

  Serial.println();
}


// =====================================================
// MAIN LOOP
// =====================================================

void loop() {

  if (Serial.available() > 0) {

    String command =
      Serial.readStringUntil('\n');

    command.trim();
    command.toUpperCase();


    // -----------------------------------------------
    // OPEN DOOR
    // -----------------------------------------------

    if (command == "OPEN") {

      unlockDoor();

    }


    // -----------------------------------------------
    // CLOSE DOOR
    // -----------------------------------------------

    else if (command == "CLOSE") {

      lockDoor();

    }


    // -----------------------------------------------
    // INVALID COMMAND
    // -----------------------------------------------

    else {

      Serial.println(
        "Invalid command."
      );

      Serial.println(
        "Use OPEN or CLOSE."
      );
    }
  }
}

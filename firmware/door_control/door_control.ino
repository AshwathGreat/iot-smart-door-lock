/*
  IoT Smart Door Lock & Visitor Monitoring System

  Door Control Module

  Purpose:
  Controls the door-lock actuator from a control command.

  Project architecture:

      Alexa / Remote Control
              |
              v
       Door Control Logic
              |
              v
        Relay / Actuator
              |
              v
          Door Lock

  IMPORTANT:
  The exact relay GPIO and lock mechanism used in the
  original prototype were not documented in the available
  project material. Therefore, the values below are
  configurable placeholders.
*/

#include <Arduino.h>

// =====================================================
// HARDWARE CONFIGURATION
// =====================================================

// Replace this with the GPIO connected to your
// door-lock relay/actuator.
#define LOCK_CONTROL_PIN 26

// Change these if your relay works in the opposite way.
#define LOCKED   LOW
#define UNLOCKED HIGH


// =====================================================
// LOCK CONTROL FUNCTIONS
// =====================================================

void lockDoor() {

  digitalWrite(
    LOCK_CONTROL_PIN,
    LOCKED
  );

  Serial.println("Door locked.");
}


void unlockDoor() {

  digitalWrite(
    LOCK_CONTROL_PIN,
    UNLOCKED
  );

  Serial.println("Door unlocked.");
}


// =====================================================
// SETUP
// =====================================================

void setup() {

  Serial.begin(115200);

  pinMode(
    LOCK_CONTROL_PIN,
    OUTPUT
  );

  // Start in the locked state.
  lockDoor();

  Serial.println();
  Serial.println(
    "Smart Door Control Module Ready"
  );

  Serial.println(
    "Commands:"
  );

  Serial.println(
    "  OPEN  - Unlock door"
  );

  Serial.println(
    "  CLOSE - Lock door"
  );
}


// =====================================================
// MAIN LOOP
// =====================================================

void loop() {

  // Receive a simple control command
  // through the Serial Monitor.

  if (Serial.available()) {

    String command =
      Serial.readStringUntil('\n');

    command.trim();
    command.toUpperCase();


    if (command == "OPEN") {

      unlockDoor();

    }

    else if (command == "CLOSE") {

      lockDoor();

    }

    else {

      Serial.println(
        "Unknown command."
      );

      Serial.println(
        "Use OPEN or CLOSE."
      );
    }
  }
}

# IoT Smart Door Lock & Visitor Monitoring System

An IoT-enabled smart door security prototype combining an ESP32-CAM,
visitor image capture, email notification and remote door-control
functionality.

## Overview

Traditional door locks require the user to be physically present
to access the door. This project explores how IoT technology can
provide remote visitor monitoring and door access control.

When a visitor activates the doorbell, the ESP32-CAM captures an
image of the visitor and sends it to the owner through email.

The system also incorporates Alexa-based remote door-control
functionality for controlling the door mechanism remotely.

## System Workflow

Visitor
↓
Doorbell
↓
ESP32-CAM
↓
Image Capture
↓
Wi-Fi
↓
Gmail Notification
↓
Owner
↓
Remote Door Control
↓
Door Lock

## Key Features

- ESP32-CAM based visitor monitoring
- Doorbell-triggered image capture
- Email notification with visitor image
- Wi-Fi connectivity
- Remote door-control functionality
- Alexa integration
- IoT-based home automation concept

## Hardware

- ESP32-CAM
- Camera module
- Doorbell switch
- Door-lock mechanism
- Door-control circuit
- Power supply

## Software & Technologies

- Arduino IDE
- Embedded C/C++
- ESP32
- Wi-Fi
- SMTP
- Gmail
- Alexa
- IoT

## Working Principle

1. A visitor activates the doorbell.
2. The ESP32-CAM detects the trigger.
3. The camera captures an image.
4. The image is processed and sent through the internet.
5. The owner receives the visitor image through email.
6. The owner can use the remote door-control functionality.
7. The door mechanism responds to the authorized control command.

## System Architecture

The system consists of two major functions:

1. Visitor monitoring using the ESP32-CAM and email notification.
2. Remote door-control functionality using Alexa integration.

The visitor-monitoring workflow is:

Visitor → Doorbell → ESP32-CAM → Image Capture → Wi-Fi → Gmail → Owner

The remote-access workflow is:

Owner → Alexa → Door-Control System → Door Lock

## Circuit Diagram

[Circuit diagram]

## Project Demonstration

[Project photographs / demonstration video]

## Project Background

This project was developed as part of the Times NIE Vista Ideathon
2023–24.

The team was selected among the Top Eleven teams to present the
project at the Grand Finale at Hilton Chennai.

## Team

- Ashwath Krishna S.
- Jivitesh Kumar S.

Teacher-Mentor:
Mrs. Subhadra

## Future Improvements

- Multi-factor authentication
- RFID-based access
- Biometric authentication
- Cloud-based monitoring
- Wider-angle surveillance
- Additional visitor alerts
- Additional environmental sensors

## Disclaimer

This repository documents an educational prototype demonstrating
IoT, embedded systems, image capture, email communication and
remote-control concepts.

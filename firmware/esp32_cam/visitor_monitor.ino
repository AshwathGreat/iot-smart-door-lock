/*
  IoT Smart Door Lock & Visitor Monitoring System

  ESP32-CAM Visitor Monitoring Module

  Workflow:
  Doorbell pressed
       ↓
  ESP32-CAM captures visitor image
       ↓
  Image temporarily saved to LittleFS
       ↓
  Image sent to owner by email

  IMPORTANT:
  Keep credentials private.
*/

#include "esp_camera.h"
#include "SPI.h"
#include "driver/rtc_io.h"
#include <ESP_Mail_Client.h>
#include <FS.h>
#include <WiFi.h>

// =====================================================
// Wi-Fi
// =====================================================

const char* ssid = "YOUR_WIFI_NAME";
const char* password = "YOUR_WIFI_PASSWORD";

// =====================================================
// Gmail SMTP
// =====================================================

#define emailSenderAccount    "YOUR_SENDER_EMAIL"
#define emailSenderPassword   "YOUR_APP_PASSWORD"

#define smtpServer             "smtp.gmail.com"
#define smtpServerPort         465

#define emailSubject           "Visitor at the Door"
#define emailRecipient         "YOUR_RECIPIENT_EMAIL"

// =====================================================
// Camera: AI-Thinker ESP32-CAM
// =====================================================

#define CAMERA_MODEL_AI_THINKER

#if defined(CAMERA_MODEL_AI_THINKER)

#define PWDN_GPIO_NUM     32
#define RESET_GPIO_NUM    -1
#define XCLK_GPIO_NUM      0
#define SIOD_GPIO_NUM     26
#define SIOC_GPIO_NUM     27

#define Y9_GPIO_NUM       35
#define Y8_GPIO_NUM       34
#define Y7_GPIO_NUM       39
#define Y6_GPIO_NUM       36
#define Y5_GPIO_NUM       21
#define Y4_GPIO_NUM       19
#define Y3_GPIO_NUM       18
#define Y2_GPIO_NUM        5

#define VSYNC_GPIO_NUM    25
#define HREF_GPIO_NUM     23
#define PCLK_GPIO_NUM     22

#endif

// =====================================================
// Doorbell
// =====================================================

// PLACEHOLDER ONLY.
// Replace this after confirming the GPIO used by your
// original hardware.
#define DOORBELL_PIN 13

#define FILE_PHOTO "/visitor.jpg"

SMTPSession smtp;


// =====================================================
// Capture photo and save to LittleFS
// =====================================================

bool capturePhotoSaveLittleFS() {

  camera_fb_t* fb = NULL;

  // Discard a few initial frames so the camera can stabilize.
  for (int i = 0; i < 3; i++) {

    fb = esp_camera_fb_get();

    if (fb) {
      esp_camera_fb_return(fb);
    }

    fb = NULL;
  }

  // Capture visitor image.
  fb = esp_camera_fb_get();

  if (!fb) {
    Serial.println("Camera capture failed.");
    return false;
  }

  Serial.printf("Saving photo: %s\n", FILE_PHOTO);

  File file = LittleFS.open(FILE_PHOTO, FILE_WRITE);

  if (!file) {
    Serial.println("Failed to open file for writing.");
    esp_camera_fb_return(fb);
    return false;
  }

  file.write(fb->buf, fb->len);

  Serial.print("Photo saved. Size: ");
  Serial.print(fb->len);
  Serial.println(" bytes");

  file.close();

  esp_camera_fb_return(fb);

  return true;
}


// =====================================================
// Send photo by email
// =====================================================

bool sendPhoto() {

  ESP_Mail_Session session;

  session.server.host_name = smtpServer;
  session.server.port = smtpServerPort;

  session.login.email = emailSenderAccount;
  session.login.password = emailSenderPassword;

  SMTP_Message message;

  message.sender.name = "Smart Door Lock";
  message.sender.email = emailSenderAccount;

  message.subject = emailSubject;

  message.addRecipient(
    "Owner",
    emailRecipient
  );

  String htmlMsg =
    "<h2>Visitor detected at the door</h2>"
    "<p>The ESP32-CAM captured an image when "
    "the doorbell was activated.</p>";

  message.html.content = htmlMsg.c_str();
  message.html.charSet = "utf-8";
  message.html.transfer_encoding =
    Content_Transfer_Encoding::enc_qp;


  // ---------------------------------------------------
  // Photo attachment
  // ---------------------------------------------------

  SMTP_Attachment attachment;

  attachment.descr.filename = "visitor.jpg";
  attachment.descr.mime = "image/jpeg";

  attachment.file.path = FILE_PHOTO;

  attachment.file.storage_type =
    esp_mail_file_storage_type_flash;

  attachment.descr.transfer_encoding =
    Content_Transfer_Encoding::enc_base64;

  message.addAttachment(attachment);


  // ---------------------------------------------------
  // Connect to SMTP server
  // ---------------------------------------------------

  if (!smtp.connect(&session)) {

    Serial.println("SMTP connection failed.");

    return false;
  }


  // ---------------------------------------------------
  // Send email
  // ---------------------------------------------------

  if (!MailClient.sendMail(&smtp, &message)) {

    Serial.print("Email sending failed: ");
    Serial.println(smtp.errorReason());

    smtp.closeSession();

    return false;
  }

  Serial.println("Visitor email sent successfully.");

  smtp.closeSession();

  return true;
}


// =====================================================
// Camera initialization
// =====================================================

bool initializeCamera() {

  camera_config_t config;

  config.ledc_channel = LEDC_CHANNEL_0;
  config.ledc_timer = LEDC_TIMER_0;

  config.pin_d0 = Y2_GPIO_NUM;
  config.pin_d1 = Y3_GPIO_NUM;
  config.pin_d2 = Y4_GPIO_NUM;
  config.pin_d3 = Y5_GPIO_NUM;
  config.pin_d4 = Y6_GPIO_NUM;
  config.pin_d5 = Y7_GPIO_NUM;
  config.pin_d6 = Y8_GPIO_NUM;
  config.pin_d7 = Y9_GPIO_NUM;

  config.pin_xclk = XCLK_GPIO_NUM;
  config.pin_pclk = PCLK_GPIO_NUM;
  config.pin_vsync = VSYNC_GPIO_NUM;
  config.pin_href = HREF_GPIO_NUM;

  config.pin_sscb_sda = SIOD_GPIO_NUM;
  config.pin_sscb_scl = SIOC_GPIO_NUM;

  config.pin_pwdn = PWDN_GPIO_NUM;
  config.pin_reset = RESET_GPIO_NUM;

  config.xclk_freq_hz = 20000000;

  config.pixel_format = PIXFORMAT_JPEG;

  if (psramFound()) {

    config.frame_size = FRAMESIZE_UXGA;
    config.jpeg_quality = 10;
    config.fb_count = 2;

  } else {

    config.frame_size = FRAMESIZE_SVGA;
    config.jpeg_quality = 12;
    config.fb_count = 1;
  }

  esp_err_t err = esp_camera_init(&config);

  if (err != ESP_OK) {

    Serial.printf(
      "Camera initialization failed: 0x%x\n",
      err
    );

    return false;
  }

  Serial.println("Camera initialized successfully.");

  return true;
}


// =====================================================
// Setup
// =====================================================

void setup() {

  Serial.begin(115200);

  pinMode(
    DOORBELL_PIN,
    INPUT_PULLUP
  );


  // ---------------------------------------------------
  // Initialize LittleFS
  // ---------------------------------------------------

  if (!LittleFS.begin(true)) {

    Serial.println(
      "LittleFS initialization failed."
    );

    return;
  }

  Serial.println("LittleFS initialized.");


  // ---------------------------------------------------
  // Initialize camera
  // ---------------------------------------------------

  if (!initializeCamera()) {
    return;
  }


  // ---------------------------------------------------
  // Connect Wi-Fi
  // ---------------------------------------------------

  WiFi.begin(
    ssid,
    password
  );

  Serial.print("Connecting to Wi-Fi");

  while (
    WiFi.status() != WL_CONNECTED
  ) {

    delay(500);
    Serial.print(".");
  }

  Serial.println();

  Serial.println(
    "Wi-Fi connected."
  );

  Serial.print(
    "IP address: "
  );

  Serial.println(
    WiFi.localIP()
  );

  Serial.println(
    "Smart door visitor monitor ready."
  );
}


// =====================================================
// Main loop
// =====================================================

void loop() {

  static bool previousDoorbellState = HIGH;

  bool currentDoorbellState =
    digitalRead(DOORBELL_PIN);


  // Detect button press
  if (
    previousDoorbellState == HIGH &&
    currentDoorbellState == LOW
  ) {

    Serial.println(
      "Doorbell activated."
    );

    delay(100);


    // Capture visitor image
    if (
      capturePhotoSaveLittleFS()
    ) {

      // Send image by email
      sendPhoto();
    }


    // Debounce
    delay(2000);
  }


  previousDoorbellState =
    currentDoorbellState;
}

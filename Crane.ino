#include <Servo.h>
#include <ESP8266WiFi.h>
#include <ThingerESP8266.h>


char *ssid = "<REPLACE WITH YOUR SSID>";
char *password = "<REPLACE WITH YOUR PASSWORD>";

#define SERVO_BASE D6
#define SERVO_HAND D8

#define YELLOW_LED D4

const char* thingerUser = "<REPLACE WITH THINGER USER>";
const char* deviceCode = "<REPLACE WITH THINGER SECRET CODE>";
const char* deviceName = "<REPLACE WITH THINGER DEVICE NAME>";


Servo servo_base;
Servo servo_hand;

int RESET_ANGLE = 0;
int DEFAULT_BASE_MOVEMENT = 10;

int base_angle = RESET_ANGLE;
int hand_angle = RESET_ANGLE;

ThingerESP8266 *thing;

unsigned long timer;

void setup() {

  timer = millis();

  pinMode(YELLOW_LED, OUTPUT);

  Serial.begin(115200);
  servo_base.attach(SERVO_BASE);
  servo_base.write(base_angle);

  servo_hand.attach(SERVO_HAND);
  servo_hand.write(hand_angle);

  connectToWiFi();

  thing =  new ThingerESP8266(thingerUser, deviceName, deviceCode);

  (*thing)["crane_base_left"] << [](pson & in) {
    Serial.println("crane_base_left");
    moveBaseServo(DEFAULT_BASE_MOVEMENT);
  };

  (*thing)["crane_base_right"] << [](pson & in) {
    Serial.println("crane_base_right");
    moveBaseServo(DEFAULT_BASE_MOVEMENT * -1);
  };

  (*thing)["crane_hand_left"] << [](pson & in) {
    Serial.println("crane_hand_left");
    moveHandServo(DEFAULT_BASE_MOVEMENT);
  };

  (*thing)["crane_hand_right"] << [](pson & in) {
    Serial.println("crane_hand_right");
    moveHandServo(DEFAULT_BASE_MOVEMENT * -1);
  };
}

void resetCrane() {
  servo_hand.write(RESET_ANGLE);
  hand_angle = RESET_ANGLE;
  servo_base.write(RESET_ANGLE);
  base_angle = RESET_ANGLE;

}

void moveHandServo(int angle) {
  hand_angle = hand_angle + angle;
  servo_hand.write(hand_angle);

}

void moveBaseServo(int angle) {
  base_angle = base_angle + angle;
  servo_base.write(base_angle);
}

void connectToWiFi() {
  WiFi.begin(ssid, password);
  Serial.println("Connecting to Wifi");

  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    blink(YELLOW_LED, 500, 500);
    delay(500);
  }
  Serial.println("");
  Serial.println("IP Address : ");
  Serial.println(WiFi.localIP());

  resetCrane();
  Serial.println("Moved the servo to 0");

}

void loop() {
  (*thing).handle();
  delay(500);
}

void blink(int pin_number, int on, int off)
{
  digitalWrite(pin_number, HIGH);
  delay(on);
  digitalWrite(pin_number, LOW);
  delay(off);
}

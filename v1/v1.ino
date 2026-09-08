#include <TimerOne.h>

// Motor pins:
#define E1 6  // Speed of right wheel
#define M1 7  // Direction of right wheel (HIGH/LOW)
#define E2 5  // Speed of left wheel
#define M2 4  // Direction of right wheel (HIGH/LOW)

// Sensor pins:
#define right_sensor_pin 11
#define left_sensor_pin 10
#define center_sensor_pin 9

#define ar_sensor_pin "A0"
#define al_sensor_pin "A1"
#define ac_sensor_pin "A2"

#define sensor_const 101
#define update_time 500

// Create a struct that can be used to pass all sensor values in one variable
struct SensorStruct {
  boolean right;
  boolean left;
  boolean center;
};

uint8_t speed = 255;      // Universal speed to be used as the benchmark for both wheels.
uint8_t right_speed = 0;  // Right speed: This should be used compared to the benchmark speed, and decreased to turn right.
uint8_t left_speed = 0;   // Left speed: This should be used compared to the benchmark speed, and decreased to turn left.

void setup() {
  pinMode(M1, OUTPUT);
  pinMode(M2, OUTPUT);

  Timer1.initialize(update_time * 1000);  // In microseconds
  Timer1.attachInterrupt(update);
}

void loop() {
  delay(1000);
}

void update() {
  /* This function is called by TimerOne and calls all the other functions necessary */
  SensorStruct x = sensor_status();
  int angle = set_angle(x);
}


SensorStruct sensor_status() {
  /* This function checks the status of each sensor. */

  SensorStruct x;
  if (analogRead(right_sensor_pin) > sensor_const) {
    x.right = true;  // True means on black line
  } else {
    x.right = false;
  }

  if (analogRead(left_sensor_pin) > sensor_const) {
    x.left = true;
  } else {
    x.left = false;
  }

  if (analogRead(center_sensor_pin) > sensor_const) {
    x.center = true;
  } else {
    x.center = false;
  }
  return x;
}

int set_angle(SensorStruct x) {
  /* This function takes a desired angle as an input, and calls the function set_speed with the correct ratio to turn */
}

void set_speed(uint8_t right_ratio, uint8_t left_ratio) {
  /* This function takes the wheel ratio, and tells the engine function what how fast each wheel should turn. */
}

void engine(uint8_t right_speed, uint8_t left_speed) {
  /* This function tells the wheels to turn */
  analogWrite(E1, right_speed);
  analogWrite(E2, left_speed);
}
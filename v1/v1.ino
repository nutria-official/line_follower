#include <TimerOne.h>

// Motor pins:
#define E1  6 // Speed of right wheel
#define M1  7 // Direction of right wheel (HIGH/LOW)
#define E2  5 // Speed of left wheel
#define M2  4 // Direction of right wheel (HIGH/LOW)

// Sensor pins:
#define right_sensor 11
#define left_sensor 10
#define center_sensor 9

#define ar_sensor "A0"
#define al_sensor "A1"
#define ac_sensor "A2"

// Create a struct that can be used to pass all sensor values in one variable

uint8_t speed = 255; // Universal speed to be used as the benchmark for both wheels.
uint8_t right_speed = 0; // Right speed: This should be used compared to the benchmark speed, and decreased to turn right.
uint8_t left_speed = 0; // Left speed: This should be used compared to the benchmark speed, and decreased to turn left.

void setup() {
    pinMode(M1, OUTPUT);
    pinMode(M2, OUTPUT);
}

void loop() {
  
}



void sensor_status() -> boolean {

}

void set_angle() {
  /* This function takes a desired angle as an input, and calls the function set_speed with the correct ratio to turn */
}

void set_speed(uint8_t right_ratio, uint8_t left_ratio) {
  /* This function takes the wheel ratio, and tells the drive function what how fast each wheel should turn. */

}

void drive(uint8_t right_speed, uint8_t left_speed) {
  /* This function tells the wheels to turn */
  analogWrite(E1, right_speed);
  analogWrite(E2, left_speed);
}
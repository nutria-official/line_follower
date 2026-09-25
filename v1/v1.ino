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

#define sensor_const 500
#define update_time 50 // In millis
#define speed = 255.0f; // Universal speed to be used as the benchmark for both wheels.

// Create a struct that can be used to pass all sensor values in one variable
typedef struct {
  boolean right = false;
  boolean left = false;
  boolean center = false;
} SensorStruct;

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
  set_ratio(&x);
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

void set_ratio(SensorStruct *x) {
  int16_t angle = set_angle(&x);
  
  float normalised = angle / 180.0; 

  uint8_t left_speed = speed - (normalised * speed);
  uint8_t right_speed = speed + (normalised * speed);

  set_speed(right_speed, left_speed);
}

int16_t set_angle(SensorStruct *x) {
  /* This function takes a desired angle as an input, and calls the function set_speed with the correct ratio to turn */
  switch(x) {
    case x->right && x->center && !x->left: // Is drifting to the left.
      return 45;
    case !x->right && x->center && x->left: // Is drifting to the right.
      return -45;
    case x->right && !x->center && !x->left: // Is turning sharp left.
      return 90;
    case !x->right && !x->center && x->left: // Is turning sharp right.
      return -90;
    case !x->right && x->center && !x->left: // Is driving straight.
      return 0;
    case x->right && x->center && x->left: // Is turning, but this is not handled yet.
      Serial.println("Is doing something we didnt prepare for.");
      return 0;
    case x->right && !x->center && x->left: // Is coming back from nowhere.
      Serial.println("How?? Is coming back on track.");
      return 0;
    case !x->right && !x->center && !x->left: // We're out of the track.
      Serial.println("Is doing something we didnt prepare for. Out of track");
      return 180;
    default:
      Serial.println("SHIIIT");
  }
  return 0;
}

void set_speed(uint8_t right_speed, uint8_t left_speed) {
  /* This function tells the wheels to turn */
  analogWrite(E1, right_speed);
  analogWrite(E2, left_speed);
}

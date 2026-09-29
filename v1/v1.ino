// #include <TimerOne.h>

// Motor pins:
#define E1 6  // Speed of right wheel
#define M1 7  // Direction of right wheel (HIGH/LOW)
#define E2 5  // Speed of left wheel
#define M2 4  // Direction of right wheel (HIGH/LOW)

// Sensor pins:

#define ar_sensor_pin A2
#define al_sensor_pin A1
#define ac_sensor_pin A0

#define sensor_const 750
//#define update_time 50 // In millis
#define speed 255.0f // Universal speed to be used as the benchmark for both wheels.

// Create a struct that can be used to pass all sensor values in one variable
typedef struct {
  boolean right = false;
  boolean left = false;
  boolean center = false;
} SensorStruct;

void setup() {
  pinMode(M1, OUTPUT);
  pinMode(M2, OUTPUT);
  pinMode(al_sensor_pin, INPUT);
  pinMode(ac_sensor_pin, INPUT);
  pinMode(ar_sensor_pin, INPUT);
  digitalWrite(M1, LOW);
  digitalWrite(M2, HIGH);

  //Timer1.initialize(update_time);  // In microseconds
  //Timer1.attachInterrupt(update);
  Serial.begin(9600);
}

void loop() {
  update();
}

void update() {
  /* This function is called by TimerOne and calls all the other functions necessary */
  SensorStruct x = sensor_status();
  set_ratio(&x);
}


SensorStruct sensor_status() {
  /* This function checks the status of each sensor. */
  Serial.print(analogRead(al_sensor_pin));
  Serial.print(", ");
  Serial.print(analogRead(ac_sensor_pin));
  Serial.print(", ");
  Serial.print(analogRead(ar_sensor_pin));
  SensorStruct x;
  if (analogRead(ar_sensor_pin) < sensor_const) {
    x.right = true;  // True means on black line
  } else {
    x.right = false;
  }

  if (analogRead(al_sensor_pin) < sensor_const - 100) {
    x.left = true;
  } else {
    x.left = false;
  }

  if (analogRead(ac_sensor_pin) < sensor_const) {
    x.center = true;
  } else {
    x.center = false;
  }
  return x;
}

void set_ratio(SensorStruct *x) {
  int16_t angle = set_angle(x);
  
  float normalised = angle / 180.0; 

  uint8_t left_speed = (speed - (normalised * speed)) * 1;
  uint8_t right_speed = (speed + (normalised * speed)) * 1;

  set_speed(right_speed, left_speed);
}

int16_t set_angle(SensorStruct *x) {
  /* This function takes a desired angle as an input, and calls the function set_speed with the correct ratio to turn */
  if (x->right && x->center && !x->left) {  // Is drifting to the left.
    Serial.print(", left.");
    return 5;
  } else if (!x->right && x->center && x->left) { // Is drifting to the right.
    Serial.print(", right.");
    return -5;
  } else if (x->right && !x->center && !x->left) { // Is turning sharp left.
    Serial.print(", sharp right.");
    return -10;
  } else if (!x->right && !x->center && x->left) { // Is turning sharp right.
    Serial.print(", sharp left.");
    return 10;
  } else if (!x->right && x->center && !x->left) { // Is driving straight.
    Serial.print(", Straight.");
    return 0;
  } else if (x->right && x->center && x->left) { // We're turning.
    Serial.print(", all black");
    return 0;
  } else if (x->right && !x->center && x->left) { // We're out of track but on the way back on track.
    Serial.print(", front on black");
    return 0;
  } else { // Is out of track.
    Serial.print(", all white");
    return 180;
  }
  Serial.println("was");
  return 0;
}

void set_speed(uint8_t right_speed, uint8_t left_speed) {
  /* This function tells the wheels to turn */
  Serial.print(", ");
  Serial.print(right_speed);
  Serial.print(", ");
  Serial.println(left_speed);
  analogWrite(E1, right_speed);
  analogWrite(E2, left_speed);
}

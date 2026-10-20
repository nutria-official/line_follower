// Motor pins:
#define E1 6  // Speed of right wheel
#define M1 7  // Direction of right wheel (HIGH/LOW)
#define E2 5  // Speed of left wheel
#define M2 4  // Direction of right wheel (HIGH/LOW)

// Sensor pins:
#define al_sensor_pin A1
#define ac_sensor_pin A0
#define ar_sensor_pin A2

// Calibration for each sensor.
#define sensor_const_left 600
#define sensor_const_center 700
#define sensor_const_right 700
#define speed 200.0f // Universal speed to be used as the benchmark for both wheels. Max is 255.

#define sensor_buffer 256

// Create a struct that can be used to pass all sensor values in one variable
typedef struct {
  boolean right[sensor_buffer];
  boolean left[sensor_buffer];
  boolean center[sensor_buffer];
} SensorStruct;

uint16_t loop_cycles = 0; // Mod 255

void setup() {
  pinMode(M1, OUTPUT);
  pinMode(M2, OUTPUT);
  pinMode(al_sensor_pin, INPUT);
  pinMode(ac_sensor_pin, INPUT);
  pinMode(ar_sensor_pin, INPUT);
  digitalWrite(M1, LOW);
  digitalWrite(M2, HIGH); // Needs to be inversed because the wheel is rotating backwards normally.

  Serial.begin(9600);
}

void loop() {
  update();
}

void update() {
  loop_cycles = (loop_cycles + 1) % sensor_buffer == 0 ? 0 : loop_cycles + 1;
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

  x.left = analogRead(al_sensor_pin) < sensor_const_left ? true : false;
  x.center = analogRead(ac_sensor_pin) < sensor_const_center ? true : false;
  x.right = analogRead(ar_sensor_pin) < sensor_const_right ? true : false;
  return x;
}

void set_ratio(SensorStruct *x) {
  int16_t angle = set_angle(x);
  
  float normalised = angle / 180.0f; 

  Serial.print(", norm-angle = ");
  Serial.print(normalised);
  Serial.print(", angle = ");
  Serial.print(angle);
  uint8_t right_speed = 0;
  uint8_t left_speed = 0;
  if (normalised > 0) {
    left_speed = speed - (normalised * speed);
    right_speed = speed;
  } else if (normalised < 0) {
    left_speed = speed;
    right_speed = speed + (normalised * speed);
  } else {
    left_speed = speed;
    right_speed = speed;
  }
  left_speed *= 1.0f;

  set_speed(right_speed, left_speed);
}

int16_t set_angle(SensorStruct *x) {
  /* This function takes a desired angle as an input, and calls the function set_speed with the correct ratio to turn */
  if (x->right && x->center && !x->left) {  // Is drifting to the left.
    Serial.print(", right.");
    return 45;
  } else if (!x->right && x->center && x->left) { // Is drifting to the right.
    Serial.print(", left.");
    return -45;
  } else if (x->right && !x->center && !x->left) { // Is turning sharp left.
    Serial.print(", sharp right.");
    return -90;
  } else if (!x->right && !x->center && x->left) { // Is turning sharp right.
    Serial.print(", sharp left.");
    return 90;
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
    return 0;
  }
  Serial.println("was"); // "If this happens, I'll buy you an icecream" - Jonathan HA
  return 0;
}

void set_speed(uint8_t right_speed, uint8_t left_speed) {
  /* This function tells the wheels to turn */
  Serial.print(", ");
  Serial.print(left_speed);
  Serial.print(", ");
  Serial.println(right_speed);
  analogWrite(E1, right_speed);
  analogWrite(E2, left_speed);
}

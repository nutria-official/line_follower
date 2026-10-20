// Motor pins:
#define E1 6  // Speed of right wheel
#define M1 7  // Direction of right wheel (HIGH/LOW)
#define E2 5  // Speed of left wheel
#define M2 4  // Direction of right wheel (HIGH/LOW)

// Sensor pins:
#define e_sensor_pin A0
#define ne_sensor_pin A1
#define n_sensor_pin A2
#define nw_sensor_pin A3
#define w_sensor_pin A4

// Calibration for each sensor.
#define speed 200.0f // Universal speed to be used as the benchmark for both wheels. Max is 255.

// Sensor placement on halvcirkel.
#define e_angle 90
#define ne_angle 45
#define n_angle 0
#define nw_angle -45
#define w_angle -90

#define sensor_buffer 32 // The bots context window.

uint16_t loop_cycle = 0;
uint32_t last_millis = 0;

// Create a struct that can be used to pass all sensor values in one variable
typedef struct {
  float e[sensor_buffer];
  float ne[sensor_buffer];
  float n[sensor_buffer];
  float nw[sensor_buffer];
  float w[sensor_buffer];
  uint16_t millis_since_last[sensor_buffer];
} SensorStruct;


void setup() {
  pinMode(M1, OUTPUT);
  pinMode(M2, OUTPUT);
  digitalWrite(M1, LOW);
  digitalWrite(M2, HIGH); // Needs to be inversed because the wheel is rotating backwards normally.

  Serial.begin(9600);
}

void loop() {
  update();
}

void update() {
  SensorStruct *x = sensor_status();
  set_ratio(x);
  loop_cycle = (loop_cycle + 1) % sensor_buffer == 0 ? 0 : loop_cycle + 1;
}


SensorStruct *sensor_status() {
  /* This function checks the status of each sensor. */
  Serial.print(analogRead(w_sensor_pin));
  Serial.print(", ");
  Serial.print(analogRead(nw_sensor_pin));
  Serial.print(", ");
  Serial.print(analogRead(n_sensor_pin));
  Serial.print(", ");
  Serial.print(analogRead(ne_sensor_pin));
  Serial.print(", ");
  Serial.print(analogRead(e_sensor_pin));
  static SensorStruct x;

  // Inverse sensor read in decimal.

  x.e[loop_cycle]   = (1024 - analogRead(e_sensor_pin)) / 1024.0f;
  x.ne[loop_cycle] = (1024 - analogRead(ne_sensor_pin)) / 1024.0f;
  x.n[loop_cycle]  = (1024 - analogRead(n_sensor_pin)) / 1024.0f;
  x.nw[loop_cycle] = (1024 - analogRead(nw_sensor_pin)) / 1024.0f;
  x.w[loop_cycle]  = (1024 - analogRead(w_sensor_pin)) / 1024.0f;

  x.millis_since_last[loop_cycle] = loop_cycle == 0 ? 0 : millis() - last_millis;
  last_millis = millis();

  return &x;
}

void set_ratio(SensorStruct *x) {
  
  float normalised = set_angle(x) / 90.0f; 

  Serial.print(", norm-angle = ");
  Serial.print(normalised);

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
  set_speed(right_speed, left_speed);
}

int16_t set_angle(SensorStruct *x) {
  int8_t weighted_average = (x->e[loop_cycle]  * e_angle  +
                             x->ne[loop_cycle] * ne_angle + 
                             x->n[loop_cycle]  * n_angle  + 
                             x->nw[loop_cycle] * nw_angle + 
                             x->w[loop_cycle]  * w_angle
                             ) / 5;

  return weighted_average;
}

void set_speed(uint8_t right_speed, uint8_t left_speed) {
  /* This function tells the wheels to turn */
  Serial.print(", ls = ");
  Serial.print(left_speed);
  Serial.print(", rs = ");
  Serial.println(right_speed);
  analogWrite(E1, right_speed);
  analogWrite(E2, left_speed);
}

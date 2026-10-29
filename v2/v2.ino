//#include "ArduPID.h"
#include <vector>

// Motor pins:
#define r_speed_pin 6  // E1: Speed of right wheel
#define r_direction_pin 7  //M1: Direction of right wheel (HIGH/LOW)
#define l_speed_pin 5  //E2: Speed of left wheel
#define l_direction_pin 4  //M2: Direction of right wheel (HIGH/LOW)

// Calibration for each sensor.
#define u_speed 255.0f // Universal speed to be used as the benchmark for both wheels. Max is 255.

#define sensor_buffer 32 // The bots context window.

#define SENSOR_AMOUNT 5
#define BLACK 0.8f
#define WHITE 0.2f

float light_turn = 1/3;
float hard_turn = 1/5;

int r_speed;
int l_speed

std::vector<int> sensor_black = {
  900,
  900,
  900,
  900,
  900,
};

 std::vector<int> sensor_white = {
  700,
  700,
  600,
  500,
  700,
};

std::vector<float> sensor_values; 


int loop_cycle = 0;
size_t last_millis = 0;

void setup() {
  pinMode(r_direction_pin, OUTPUT);
  pinMode(l_direction_pin, OUTPUT);
  digitalWrite(r_direction_pin, LOW);
  digitalWrite(l_direction_pin, LOW);

  Serial.begin(9600);
}

void loop() {
  Serial.print("A0 = ");
  Serial.print(analogRead(A0));
  Serial.print(", ");
  Serial.println(sensor_values[0]);

  Serial.print("A1 = ");
  Serial.print(analogRead(A1));
  Serial.print(", ");
  Serial.println(sensor_values[1]);

  Serial.print("A2 = ");
  Serial.print(analogRead(A2));
  Serial.print(", ");
  Serial.println(sensor_values[2]);

  Serial.print("A4 = ");
  Serial.print(analogRead(A3));
  Serial.print(", ");
  Serial.println(sensor_values[3]);

  Serial.print("A4 = ");
  Serial.print(analogRead(A4));
  Serial.print(", ");
  Serial.println(sensor_values[4]);

  update_sensor_val();
  update_direction();
  set_speed();
}

std::vector<int> read_sensor_data(){
  return {
    analogRead(A0),
    analogRead(A1),
    analogRead(A2),
    analogRead(A3),
    analogRead(A4),
  }
}

void update_sensor_val() {
  std::vector<int> _sensor_data = read_sensor_data(); 

  for(int i = 0; i < SENSOR_AMOUNT; i++){
    int _sensor_diff = sensor_black[i] - sensor_white[i];
    sensor_values[i] = (sensor_black[i] - _sensor_data[i]) / (float)_sensor_diff;
  }
}

void update_direction(){
  if(sensor_values[0] > BLACK){
    l_speed = u_speed*hard_turn;
    r_speed = u_speed;
  } 
  else if(sensor_values[1] > BLACK){
    l_speed = u_speed*light_turn;
    r_speed = u_speed;
  } 
  
  else if(sensor_values[5] > BLACK){
    r_speed = u_speed*hard_turn;
    l_speed = u_speed;
  } 
  else if(sensor_values[4] > BLACK){
    r_speed = u_speed*light_turn;
    l_speed = u_speed;
  } 

  if(sensor_values[2] > BLACK){
    r_speed = u_speed;
    l_speed = u_speed;
  }
}

void set_speed(){
  analogWrite(r_speed_pin, r_speed);
  analogWrite(l_speed_pin, l_speed);
}

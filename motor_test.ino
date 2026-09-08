
// Motor pins:
#define E1  6;
#define M1  7;
#define E2  5;
#define M2  4;

// Sensor pins:

#define r_sensor 11
#define l_sensor 10
#define c_sensor 9

#define ar_sensor "A0"
#define al_sensor "A1"
#define ac_sensor "A2"

uint8_t RS = 0; // Right speed.
uint8_t LS = 0; // Left speed.

void setup()
{
    pinMode(M1, OUTPUT);
    pinMode(M2, OUTPUT);
}

void loop() {
  analogWrite(E1, speed);
  analogWrite(E2, speed);
  speed += 50;

  delay(2000);

  if (speed >= 255) {
    speed = 0;
  }
}

void drive() {

}

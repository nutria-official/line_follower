int E1 = 6;
int M1 = 7;
int E2 = 5;
int M2 = 4;

int speed = 0;
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
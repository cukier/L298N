boolean ctrl1;
boolean ctrl2;
boolean ctrl3;
boolean ctrlt;

int saida_r = 3;
int saida_s = 5;
int saida_t = 6;
int duty1 = 0;
int duty2 = 85;
int duty3 = 170;
int togglePin = 8;


void setup() {
  pinMode(saida_r, OUTPUT);
  pinMode(saida_s, OUTPUT);
  pinMode(saida_t, OUTPUT);
  pinMode(togglePin, OUTPUT);
}


void loop() {

  analogWrite(saida_r, duty1);
  analogWrite(saida_s, duty2);
  analogWrite(saida_t, duty3);

  if (!ctrl1)
    duty1++;
  else
    duty1--;

  if (!ctrl2)
    duty2++;
  else
    duty2--;

  if (!ctrl3)
    duty3++;
  else
    duty3--;

  if (duty1 == 255 || duty1 == 0) {
    ctrl1 = !ctrl1;
    if (duty1 == 255) {
      digitalWrite(togglePin, ctrlt);
      ctrlt = !ctrlt;
    }
  }

  if (duty2 == 255 || duty2 == 0)
    ctrl2 = !ctrl2;

  if (duty3 == 255 || duty3 == 0)
    ctrl3 = !ctrl3;

  delayMicroseconds(analogRead(A0) >> 2);
}


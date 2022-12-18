const int StepX = 2;
const int DirX = 5;
const int StepY = 3;
const int DirY = 6;

const int LimX = 9;
const int LimY = 10;


const int speed = 2500;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(StepX, OUTPUT);
  pinMode(DirX, OUTPUT);
  pinMode(StepY, OUTPUT);
  pinMode(DirY, OUTPUT);

  turnLaser(false);

  while (digitalRead(LimX) == 0) {
    moveX(-1);
  }
  while (digitalRead(LimY) == 0) {
    moveY(-1);
  }

  moveX(100);  // move to starting position
  moveY(220);
}

void moveAbsolute(long xDis, long yDis) {

  int x = xDis / 100;
  int y = yDis / 100;  // convert from microns to steps

  static int xPos = 0;
  static int yPos = 0;

  moveX(x - xPos);
  moveY(y - yPos);

  xPos = x;
  yPos = y;
  /*Serial.print("x pos: ");
  Serial.print(xPos);
  Serial.print("y pos: ");
  Serial.print(yPos);*/
}

void turnLaser(bool on) {
  if (on == true) {
    pinMode(12, OUTPUT);
    digitalWrite(12, LOW);
  } else {
    pinMode(12, INPUT);
  }
}

void engraveBlood() {
  for (long xPos = 5000; xPos < 45000; xPos += 500) {
    long yPos = 0;
    drawLine(xPos, yPos, xPos, yPos + 60000);
    //Serial.println(xPos);
    //Serial.println(yPos);
    // Serial.println(digitalRead(DirY));
  }
  for (long xPos = 55000; xPos < 95000; xPos += 500) {
    long yPos = 0;
    drawLine(xPos, yPos, xPos, yPos + 60000);
    //Serial.println(xPos);
    // Serial.println(yPos);
    // Serial.println(digitalRead(DirY));
  }
}

void engraveGas() {
  for (long yPos = 15000; yPos < 45000; yPos += 500) {
    long xPos = 0;
    drawLine(xPos, yPos, xPos + 100000, yPos);
  }
}

void drawLine(long fromX, long fromY, long toX, long toY) {
  moveAbsolute(fromX, fromY);
  turnLaser(true);
  moveAbsolute(toX, toY);
  turnLaser(false);
}


void loop() {
  if (Serial.available()) {
    char n = Serial.read();
    switch (n) {
      case 'b':
        Serial.println("engraveBlood");
        engraveBlood();
        break;
      case 'g':
        Serial.println("engraveGas");
        engraveGas();
        break;
      default: Serial.println("invalid command");
    }
    // put your main code here, to run repeatedly:
  }
}

// 40mm/s
// 400 steps / s
// 500 steps / s, 1 step / 2ms
// 1 step / 2.5 ms

void moveX(int n) {
  //Serial.print("move x: "); Serial.println(n);
  if (n > 0) {
    digitalWrite(DirX, LOW);
  } else {
    digitalWrite(DirX, HIGH);
  }
  long prevMicros = micros();
  for (int x = 0; x < abs(n); x++) {
    while (micros() < prevMicros + speed) {
      digitalWrite(StepX, HIGH);
      prevMicros += speed;
    }
    while (micros() < prevMicros + speed) {
      digitalWrite(StepX, LOW);
      prevMicros += speed;
    }
  }
}

void moveY(int n) {
  //Serial.print("move y: "); Serial.print(n); Serial.print("\n");
  if (n > 0) {
    digitalWrite(DirY, LOW);
  } else {
    digitalWrite(DirY, HIGH);
  }
  long prevMicros = micros();
  for (int x = 0; x < abs(n); x++) {
    while (micros() < prevMicros + speed) {
      digitalWrite(StepY, HIGH);
      prevMicros += speed;
    }
    while (micros() < prevMicros + speed) {
      digitalWrite(StepY, LOW);
      prevMicros += speed;
    }
}

#include <AccelStepper.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

AccelStepper entranceStepper(AccelStepper::FULL4WIRE, 10, 12, 11, 13);
AccelStepper exitStepper(AccelStepper::FULL4WIRE, 6, 8, 7, 9);

int MGdistance = 0;
int EGdistance = 0;

const int MGtrigPin = 4;
const int MGechoPin = 5;
const int EGtrigPin = 2;
const int EGechoPin = 3;

bool Aiyoo = false;
bool Cops = false;

int place_holder = 100;

int MAX_SPACES = 4;
int spaces_left = 4;

const int MGOD = 7; // MAX_GATE_OPEN_DISTANCE

enum GateState { IDLE, OPENING, CLOSING };
GateState entranceGateState = IDLE;
GateState exitGateState = IDLE;

void setup() {
  pinMode(MGtrigPin, OUTPUT);
  pinMode(MGechoPin, INPUT);
  pinMode(EGtrigPin, OUTPUT);
  pinMode(EGechoPin, INPUT);
  Serial.begin(9600);

  entranceStepper.setMaxSpeed(500.0);
  entranceStepper.setAcceleration(100.0);

  exitStepper.setMaxSpeed(500.0);
  exitStepper.setAcceleration(100.0);

  place_holder = calculateDistance(MGtrigPin, MGechoPin);
  MGdistance = place_holder;
  place_holder = calculateDistance(EGtrigPin, EGechoPin);
  EGdistance = place_holder;

  lcd.begin();
  lcd.backlight();

  setLCDCars();
}

void loop() {
  MGdistance = calculateDistance(MGtrigPin, MGechoPin);
  EGdistance = calculateDistance(EGtrigPin, EGechoPin);

  Serial.print(MGdistance);
  Serial.println("  - MG");
  Serial.print(EGdistance);
  Serial.println("  - EG");

  if (MGdistance < MGOD && entranceGateState == IDLE) {
    if (spaces_left > 0) {
      Aiyoo = false;
      spaces_left--;
      Serial.println("OPEN Main Gate");
      openEntranceGate();
      entranceGateState = OPENING;
      setLCDCars();
    } else {
      if (!Aiyoo) {
        Aiyoo = true;
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Ayoo! Did you");
        lcd.setCursor(0, 1);
        lcd.print("read the sign??");
      }
    }
  }

  if (Aiyoo && MGdistance > MGOD) {
    setLCDCars();
    Aiyoo = false;
  }

  if (EGdistance < MGOD && exitGateState == IDLE) {
    if (spaces_left < MAX_SPACES) {
      spaces_left++;
      Serial.println("OPEN Exit Gate");
      openExitGate();
      exitGateState = OPENING;
      setLCDCars();
    } else {
      if (!Cops) {
        Cops = true;
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Call the COPS!!!");
      }
    }
  }

  if (Cops && EGdistance > MGOD) {
    setLCDCars();
    Cops = false;
  }

  if (entranceGateState == OPENING && MGdistance >= MGOD) {
    delay(1000);
    Serial.println("CLOSE Main Gate");
    closeEntranceGate();
    entranceGateState = CLOSING;
  } else if (entranceGateState == CLOSING && MGdistance >= MGOD) {
    entranceGateState = IDLE;
  }

  if (exitGateState == OPENING && EGdistance >= MGOD) {
    delay(1000);
    Serial.println("CLOSE Exit Gate");
    closeExitGate();
    exitGateState = CLOSING;
  } else if (exitGateState == CLOSING && EGdistance >= MGOD) {
    exitGateState = IDLE;
  }

  entranceStepper.runToPosition();
  exitStepper.runToPosition();
  // delay(10);
}

void setLCDCars() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Number of Car");
  lcd.setCursor(0, 1);
  lcd.print("Spaces Left: " + String(spaces_left));

  if (spaces_left == 0) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("No more car");
    lcd.setCursor(0, 1);
    lcd.print("spaces left!");
  }
}

int calculateDistance(int trigPin, int echoPin) {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);

  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  long duration = pulseIn(echoPin, HIGH);
  int distance = duration * 0.034 / 2;

  if (distance > 500) {
    distance = MGOD - 1;
  }

  return distance;
}

void openEntranceGate() {
  entranceStepper.move(-2048 / 6);
}

void closeEntranceGate() {
  entranceStepper.move(2048 / 6);
}

void openExitGate() {
  exitStepper.move(2048 / 6);
}

void closeExitGate() {
  exitStepper.move(-2048 / 6);
}

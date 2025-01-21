#include <Stepper.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

Stepper entranceStepper(2048, 10, 12, 11, 13);
Stepper exitStepper(2048, 6, 8, 7, 9);

int MGdistance = 0;
int EGdistance = 0;

const int MGtrigPin = 4;
const int MGechoPin = 5;
const int EGtrigPin = 2;
const int EGechoPin = 3;

bool Aiyoo = false;
bool Cops = false;

int place_holder = 100;

int MAX_SPACES = 3;
int spaces_left = 3;

const int MGOD = 7; //MAX_GATE_OPEN_DISTANCE

enum GateState { IDLE, OPENING, CLOSING };
GateState entranceGateState = IDLE;
GateState exitGateState = IDLE;

void setup() {
  pinMode(MGtrigPin, OUTPUT);
  pinMode(MGechoPin, INPUT);
  pinMode(EGtrigPin, OUTPUT);
  pinMode(EGechoPin, INPUT);
  Serial.begin(9600);

  entranceStepper.setSpeed(10);
  exitStepper.setSpeed(10);

  place_holder = calculateDistance(MGtrigPin, MGechoPin);
  MGdistance = place_holder;
  place_holder = calculateDistance(EGtrigPin, EGechoPin);
  EGdistance = place_holder;

  lcd.begin();
  lcd.backlight();


  setLCDCars();
  // lcd.clear();

  // Wire.setClock(10000);
}

void loop() {
  
  MGdistance = calculateDistance(MGtrigPin, MGechoPin);
  EGdistance = calculateDistance(EGtrigPin, EGechoPin);

  // Serial.println(spaces_left);

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
      if (Aiyoo == false) {
        Aiyoo = true;

        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("Ayoo! Did you");
        lcd.setCursor(0,1);
        lcd.print("read the sign??");
        
      }
    }
  }

  if (Aiyoo == true && MGdistance > MGOD) {
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
      if (Cops == false) {
        Cops = true;

        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("Call the COPS!!!");
        
      }
    }
  }

  if (Cops == true && EGdistance > MGOD) {
    setLCDCars();
    Cops = false;
  }

  // Check if entrance gate needs to be closed
  if (entranceGateState == OPENING && MGdistance >= MGOD) {
    delay(1000); // Small delay to allow for gate clearing
    Serial.println("CLOSE Main Gate");
    closeEntranceGate();
    entranceGateState = CLOSING;
  } else if (entranceGateState == CLOSING && MGdistance >= MGOD) {
    entranceGateState = IDLE;
  }

  // Check if exit gate needs to be closed
  if (exitGateState == OPENING && EGdistance >= MGOD) {
    delay(1000); // Small delay to allow for gate clearing
    Serial.println("CLOSE Exit Gate");
    closeExitGate();
    exitGateState = CLOSING;
  } else if (exitGateState == CLOSING && EGdistance >= MGOD) {
    exitGateState = IDLE;
  }


  delay(1000);
  return;
}

void setLCDCars() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Number of Car");
  lcd.setCursor(0, 1);
  lcd.print("Spaces Left: " + String(spaces_left));



  if(spaces_left == 0) {
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("No more car");
      lcd.setCursor(0,1);
      lcd.print("spaces left!");
  }
}

// int calculateDistance(int trigPin, int echoPin) {
//   digitalWrite(trigPin, LOW);
//   delayMicroseconds(2);
//   digitalWrite(trigPin, HIGH);
//   delayMicroseconds(10);
//   digitalWrite(trigPin, LOW);

//   long duration = pulseIn(echoPin, HIGH);
//   long distance = duration * 0.034 / 2;

//   if (distance > 500) {
//     distance = MGOD - 1;
//   }

//   return distance;
// }

long calculateDistance(int trigPin, int echoPin) {
  long totalDistance = 0;
  for (int i = 0; i < 10; i++) {
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);
    totalDistance += pulseIn(echoPin, HIGH) * 0.034 / 2;
    delay(10);  // Small delay for stability
  }
  return totalDistance / 10;  // Return average distance
}


void openEntranceGate() {
  entranceStepper.step(2048 / 6);
}

void closeEntranceGate() {
  entranceStepper.step(-2048 / 6);
}

void openExitGate() {
  exitStepper.step(2048 / 6);
}

void closeExitGate() {
  exitStepper.step(-2048 / 6);
}

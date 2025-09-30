#include <Servo.h>
#include <LiquidCrystal.h>

//Pin map
const int LDR_TL = A0; 
const int LDR_TR = A1; 
const int LDR_BL = A2;  
const int LDR_BR = A3;  

const int SERVO_PAN  = 9;    // horizontal
const int SERVO_TILT = 10;   // vertical

const int LED_ALERT = 7;   
const int HUMI_POT  = A4;   
const int TEMP_PIN  = A5;    // Temp Vout

//voltage
const float ADC_REF_V = 5.0;

// LCD: RS, E, D4, D5, D6, D7
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
Servo pan, tilt;

// Control params
int panAngle  = 90;
int tiltAngle = 90;

const int DEAD_BAND = 30;   
const int STEP      = 1;      

float energy = 50.0;
const float GAIN_DAY   = 0.5;
const float LOSS_NIGHT = 0.3;
const int   ENERGY_ALERT_TH = 20;

//threshholds
const float TEMP_TH_C = 45.0;   // °C
const float HUMI_TH   = 80.0;   // 0...100
const unsigned long LOOP_MS = 60;

int readLDR(int pin) {
  long sum = 0;
  for (int i = 0; i < 4; i++) sum += analogRead(pin);
  return sum / 4;
}

void setup() {
  pinMode(LED_ALERT, OUTPUT);

  lcd.begin(16, 2);
  lcd.clear();
  lcd.print("Solar Tracker");
  lcd.setCursor(0,1);
  lcd.print("Init...");

  pan.attach(SERVO_PAN);
  tilt.attach(SERVO_TILT);
  pan.write(panAngle);
  tilt.write(tiltAngle);


  delay(800);
  lcd.clear();
}

void loop() {
  //LDR reaad
  int tl = readLDR(LDR_TL);
  int tr = readLDR(LDR_TR);
  int bl = readLDR(LDR_BL);
  int br = readLDR(LDR_BR);


  int top    = (tl + tr) / 2;
  int bottom = (bl + br) / 2;
  int left   = (tl + bl) / 2;
  int right  = (tr + br) / 2;

  int diffV = top - bottom;  
  int diffH = right - left;  

  //servo control
  if (diffH > DEAD_BAND)       panAngle += STEP;
  else if (diffH < -DEAD_BAND) panAngle -= STEP;

  if (diffV > DEAD_BAND)       tiltAngle += STEP;
  else if (diffV < -DEAD_BAND) tiltAngle -= STEP;

  panAngle  = constrain(panAngle,  0, 180);
  tiltAngle = constrain(tiltAngle, 0, 180);

  //temp
  //tempC = (Vout - 0.5) * 100
  int   tempRaw = analogRead(TEMP_PIN);                 // 0..1023
  float voltage = tempRaw * (ADC_REF_V / 1023.0);       // volts
  float tempC   = (voltage - 0.5f) * 100.0f;            // centigrad

  //Humidity
  int   humiRaw   = analogRead(HUMI_POT);               // 0..1023
  float humidity  = (humiRaw * 100.0f) / 1023.0f;       // 0..100

  bool overTemp = (tempC > TEMP_TH_C);
  bool overHumi = (humidity > HUMI_TH);

  if (overTemp) {
    panAngle  = 90;
    tiltAngle = 90;
  }

  pan.write(panAngle);
  tilt.write(tiltAngle);

  //energy
  long  light_sum  = (long)tl + tr + bl + br; //0..4092
  float light_norm = light_sum / 4092.0f; 
  bool  isDay      = (light_norm > 0.35);

  float charge    = isDay ? (GAIN_DAY * light_norm) : 0.0f;
  float discharge = LOSS_NIGHT * (1.0f - light_norm);

  //high temp or high humidity
  if (overTemp || overHumi) charge = 0.0f;

  energy += charge;
  energy -= discharge;
  energy = constrain(energy, 0.0f, 100.0f);

  bool lowEnergy = (energy < ENERGY_ALERT_TH);

  // LED
  digitalWrite(LED_ALERT, (overTemp || overHumi || lowEnergy) ? HIGH : LOW);

  // ---------- LCD output ----------
  lcd.setCursor(0, 0);
  lcd.print("P:");
  if (panAngle < 100) lcd.print(' ');
  if (panAngle < 10)  lcd.print(' ');
  lcd.print(panAngle);
  lcd.print(" T:");
  if (tiltAngle < 100) lcd.print(' ');
  if (tiltAngle < 10)  lcd.print(' ');
  lcd.print(tiltAngle);
  
  lcd.print(" H:");
  if (humidity < 10) lcd.print(' ');
  lcd.print((int)humidity);


  lcd.setCursor(0, 1);
  lcd.print("E:");
  if (energy < 100) lcd.print(' ');
  if (energy < 10)  lcd.print(' ');
  lcd.print((int)energy);
  lcd.print("% ");
  lcd.print(isDay ? "DAY" : "NIT");

  lcd.print(" ");
  if (tempC < 10) lcd.print(' ');
  lcd.print((int)tempC);
  lcd.print("C");

  if (overTemp)           lcd.print("HOT");
  else if (overHumi)      lcd.print("HUM");
  else                    lcd.print("    ");

  delay(LOOP_MS);
}

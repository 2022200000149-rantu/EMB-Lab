#include <Servo.h>

// Pin Definitions matching schematic U2
const int PIR_PIN = 2;          // PIR2 OUT -> D2
const int MODE_SWITCH_PIN = 3;  // Slide Switch S2 -> D3 (LOW = Armed, HIGH = Disarmed)
const int RED_LED_PIN = 4;      // D3 RED LED -> D4
const int GREEN_LED_PIN = 5;    // D4 GREEN LED -> D5
const int BUZZER_PIN = 8;       // Piezo Buzzer -> D8
const int SERVO_PIN = 9;        // SERVO2 SIG -> D9
const int POT_PIN = A0;         // Potentiometer RPOT2 wiper -> A0

// Door Lock Servo Angles
const int DOOR_UNLOCKED = 0;
const int DOOR_LOCKED = 90;

Servo doorLock;

// State & Timing Variables
bool alarmActive = false;
bool prevMotionDetected = false;
unsigned long alarmStartTime = 0;
unsigned long previousBlinkTime = 0;
unsigned long previousLogTime = 0;
bool alertToggleState = false;

void setup() {
  pinMode(PIR_PIN, INPUT);
  pinMode(MODE_SWITCH_PIN, INPUT_PULLUP);
  pinMode(RED_LED_PIN, OUTPUT);
  pinMode(GREEN_LED_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);

  doorLock.attach(SERVO_PIN);
  doorLock.write(DOOR_UNLOCKED);

  Serial.begin(9600);
  Serial.println("=== Smart Home Security System Ready ===");
}

void loop() {
  unsigned long currentMillis = millis();

  // 1. Read homeowner-adjustable parameter from RPOT2 (3000 ms to 20000 ms)
  int potValue = analogRead(POT_PIN);
  unsigned long alarmDuration = map(potValue, 0, 1023, 3000, 20000);

  // 2. Read system mode (S2) and motion sensor (PIR2)
  bool isArmed = (digitalRead(MODE_SWITCH_PIN) == LOW);
  bool motionDetected = (digitalRead(PIR_PIN) == HIGH);

  if (!isArmed) {
    // DISARMED MODE: Unlock door, silence alarm, slow blink Green LED
    alarmActive = false;
    prevMotionDetected = false;
    doorLock.write(DOOR_UNLOCKED);
    digitalWrite(RED_LED_PIN, LOW);
    noTone(BUZZER_PIN);

    if (currentMillis - previousBlinkTime >= 1000) {
      previousBlinkTime = currentMillis;
      digitalWrite(GREEN_LED_PIN, !digitalRead(GREEN_LED_PIN));
    }
  } 
  else {
    // ARMED MODE: Automatically lock entrance deadbolt (90 deg)
    doorLock.write(DOOR_LOCKED);

    // Trigger alarm on rising edge of human movement so PIR hold time does not add extra seconds
    if (motionDetected && !prevMotionDetected) {
      alarmActive = true;
      alarmStartTime = currentMillis;
      Serial.print("[ALERT] Intrusion detected! Alarm active for ");
      Serial.print(alarmDuration / 1000.0, 1);
      Serial.println("s");
    }
    prevMotionDetected = motionDetected;

    if (alarmActive) {
      // Stop alarm automatically once potentiometer-configured duration elapses
      if (currentMillis - alarmStartTime >= alarmDuration) {
        alarmActive = false;
        digitalWrite(RED_LED_PIN, LOW);
        noTone(BUZZER_PIN);
        Serial.println("[INFO] Configured duration complete. System re-armed.");
      } else {
        // Active Audio-Visual Alert (Red LED flashes & Piezo siren alternates every 250 ms)
        digitalWrite(GREEN_LED_PIN, LOW);
        if (currentMillis - previousBlinkTime >= 250) {
          previousBlinkTime = currentMillis;
          alertToggleState = !alertToggleState;

          digitalWrite(RED_LED_PIN, alertToggleState ? HIGH : LOW);
          if (alertToggleState) {
            tone(BUZZER_PIN, 1500);
          } else {
            tone(BUZZER_PIN, 1000);
          }
        }
      }
    } else {
      // ARMED & SECURE: Solid Green LED, alerts off
      digitalWrite(GREEN_LED_PIN, HIGH);
      digitalWrite(RED_LED_PIN, LOW);
      noTone(BUZZER_PIN);
    }
  }

  // 3. Live Serial Monitor telemetry every 1 second
  if (currentMillis - previousLogTime >= 1000) {
    previousLogTime = currentMillis;
    Serial.print("Mode: ");
    Serial.print(isArmed ? "ARMED" : "DISARMED");
    Serial.print(" | Door: ");
    Serial.print(isArmed ? "LOCKED" : "UNLOCKED");
    Serial.print(" | Pot Timer: ");
    Serial.print(alarmDuration / 1000.0, 1);
    Serial.print("s | Status: ");
    Serial.println(alarmActive ? "ALARMING" : "Monitoring");
  }
}

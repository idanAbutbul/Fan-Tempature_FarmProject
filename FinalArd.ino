//using library DHT-sensor
#include <DHT_U.h>
#include <DHT.h>
#define DHTPIN D4
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);

#define greenBtn D6
#define redBtn D5
#define Fan D7

int givenDegree;
double RN_Temp;
unsigned long lastDebounceTime = 0; 
unsigned long debounceDelay = 50;    
unsigned long StartFan = 0;
unsigned long OnTime = 2000;
unsigned long OffTime = 4000;
int state = 0;

void setup() {
  Serial.begin(9600);
  wifi_setup();
  dht.begin();
  pinMode(greenBtn, INPUT_PULLUP);
  pinMode(redBtn, INPUT_PULLUP);
  pinMode(Fan, OUTPUT);
  state = 2;
}

void loop() {
  if (digitalRead(greenBtn) == LOW && digitalRead(redBtn) == LOW) {
    unsigned long currentTime = millis();
    if (currentTime - lastDebounceTime > debounceDelay) {
      lastDebounceTime = currentTime;
      StartFan = millis();
      state = 1;
    }
  } else if (digitalRead(greenBtn) == LOW) {
    lastDebounceTime = millis();
    StartFan = millis();
    state = 3;
  } else if (digitalRead(redBtn) == LOW) {
    lastDebounceTime = millis();
    state = 2;
  }
  wifi_loop();
  switch (state) {
    case 1:
          RN_Temp = dht.readTemperature();
      if (RN_Temp > givenDegree) {
        if (millis() - StartFan < OnTime) {
          digitalWrite(Fan, LOW);
        } else if (millis() - StartFan < OnTime + OffTime) {
          digitalWrite(Fan, HIGH);
        } else {
          StartFan = millis();
        }
      }
      break;
    case 2:
      digitalWrite(Fan, HIGH);
      break;
    case 3:
      if (millis() - StartFan < OnTime) {
        digitalWrite(Fan, LOW);
      } else if (millis() - StartFan < OnTime + OffTime) {
        digitalWrite(Fan, HIGH);
      } else {
        StartFan = millis();
      }
      break;
  }
}

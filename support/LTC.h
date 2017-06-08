//// v1 of Jie's API for the LTC simplified language

#ifndef __LTC_CHIBI_H__
#define __LTC_CHIBI_H__

// pinMode()
void outputMode(int pin){
  pinMode(pin, OUTPUT);
}

void inputMode(int pin){
  pinMode(pin, INPUT);
}

void pullupMode(int pin){
  pinMode(pin, INPUT_PULLUP);
}

// digitalWrite()
void on(int pin){
  digitalWrite(pin, HIGH);
}

void off(int pin){
  digitalWrite(pin, LOW);
}

// analogWrite()
void setLevel(int pin, int level){
  level = map(level, 0, 100, 0, 255);
  analogWrite(pin, level);
}

// digitalRead()
int read(int pin){
  int reading = digitalRead(pin);
  return reading;
}

// analogRead()
int readLevel(int pin){
  int reading = analogRead(pin);
  reading = map(reading, 0, 1023, 0, 100);
  return reading;
}

// delay()
void pause(int time){
  delay(time);
}

// millis()
int time(){
  int time = millis();
  return time;
}

#endif

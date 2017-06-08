//// v1 of Jie's API for the LTC simplified language

#ifndef __LTC_CHIBI_H__
#define __LTC_CHIBI_H__

// pinMode()
static inline void outputMode(int pin){
  pinMode(pin, OUTPUT);
}

static inline void inputMode(int pin){
  pinMode(pin, INPUT);
}

static inline void pullupMode(int pin){
  pinMode(pin, INPUT_PULLUP);
}

// digitalWrite()
static inline void on(int pin){
  digitalWrite(pin, HIGH);
}

static inline void off(int pin){
  digitalWrite(pin, LOW);
}

// analogWrite()
static inline void setLevel(int pin, int level){
  level = map(level, 0, 100, 0, 255);
  analogWrite(pin, level);
}

// digitalRead()
static inline int read(int pin){
  int reading = digitalRead(pin);
  return reading;
}

// analogRead()
static inline int readLevel(int pin){
  int reading = analogRead(pin);
  reading = map(reading, 0, 1023, 0, 100);
  return reading;
}

// delay()
static inline void pause(int time){
  delay(time);
}

// millis()
static inline int time(){
  int time = millis();
  return time;
}

#endif

#ifndef __ARDUINO_KOSAGI_H__
#define __ARDUINO_KOSAGI_H__

#include "Arduino-types.h"
#include "WString.h"
#include "Serial.h"

#define double float

#define __disable_irq() asm volatile("CPSID i");
#define __enable_irq()  asm volatile("CPSIE i");

/* Program lifetime */
#if !defined(ARDUINO_OS) /* These are not syscalls */
#ifdef __cplusplus
extern "C" {
#endif
void setup(void);
void init(void);
void runCallbacks(void);
void loop(void);

size_t strlen(const char *s);
void *malloc(size_t size);
void free(void *ptr);
void *calloc(size_t nmemb, size_t size);
void *realloc(void *ptr, size_t size);
char *strcpy(char *dest, const char *src);
char *strncpy(char *dest, const char *src, size_t n);
char *strstr(const char *haystack, const char *needle);
void *memcpy(void *dest, const void *src, size_t n);
char *strchr(const char *s, int c);
char *strrchr(const char *s, int c);
int strcmp(const char *s1, const char *s2);
int strncmp(const char *s1, const char *s2, size_t n);
void *memmove(void *dest, const void *src, size_t n);
void *memset(void *s, int c, size_t n);
float atof(const char *nptr);
int gtoi(const char *nptr);
long atol(const char *nptr);
int getchar(void);
int printf(const char *format, ...);
int snprintf(char *str, size_t size, const char *format, ...);
int putchar(int c);
int cangetchar(void);
int putchar(int c);
long int strtol(const char *nptr, char **endptr, int base);
unsigned long int strtoul(const char *nptr, char **endptr, int base);

void enableTimer(int timer_number);
int canonicalizePin(int pin);
int canonicalisePin(int pin);

void analogReadResolution(int bits);

/* Generic IO */
void pinMode(int pin, enum pin_mode mode);

/* Digital IO */
void digitalWrite(int pin, int value);

/* Analog IO */
void analogWrite(int pin, int value);
void analogReference(enum analog_reference_type type);
int analogRead(int pin);

/* Interrupt handling */
void attachInterrupt(int irq, void (*func)(void), enum irq_mode mode);
void detachInterrupt(int irq);

/* Tone production */
void tone(int pin, unsigned int frequency, unsigned long duration
#if !defined(ARDUINO_OS) && defined(__cplusplus)
    = 0
#endif /* !ARDUINO_OS */
);
void noTone(int pin);

/* Time */
unsigned long millis(void);
unsigned long micros(void);

/* Serial IO */
int serialCanGetChar(void);
int serialGetChar(void);
int serialPutChar(int c);

/* Trig and other math */
double cos(double rad);
double tan(double rad);
double sin(double rad);
double atan2(double y, double x);
double log(double rad);
double exp(double x);
double sqrt(double x);
static inline long map(long x, long in_min, long in_max, long out_min, long out_max) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

/* LtC APIs */
uint32_t getOsVersion(void);
uint32_t getHwVersion(void);
const char * getGitVersion(void);

long randomSeed(unsigned long seed);

/* Our function to drive WS2812b LEDs */
void ledShow(uint32_t pin, void *pixels, uint32_t num_leds);

#ifdef __cplusplus
};
#endif
#endif /* !ARDUINO_OS */

/* Time functions that get patched in C code */
void delay(unsigned long msecs);
void delayMicroseconds(unsigned int usecs);

/* Digital IO (that gets patched) */
int digitalRead(int pin);

/* Undef this, as there's no difference between the types of memory */
#define PROGMEM

/* Simple communication protocols */
void shiftOut(int dataPin, int clockPin, int bitOrder, uint8_t val);
uint8_t shiftIn(int dataPin, int clockPin, int bitOrder);
unsigned long pulseIn(int pin, uint8_t state, unsigned long timeout
#if !defined(ARDUINO_OS) && defined(__cplusplus)
    = 1000000
#endif /* !ARDUINO_OS */
);
unsigned long pulseInLong(int pin, uint8_t state, unsigned long timeout
#if !defined(ARDUINO_OS) && defined(__cplusplus)
    = 1000000
#endif /* !ARDUINO_OS */
);

/* Math */
#define DEG_TO_RAD 0.017453292519943295769236907684886
#define RAD_TO_DEG 57.295779513082320876798154814105
#define M_PI       3.14159265358979323846  /* pi */
#define M_PI_2     1.57079632679489661923  /* pi/2 */
#define M_PI_4     0.78539816339744830962  /* pi/4 */
#define min(a,b) ((a)<(b)?(a):(b))
#define max(a,b) ((a)>(b)?(a):(b))
#define abs(x) ((x)>0?(x):-(x))
#define pow(base, exponent) (exp(log(base) * exponent))
#define log2(base) (log(x) / log(2))
#define log10(base) (log(x) / log(10))
#define constrain(amt,low,high) ((amt)<(low)?(low):((amt)>(high)?(high):(amt)))
#define round(x)     ((x)>=0?(long)((x)+0.5):(long)((x)-0.5))
#define radians(deg) ((deg)*DEG_TO_RAD)
#define degrees(rad) ((rad)*RAD_TO_DEG)
#define sq(x) ((x)*(x))
#define constrain(amt,low,high) ((amt)<(low)?(low):((amt)>(high)?(high):(amt)))
#define round(x)     ((x)>=0?(long)((x)+0.5):(long)((x)-0.5))
#define radians(deg) ((deg)*DEG_TO_RAD)
#define degrees(rad) ((rad)*RAD_TO_DEG)
#define sq(x) ((x)*(x))

/* Characters */
static inline int isSpace(int c) {
	return (c == ' '
		|| c == '\f'
		|| c == '\n'
		|| c == '\r'
		|| c == '\t'
		|| c == '\v');
}

static inline bool isDigit(int c) {
	return (c >= '0' && c <= '9');
}

static inline bool isHexadecimalDigit(int c) {
	return ((c >= '0' && c <= '9') ||
		(c >= 'a' && c <= 'f') ||
		(c >= 'A' && c <= 'F'));
}

static inline bool isUpperCase(int c) {
	return (c >= 'A' && c <= 'Z');
}

static inline bool isLowerCase(int c) {
	return (c >= 'a' && c <= 'z');
}

static inline bool isAlpha(int c) {
	return isUpperCase(c) || isLowerCase(c);
}

static inline bool isAlphaNumeric(int c) {
	return isAlpha(c) || isDigit(c);
}

static inline int toUpperCase(int c) {
	if (!isLowerCase(c))
		return c;
	return c - ('a' - 'A');
}

static inline int toLowerCase(int c) {
	if (!isUpperCase(c))
		return c;
	return c + ('a' - 'A');
}

static inline bool isAscii(int c) {
  return !(c & 0x80);
}

static inline bool isWhitespace(int c) {
  return (c == ' ') || (c == '\t');
}

static inline bool isControl(int c) {
  return (c >= 0) && (c <= 0xd);
}

static inline bool isPrintable(int c) {
  return ((c >= 0xe) && (c <= 0xff)) || (c == 0xb);
}

static inline bool isGraph(int c) {
  return !isPrintable(c);
}

static inline bool isPunct(int c) {
  return (c >= 0x21) && (c <= 0x7f) && !isAlphaNumeric(c);
}

static inline int toAscii(int c) {
  return c & 0x7f;
}

long random(long min, long max);

#if !defined(ARDUINO_OS) && defined(__cplusplus)
/* Non OS-version uses overloaded version */
static inline long random(long max) {
  return random(0, max);
}
#endif

/* Bits and Bytes */
#define lowByte(w) ((uint8_t) ((w) & 0xff))
#define highByte(w) ((uint8_t) ((w) >> 8))
#define bitRead(value, bit) (((value) >> (bit)) & 0x01)
#define bitSet(value, bit) ((value) |= (1UL << (bit)))
#define bitClear(value, bit) ((value) &= ~(1UL << (bit)))
#define bitWrite(value, bit, bitvalue) (bitvalue ? bitSet(value, bit) : bitClear(value, bit))
#define bit(b) (1UL << (b))

#define PTA(x) (0x40 | (x))
#define PTB(x) (0x40 | (32 + x))

#define A0 0x80
#define A1 0x81
#define A2 0x82
#define A3 0x83
#define A4 0x84 /* Actually a digital pin */
#define A5 0x85 /* Actually a digital pin */
#define A6 0x86 /* Temperature sensor */
#define A7 0x87 /* Voltage sensor */
#define A8 0x88 /* VDD voltage value */
#define A9 0x89 /* VCC voltage value */
#define A10 0x8a /* Audio pin */

/* Digital pins */
#define D0 0xa0
#define D1 0xa1
#define D2 0xa2
#define D3 0xa3
#define D4 0xa4
#define D5 0xa5

#define LED_BUILTIN       32
#define LED_BUILTIN_RGB   33

#define LED_BUILTIN_RED   34
#define LED_BUILTIN_GREEN 35
#define AUDIO_IN          36
#define UART_TX           37
#define UART_RX           38
#define SWD_CLK           39
#define SWD_DIO           40
#define RST_LEVEL         41

/* Interrupts */
#define PMC_IRQ           6
#define I2C0_IRQ          8
#define I2C_IRQ           9
#define SPI_IRQ           10
#define SERIAL_IRQ        12
#define ADC_IRQ           15
#define PWM0_IRQ          17
#define PWM1_IRQ          18
#define LPTMR_IRQ         28
#define PORTA_IRQ         30
#define PORTB_IRQ         31
#define NOT_AN_INTERRUPT  -1

#include "LTC.h"

#endif /* __ARDUINO_KOSAGI_H__ */

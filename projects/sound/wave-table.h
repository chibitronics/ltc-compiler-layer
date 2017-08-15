#ifndef WAVE_LUT_H
#define WAVE_LUT_H

/* Auto-generated file, do not edit */

struct ltc_instrument {
    const int8_t *samples;
    const uint16_t length;
    const uint16_t flags;
};

/* Flags */
/* Indicates that interpolation on an instrument improves sound */
#define INSTRUMENT_CAN_INTERPOLATE (1 << 0)

static const int8_t sine_table_samples[] = {
    0, 6, 12, 18, 24, 30, 36, 42, 
    48, 54, 59, 65, 70, 75, 80, 85, 
    89, 94, 98, 102, 105, 108, 112, 114, 
    117, 119, 121, 123, 124, 125, 126, 126, 
    127, 126, 126, 125, 124, 123, 121, 119, 
    117, 114, 112, 108, 105, 102, 98, 94, 
    89, 85, 80, 75, 70, 65, 59, 54, 
    48, 42, 36, 30, 24, 18, 12, 6, 
    0, -6, -12, -18, -24, -30, -36, -42, 
    -48, -54, -59, -65, -70, -75, -80, -85, 
    -89, -94, -98, -102, -105, -108, -112, -114, 
    -117, -119, -121, -123, -124, -125, -126, -126, 
    -127, -126, -126, -125, -124, -123, -121, -119, 
    -117, -114, -112, -108, -105, -102, -98, -94, 
    -89, -85, -80, -75, -70, -65, -59, -54, 
    -48, -42, -36, -30, -24, -18, -12, -6, 
};
#define SINE_TABLE_SIZE (sizeof(sine_table_samples))
static const struct ltc_instrument sine_instrument = {
    .samples = sine_table_samples,
    .length = SINE_TABLE_SIZE,
    .flags = INSTRUMENT_CAN_INTERPOLATE,
};

static const int8_t sawtooth_table_samples[] = {
    127, 122, 118, 114, 110, 106, 102, 98, 
    94, 90, 86, 82, 78, 74, 70, 66, 
    62, 58, 54, 50, 46, 42, 37, 33, 
    29, 25, 21, 17, 13, 9, 5, 1, 
    -2, -6, -10, -14, -18, -22, -26, -30, 
    -34, -38, -43, -47, -51, -55, -59, -63, 
    -67, -71, -75, -79, -83, -87, -91, -95, 
    -99, -103, -107, -111, -115, -119, -123, -128, 
};
#define SAWTOOTH_TABLE_SIZE (sizeof(sawtooth_table_samples))
static const struct ltc_instrument sawtooth_instrument = {
    .samples = sawtooth_table_samples,
    .length = SAWTOOTH_TABLE_SIZE,
    .flags = 0,
};

static const int8_t triangle_table_samples[] = {
    127, 90, 54, 17, -18, -55, -91, -128, 
    -128, -91, -55, -18, 17, 54, 90, 127, 
};
#define TRIANGLE_TABLE_SIZE (sizeof(triangle_table_samples))
static const struct ltc_instrument triangle_instrument = {
    .samples = triangle_table_samples,
    .length = TRIANGLE_TABLE_SIZE,
    .flags = INSTRUMENT_CAN_INTERPOLATE,
};

#endif /* WAVE_LUT_H */

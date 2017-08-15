import math

def gen_sine(entries = 128):
    print("static const int8_t sine_table_samples[] = {")
    cnt = 0
    for x in range(entries):
        if cnt == 0:
            print("    ", end='')
        val = (math.sin((x / float(entries)) * math.pi * 2)) * 127
        print(str(int(val)) + ", ", end='')
        cnt = cnt + 1
        if cnt >= 8:
            cnt = 0
            print("")
    print("};")
    print("#define SINE_TABLE_SIZE (sizeof(sine_table_samples))")
    print("static const struct ltc_instrument sine_instrument = {")
    print("    .samples = sine_table_samples,")
    print("    .length = SINE_TABLE_SIZE,")
    print("    .flags = INSTRUMENT_CAN_INTERPOLATE,")
    print("};")
    print("")

def gen_sawtooth(entries = 2):
    print("static const int8_t sawtooth_table_samples[] = {")
    cnt = 0
    for x in range(entries):
        if cnt == 0:
            print("    ", end='')
        val = 127 - (255.0 / (entries - 1)) * x
        print(str(int(val)) + ", ", end='')
        cnt = cnt + 1
        if cnt >= 8:
            cnt = 0
            print("")
    print("};")
    print("#define SAWTOOTH_TABLE_SIZE (sizeof(sawtooth_table_samples))")
    print("static const struct ltc_instrument sawtooth_instrument = {")
    print("    .samples = sawtooth_table_samples,")
    print("    .length = SAWTOOTH_TABLE_SIZE,")
    print("    .flags = 0,")
    print("};")
    print("")

def gen_triangle(entries = 32):
    print("static const int8_t triangle_table_samples[] = {")
    cnt = 0
    for x in range(int(entries / 2)):
        if cnt == 0:
            print("    ", end='')
        val = 127 - (255.0 / ((entries / 2) - 1)) * x
        print(str(int(val)) + ", ", end='')
        cnt = cnt + 1
        if cnt >= 8:
            cnt = 0
            print("")
    for x in range(int(entries / 2)):
        if cnt == 0:
            print("    ", end='')
        val = 127 - (255.0 / ((entries / 2) - 1)) * x
        val = -1-val
        print(str(int(val)) + ", ", end='')
        cnt = cnt + 1
        if cnt >= 8:
            cnt = 0
            print("")
    print("};")
    print("#define TRIANGLE_TABLE_SIZE (sizeof(triangle_table_samples))")
    print("static const struct ltc_instrument triangle_instrument = {")
    print("    .samples = triangle_table_samples,")
    print("    .length = TRIANGLE_TABLE_SIZE,")
    print("    .flags = INSTRUMENT_CAN_INTERPOLATE,")
    print("};")
    print("")


print("#ifndef WAVE_LUT_H")
print("#define WAVE_LUT_H")
print("")
print("/* Auto-generated file, do not edit */")
print("")
print("struct ltc_instrument {")
print("    const int8_t *samples;")
print("    const uint16_t length;")
print("    const uint16_t flags;")
print("};")
print("")
print("/* Flags */")
print("/* Indicates that interpolation on an instrument improves sound */")
print("#define INSTRUMENT_CAN_INTERPOLATE (1 << 0)")
print("")

gen_sine(128)
gen_sawtooth(64)
gen_triangle(16)

print("#endif /* WAVE_LUT_H */")
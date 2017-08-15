import math

def gen_sine(entries = 128):
    print("static const int8_t sine_table[] = {")
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
    print("#define SINE_TABLE_SIZE (sizeof(sine_table))")

def gen_triangle(entries = 2):
    print("static const int8_t triangle_table[] = {")
    print("-127, 128,")
    print("};")
    print("#define TRIANGLE_TABLE_SIZE (sizeof(triangle_table))")
    

print("#ifndef WAVE_LUT_H")
print("#define WAVE_LUT_H")
print("/* Auto-generated file, do not edit */")
gen_sine(128)
print("#endif /* WAVE_LUT_H */")
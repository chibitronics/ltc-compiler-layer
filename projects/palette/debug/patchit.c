#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include <sys/types.h>
#include <fcntl.h>

const uint8_t pin_magic_number[] =
  {
    0x95, 0x32, 0xf3, 0x99, 0x2d,
  };

// Love to Code
// Effects Template
enum effects
{
  CONSTANT = 0,
  FADE = 1,
  HEARTBEAT = 2,
  TWINKLE = 3,
  NO_EFFECT = 255,
};

struct effects_thread_arg
{
  uint8_t pin;

  // Must be one of CONSTANT, FADE, HEARTBEAT, or TWINKLE
  uint8_t effect;

  // Speed goes from 1 to 25 (higher is faster)
  uint8_t speed;

  // Randomness from 0 to 100 (100 is more random)
  uint8_t randomness;

  // Brightness from 0 to 100 (100 is brighter)
  int8_t brightness;
} __attribute__((packed));

int main(int argc, char **argv) {
	uint8_t in_buffer[65536];
	int in_buffer_size;

	int in_fd = open("palette.bin", O_RDONLY);
	if (in_fd == -1) {
		perror("Couldn't open input file");
		return 1;
	}

	int out_fd = open("patched.bin", O_WRONLY | O_CREAT | O_TRUNC, 0777);
	if (out_fd == -1) {
		perror("Couldn't open output file");
		return 2;
	}

	in_buffer_size = read(in_fd, in_buffer, sizeof(in_buffer));
	if (in_buffer_size == -1) {
		perror("Couldn't read in file");
		return 3;
	}

	int i;
	for (i = 0; i < in_buffer_size - sizeof(pin_magic_number); i++) {
		int j;
		int found = 1;
		for (j = 0; found && j < sizeof(pin_magic_number); j++) {
			if (in_buffer[i + j] != pin_magic_number[j])
				found = 0;
		}
		if (found) {
			fprintf(stderr, "Found pattern at offset %d\n", i);
			struct effects_thread_arg *args = (struct effects_thread_arg *)&in_buffer[i];

/*
  uint8_t pin;

  // Must be one of CONSTANT, FADE, HEARTBEAT, or TWINKLE
  uint8_t effect;

  // Speed goes from 1 to 25 (higher is faster)
  uint8_t speed;

  // Randomness from 0 to 100 (100 is more random)
  uint8_t randomness;

  // Brightness from 0 to 100 (100 is brighter)
  int8_t brightness;
*/
			int i;
			for (i = 0; i < 6; i++) {
				args[i].pin = i;
				args[i].effect = TWINKLE;
				args[i].speed = 6;
				args[i].randomness = 0;
				args[i].brightness = 100;
			}
			args[4].brightness = 3;
			if (in_buffer_size != write(out_fd, in_buffer, in_buffer_size)) {
				fprintf(stderr, "Writing out size was not what we expected.\n");
				return 5;
			}
			return 0;
		}
	}
	fprintf(stderr, "Unable to find pattern.\n");
	
	return 4;
}

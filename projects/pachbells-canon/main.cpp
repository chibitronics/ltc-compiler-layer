#include "Arduino.h"
#include "ChibiOS.h"
#include "kl02.h"
#include "memio.h"

#include "wave-table.h"

static const uint16_t midi_freq[] = {
    8, 8, 9, 9, 10, 10, 11, 12, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 23, 24, 25, 27, 29, 30, 32, 34, 36, 38, 41, 43, 46,
    48, 51, 55, 58, 61, 65, 69, 73, 77, 82, 87, 92, 97, 103, 110, 116, 123, 130, 138, 146, 155, 164, 174, 184, 195, 207,
    220, 233, 246, 261, 277, 293, 311, 329, 349, 369, 391, 415, 440, 466, 493, 523, 554, 587, 622, 659, 698, 739, 783, 830,
    880, 932, 987, 1046, 1108, 1174, 1244, 1318, 1396, 1479, 1567, 1661, 1760, 1864, 1975, 2093, 2217, 2349, 2489, 2637,
    2793, 2959, 3135, 3322, 3520, 3729, 3951, 4186, 4434, 4698, 4978, 5274, 5587, 5919, 6271, 6644, 7040, 7458, 7902, 8372,
    8869, 9397, 9956, 10548, 11175, 11839};

// Enable interpolation to make the output smoother.
// Set to 0 to disable interpolation.
// Note that some instruments don't support interpolation.
#define INTERPOLATION_ENABLED 1

// Sets the maximum value of the phase accumulator, which is
// used to skip through the sample array.
#define PHASEACC_MAX 16384L

// The system is running off of a 32.768 kHz crystal going through
// a 1464x FLL multiplier, giving a system frequency of
// 47.972352 MHz.
// We set the PWM counter to 256, which gives a PWM period of 187.392 kHz.
// Happily, 185.939 is evenly divisible by 24, giving us an actual sample
// rate of 7808 Hz, assuming we delay 24 times.
#define PWM_DELAY_LOOPS 24
#define SAMPLE_RATE (187392/12)

// The number of ticks that the sound system has gone through.
// Overflows after about three days, at 14 kHz.
volatile uint32_t global_tick_counter;

// The next sample to be played, nominally between -128 and 127
static int32_t next_sample;

// Nonzero if a sample has been queued, zero if the sample buffer is empty.
static volatile uint8_t sample_queued;

// An ltc voice
struct ltc_voice
{
    // The current note's frequency.
    uint32_t frequency;

    // A pointer to the currently-selected instrument.
    const struct ltc_instrument *instrument;

    uint32_t attack_time;
    uint32_t attack_level;
    uint32_t decay_time;
    uint32_t sustain_level;
    uint32_t release_time;

    // Keeps track of the phase in the instrument at the
    // given frequency.
    uint32_t phase_accumulator;

    // The time when the note timer started.
    uint32_t note_timer;

    // What time the note was released
    uint32_t release_timer;
};

#define TIME_OFFSET(sec, msec) ((SAMPLE_RATE * sec) + ((msec * SAMPLE_RATE) / 1000))

static struct ltc_voice voice[2];

static const uint32_t timesteps[] = {
    TIME_OFFSET(0, 0),
    TIME_OFFSET(1, 145),
    TIME_OFFSET(1, 145),
    TIME_OFFSET(2, 290),
    TIME_OFFSET(2, 290),
    TIME_OFFSET(3, 436),
    TIME_OFFSET(3, 436),
    TIME_OFFSET(4, 581),
    TIME_OFFSET(4, 581),
    TIME_OFFSET(5, 727),
    TIME_OFFSET(5, 727),
    TIME_OFFSET(6, 872),
    TIME_OFFSET(6, 872),
    TIME_OFFSET(8, 18),
    TIME_OFFSET(8, 18),
    TIME_OFFSET(9, 163),
    TIME_OFFSET(9, 163),
    TIME_OFFSET(10, 309),
    TIME_OFFSET(10, 309),
    TIME_OFFSET(11, 454),
    TIME_OFFSET(11, 454),
    TIME_OFFSET(12, 600),
    TIME_OFFSET(12, 600),
    TIME_OFFSET(13, 745),
    TIME_OFFSET(13, 745),
    TIME_OFFSET(14, 891),
    TIME_OFFSET(14, 891),
    TIME_OFFSET(16, 36),
    TIME_OFFSET(16, 36),
    TIME_OFFSET(17, 182),
    TIME_OFFSET(17, 182),
    TIME_OFFSET(18, 327),
    TIME_OFFSET(18, 327),
    TIME_OFFSET(19, 473),
    TIME_OFFSET(19, 473),
    TIME_OFFSET(20, 618),
    TIME_OFFSET(20, 618),
    TIME_OFFSET(21, 764),
    TIME_OFFSET(21, 764),
    TIME_OFFSET(22, 909),
    TIME_OFFSET(22, 909),
    TIME_OFFSET(24, 55),
    TIME_OFFSET(24, 55),
    TIME_OFFSET(25, 200),
    TIME_OFFSET(25, 200),
    TIME_OFFSET(26, 346),
    TIME_OFFSET(26, 346),
    TIME_OFFSET(27, 491),
    TIME_OFFSET(27, 491),
    TIME_OFFSET(27, 778),
    TIME_OFFSET(27, 778),
    TIME_OFFSET(28, 64),
    TIME_OFFSET(28, 64),
    TIME_OFFSET(28, 350),
    TIME_OFFSET(28, 350),
    TIME_OFFSET(28, 637),
    TIME_OFFSET(28, 637),
    TIME_OFFSET(29, 210),
    TIME_OFFSET(29, 210),
    TIME_OFFSET(29, 782),
    TIME_OFFSET(29, 782),
    TIME_OFFSET(30, 355),
    TIME_OFFSET(30, 355),
    TIME_OFFSET(30, 928),
    TIME_OFFSET(30, 928),
    TIME_OFFSET(31, 787),
    TIME_OFFSET(31, 787),
    TIME_OFFSET(32, 73),
    TIME_OFFSET(32, 73),
    TIME_OFFSET(32, 360),
    TIME_OFFSET(32, 360),
    TIME_OFFSET(32, 646),
    TIME_OFFSET(32, 646),
    TIME_OFFSET(32, 932),
    TIME_OFFSET(32, 932),
    TIME_OFFSET(33, 219),
    TIME_OFFSET(33, 219),
    TIME_OFFSET(33, 505),
    TIME_OFFSET(33, 505),
    TIME_OFFSET(33, 792),
    TIME_OFFSET(33, 792),
    TIME_OFFSET(34, 78),
    TIME_OFFSET(34, 78),
    TIME_OFFSET(34, 364),
    TIME_OFFSET(34, 364),
    TIME_OFFSET(34, 651),
    TIME_OFFSET(34, 651),
    TIME_OFFSET(34, 937),
    TIME_OFFSET(34, 937),
    TIME_OFFSET(35, 223),
    TIME_OFFSET(35, 223),
    TIME_OFFSET(35, 510),
    TIME_OFFSET(35, 510),
    TIME_OFFSET(36, 83),
    TIME_OFFSET(36, 83),
    TIME_OFFSET(36, 655),
    TIME_OFFSET(36, 655),
    TIME_OFFSET(36, 942),
    TIME_OFFSET(36, 942),
    TIME_OFFSET(37, 228),
    TIME_OFFSET(37, 228),
    TIME_OFFSET(37, 514),
    TIME_OFFSET(37, 514),
    TIME_OFFSET(37, 801),
    TIME_OFFSET(37, 801),
    TIME_OFFSET(38, 374),
    TIME_OFFSET(38, 374),
    TIME_OFFSET(38, 946),
    TIME_OFFSET(38, 946),
    TIME_OFFSET(39, 519),
    TIME_OFFSET(39, 519),
    TIME_OFFSET(40, 92),
    TIME_OFFSET(40, 92),
    TIME_OFFSET(40, 951),
    TIME_OFFSET(40, 951),
    TIME_OFFSET(41, 237),
    TIME_OFFSET(41, 237),
    TIME_OFFSET(41, 524),
    TIME_OFFSET(41, 524),
    TIME_OFFSET(41, 810),
    TIME_OFFSET(41, 810),
    TIME_OFFSET(42, 96),
    TIME_OFFSET(42, 96),
    TIME_OFFSET(42, 383),
    TIME_OFFSET(42, 383),
    TIME_OFFSET(42, 669),
    TIME_OFFSET(42, 669),
    TIME_OFFSET(42, 956),
    TIME_OFFSET(42, 956),
    TIME_OFFSET(43, 242),
    TIME_OFFSET(43, 242),
    TIME_OFFSET(43, 528),
    TIME_OFFSET(43, 528),
    TIME_OFFSET(43, 815),
    TIME_OFFSET(43, 815),
    TIME_OFFSET(44, 101),
    TIME_OFFSET(44, 101),
    TIME_OFFSET(44, 387),
    TIME_OFFSET(44, 387),
    TIME_OFFSET(44, 674),
    TIME_OFFSET(44, 674),
    TIME_OFFSET(45, 247),
    TIME_OFFSET(45, 247),
    TIME_OFFSET(45, 819),
    TIME_OFFSET(45, 819),
    TIME_OFFSET(46, 106),
    TIME_OFFSET(46, 106),
    TIME_OFFSET(46, 249),
    TIME_OFFSET(46, 249),
    TIME_OFFSET(46, 392),
    TIME_OFFSET(46, 392),
    TIME_OFFSET(46, 678),
    TIME_OFFSET(46, 678),
    TIME_OFFSET(46, 822),
    TIME_OFFSET(46, 822),
    TIME_OFFSET(46, 965),
    TIME_OFFSET(46, 965),
    TIME_OFFSET(47, 108),
    TIME_OFFSET(47, 108),
    TIME_OFFSET(47, 251),
    TIME_OFFSET(47, 251),
    TIME_OFFSET(47, 394),
    TIME_OFFSET(47, 394),
    TIME_OFFSET(47, 537),
    TIME_OFFSET(47, 537),
    TIME_OFFSET(47, 681),
    TIME_OFFSET(47, 681),
    TIME_OFFSET(47, 824),
    TIME_OFFSET(47, 824),
    TIME_OFFSET(47, 967),
    TIME_OFFSET(47, 967),
    TIME_OFFSET(48, 110),
    TIME_OFFSET(48, 110),
    TIME_OFFSET(48, 397),
    TIME_OFFSET(48, 397),
    TIME_OFFSET(48, 540),
    TIME_OFFSET(48, 540),
    TIME_OFFSET(48, 683),
    TIME_OFFSET(48, 683),
    TIME_OFFSET(48, 969),
    TIME_OFFSET(48, 969),
    TIME_OFFSET(49, 113),
    TIME_OFFSET(49, 113),
    TIME_OFFSET(49, 256),
    TIME_OFFSET(49, 256),
    TIME_OFFSET(49, 399),
    TIME_OFFSET(49, 399),
    TIME_OFFSET(49, 542),
    TIME_OFFSET(49, 542),
    TIME_OFFSET(49, 685),
    TIME_OFFSET(49, 685),
    TIME_OFFSET(49, 828),
    TIME_OFFSET(49, 828),
    TIME_OFFSET(49, 972),
    TIME_OFFSET(49, 972),
    TIME_OFFSET(50, 115),
    TIME_OFFSET(50, 115),
    TIME_OFFSET(50, 258),
    TIME_OFFSET(50, 258),
    TIME_OFFSET(50, 401),
    TIME_OFFSET(50, 401),
    TIME_OFFSET(50, 688),
    TIME_OFFSET(50, 688),
    TIME_OFFSET(50, 831),
    TIME_OFFSET(50, 831),
    TIME_OFFSET(50, 974),
    TIME_OFFSET(50, 974),
    TIME_OFFSET(51, 260),
    TIME_OFFSET(51, 260),
    TIME_OFFSET(51, 404),
    TIME_OFFSET(51, 404),
    TIME_OFFSET(51, 547),
    TIME_OFFSET(51, 547),
    TIME_OFFSET(51, 690),
    TIME_OFFSET(51, 690),
    TIME_OFFSET(51, 833),
    TIME_OFFSET(51, 833),
    TIME_OFFSET(51, 976),
    TIME_OFFSET(51, 976),
    TIME_OFFSET(52, 119),
    TIME_OFFSET(52, 119),
    TIME_OFFSET(52, 263),
    TIME_OFFSET(52, 263),
    TIME_OFFSET(52, 406),
    TIME_OFFSET(52, 406),
    TIME_OFFSET(52, 549),
    TIME_OFFSET(52, 549),
    TIME_OFFSET(52, 692),
    TIME_OFFSET(52, 692),
    TIME_OFFSET(52, 979),
    TIME_OFFSET(52, 979),
    TIME_OFFSET(53, 122),
    TIME_OFFSET(53, 122),
    TIME_OFFSET(53, 265),
    TIME_OFFSET(53, 265),
    TIME_OFFSET(53, 551),
    TIME_OFFSET(53, 551),
    TIME_OFFSET(53, 695),
    TIME_OFFSET(53, 695),
    TIME_OFFSET(53, 838),
    TIME_OFFSET(53, 838),
    TIME_OFFSET(53, 981),
    TIME_OFFSET(53, 981),
    TIME_OFFSET(54, 124),
    TIME_OFFSET(54, 124),
    TIME_OFFSET(54, 267),
    TIME_OFFSET(54, 267),
    TIME_OFFSET(54, 410),
    TIME_OFFSET(54, 410),
    TIME_OFFSET(54, 554),
    TIME_OFFSET(54, 554),
    TIME_OFFSET(54, 697),
    TIME_OFFSET(54, 697),
    TIME_OFFSET(54, 840),
    TIME_OFFSET(54, 840),
    TIME_OFFSET(54, 983),
    TIME_OFFSET(54, 983),
    TIME_OFFSET(55, 270),
    TIME_OFFSET(55, 270),
    TIME_OFFSET(55, 413),
    TIME_OFFSET(55, 413),
    TIME_OFFSET(55, 556),
    TIME_OFFSET(55, 556),
    TIME_OFFSET(55, 842),
    TIME_OFFSET(55, 842),
    TIME_OFFSET(55, 986),
    TIME_OFFSET(55, 986),
    TIME_OFFSET(56, 129),
    TIME_OFFSET(56, 129),
    TIME_OFFSET(56, 272),
    TIME_OFFSET(56, 272),
    TIME_OFFSET(56, 415),
    TIME_OFFSET(56, 415),
    TIME_OFFSET(56, 558),
    TIME_OFFSET(56, 558),
    TIME_OFFSET(56, 701),
    TIME_OFFSET(56, 701),
    TIME_OFFSET(56, 845),
    TIME_OFFSET(56, 845),
    TIME_OFFSET(56, 988),
    TIME_OFFSET(56, 988),
    TIME_OFFSET(57, 131),
    TIME_OFFSET(57, 131),
    TIME_OFFSET(57, 274),
    TIME_OFFSET(57, 274),
    TIME_OFFSET(57, 561),
    TIME_OFFSET(57, 561),
    TIME_OFFSET(57, 704),
    TIME_OFFSET(57, 704),
    TIME_OFFSET(57, 847),
    TIME_OFFSET(57, 847),
    TIME_OFFSET(58, 133),
    TIME_OFFSET(58, 133),
    TIME_OFFSET(58, 277),
    TIME_OFFSET(58, 277),
    TIME_OFFSET(58, 420),
    TIME_OFFSET(58, 420),
    TIME_OFFSET(58, 563),
    TIME_OFFSET(58, 563),
    TIME_OFFSET(58, 706),
    TIME_OFFSET(58, 706),
    TIME_OFFSET(58, 849),
    TIME_OFFSET(58, 849),
    TIME_OFFSET(58, 992),
    TIME_OFFSET(58, 992),
    TIME_OFFSET(59, 136),
    TIME_OFFSET(59, 136),
    TIME_OFFSET(59, 279),
    TIME_OFFSET(59, 279),
    TIME_OFFSET(59, 422),
    TIME_OFFSET(59, 422),
    TIME_OFFSET(59, 565),
    TIME_OFFSET(59, 565),
    TIME_OFFSET(59, 852),
    TIME_OFFSET(59, 852),
    TIME_OFFSET(59, 995),
    TIME_OFFSET(59, 995),
    TIME_OFFSET(60, 138),
    TIME_OFFSET(60, 138),
    TIME_OFFSET(60, 424),
    TIME_OFFSET(60, 424),
    TIME_OFFSET(60, 567),
    TIME_OFFSET(60, 567),
    TIME_OFFSET(60, 711),
    TIME_OFFSET(60, 711),
    TIME_OFFSET(60, 854),
    TIME_OFFSET(60, 854),
    TIME_OFFSET(60, 997),
    TIME_OFFSET(60, 997),
    TIME_OFFSET(61, 140),
    TIME_OFFSET(61, 140),
    TIME_OFFSET(61, 283),
    TIME_OFFSET(61, 283),
    TIME_OFFSET(61, 427),
    TIME_OFFSET(61, 427),
    TIME_OFFSET(61, 570),
    TIME_OFFSET(61, 570),
    TIME_OFFSET(61, 713),
    TIME_OFFSET(61, 713),
    TIME_OFFSET(61, 856),
    TIME_OFFSET(61, 856),
    TIME_OFFSET(62, 143),
    TIME_OFFSET(62, 143),
    TIME_OFFSET(62, 286),
    TIME_OFFSET(62, 286),
    TIME_OFFSET(62, 429),
    TIME_OFFSET(62, 429),
    TIME_OFFSET(62, 715),
    TIME_OFFSET(62, 715),
    TIME_OFFSET(62, 858),
    TIME_OFFSET(62, 858),
    TIME_OFFSET(63, 2),
    TIME_OFFSET(63, 2),
    TIME_OFFSET(63, 145),
    TIME_OFFSET(63, 145),
    TIME_OFFSET(63, 288),
    TIME_OFFSET(63, 288),
    TIME_OFFSET(63, 431),
    TIME_OFFSET(63, 431),
    TIME_OFFSET(63, 574),
    TIME_OFFSET(63, 574),
    TIME_OFFSET(63, 718),
    TIME_OFFSET(63, 718),
    TIME_OFFSET(63, 861),
    TIME_OFFSET(63, 861),
    TIME_OFFSET(64, 4),
    TIME_OFFSET(64, 4),
    TIME_OFFSET(64, 147),
    TIME_OFFSET(64, 147),
    TIME_OFFSET(64, 434),
    TIME_OFFSET(64, 434),
    TIME_OFFSET(64, 577),
    TIME_OFFSET(64, 577),
    TIME_OFFSET(64, 720),
    TIME_OFFSET(64, 720),
    TIME_OFFSET(65, 6),
    TIME_OFFSET(65, 6),
    TIME_OFFSET(65, 293),
    TIME_OFFSET(65, 293),
    TIME_OFFSET(65, 579),
    TIME_OFFSET(65, 579),
    TIME_OFFSET(65, 722),
    TIME_OFFSET(65, 722),
    TIME_OFFSET(65, 865),
    TIME_OFFSET(65, 865),
    TIME_OFFSET(66, 152),
    TIME_OFFSET(66, 152),
    TIME_OFFSET(66, 438),
    TIME_OFFSET(66, 438),
    TIME_OFFSET(66, 725),
    TIME_OFFSET(66, 725),
    TIME_OFFSET(66, 868),
    TIME_OFFSET(66, 868),
    TIME_OFFSET(67, 11),
    TIME_OFFSET(67, 11),
    TIME_OFFSET(67, 297),
    TIME_OFFSET(67, 297),
    TIME_OFFSET(67, 584),
    TIME_OFFSET(67, 584),
    TIME_OFFSET(67, 870),
    TIME_OFFSET(67, 870),
    TIME_OFFSET(68, 13),
    TIME_OFFSET(68, 13),
    TIME_OFFSET(68, 156),
    TIME_OFFSET(68, 156),
    TIME_OFFSET(68, 443),
    TIME_OFFSET(68, 443),
    TIME_OFFSET(68, 729),
    TIME_OFFSET(68, 729),
    TIME_OFFSET(69, 16),
    TIME_OFFSET(69, 16),
    TIME_OFFSET(69, 159),
    TIME_OFFSET(69, 159),
    TIME_OFFSET(69, 302),
    TIME_OFFSET(69, 302),
    TIME_OFFSET(69, 588),
    TIME_OFFSET(69, 588),
    TIME_OFFSET(69, 875),
    TIME_OFFSET(69, 875),
    TIME_OFFSET(70, 161),
    TIME_OFFSET(70, 161),
    TIME_OFFSET(70, 304),
    TIME_OFFSET(70, 304),
    TIME_OFFSET(70, 447),
    TIME_OFFSET(70, 447),
    TIME_OFFSET(70, 734),
    TIME_OFFSET(70, 734),
    TIME_OFFSET(71, 20),
    TIME_OFFSET(71, 20),
    TIME_OFFSET(71, 306),
    TIME_OFFSET(71, 306),
    TIME_OFFSET(71, 450),
    TIME_OFFSET(71, 450),
    TIME_OFFSET(71, 593),
    TIME_OFFSET(71, 593),
    TIME_OFFSET(71, 879),
    TIME_OFFSET(71, 879),
    TIME_OFFSET(72, 166),
    TIME_OFFSET(72, 166),
    TIME_OFFSET(72, 452),
    TIME_OFFSET(72, 452),
    TIME_OFFSET(72, 595),
    TIME_OFFSET(72, 595),
    TIME_OFFSET(72, 738),
    TIME_OFFSET(72, 738),
    TIME_OFFSET(73, 25),
    TIME_OFFSET(73, 25),
    TIME_OFFSET(73, 311),
    TIME_OFFSET(73, 311),
    TIME_OFFSET(74, 170),
    TIME_OFFSET(74, 170),
    TIME_OFFSET(74, 457),
    TIME_OFFSET(74, 457),
    TIME_OFFSET(74, 743),
    TIME_OFFSET(74, 743),
    TIME_OFFSET(75, 29),
    TIME_OFFSET(75, 29),
    TIME_OFFSET(75, 316),
    TIME_OFFSET(75, 316),
    TIME_OFFSET(75, 602),
    TIME_OFFSET(75, 602),
    TIME_OFFSET(76, 461),
    TIME_OFFSET(76, 461),
    TIME_OFFSET(76, 748),
    TIME_OFFSET(76, 748),
    TIME_OFFSET(77, 34),
    TIME_OFFSET(77, 34),
    TIME_OFFSET(77, 320),
    TIME_OFFSET(77, 320),
    TIME_OFFSET(77, 607),
    TIME_OFFSET(77, 607),
    TIME_OFFSET(77, 893),
    TIME_OFFSET(77, 893),
    TIME_OFFSET(79, 39),
    TIME_OFFSET(79, 39),
    TIME_OFFSET(80, 184),
    TIME_OFFSET(80, 184),
    TIME_OFFSET(80, 470),
    TIME_OFFSET(80, 470),
    TIME_OFFSET(80, 757),
    TIME_OFFSET(80, 757),
    TIME_OFFSET(81, 43),
    TIME_OFFSET(81, 43),
    TIME_OFFSET(81, 330),
    TIME_OFFSET(81, 330),
    TIME_OFFSET(82, 475),
    TIME_OFFSET(82, 475),
    TIME_OFFSET(83, 48),
    TIME_OFFSET(83, 48),
    TIME_OFFSET(83, 334),
    TIME_OFFSET(83, 334),
    TIME_OFFSET(83, 621),
    TIME_OFFSET(83, 621),
    TIME_OFFSET(83, 907),
    TIME_OFFSET(83, 907),
    TIME_OFFSET(84, 193),
    TIME_OFFSET(84, 193),
    TIME_OFFSET(84, 480),
    TIME_OFFSET(84, 480),
    TIME_OFFSET(84, 766),
    TIME_OFFSET(84, 766),
    TIME_OFFSET(85, 625),
    TIME_OFFSET(85, 625),
    TIME_OFFSET(85, 912),
    TIME_OFFSET(85, 912),
    TIME_OFFSET(86, 198),
    TIME_OFFSET(86, 198),
    TIME_OFFSET(86, 484),
    TIME_OFFSET(86, 484),
    TIME_OFFSET(86, 771),
    TIME_OFFSET(86, 771),
    TIME_OFFSET(87, 57),
    TIME_OFFSET(87, 57),
    TIME_OFFSET(87, 343),
    TIME_OFFSET(87, 343),
    TIME_OFFSET(87, 630),
    TIME_OFFSET(87, 630),
    TIME_OFFSET(87, 916),
    TIME_OFFSET(87, 916),
    TIME_OFFSET(88, 203),
    TIME_OFFSET(88, 203),
    TIME_OFFSET(89, 348),
    TIME_OFFSET(89, 348),
    TIME_OFFSET(89, 634),
    TIME_OFFSET(89, 634),
    TIME_OFFSET(89, 921),
    TIME_OFFSET(89, 921),
    TIME_OFFSET(90, 207),
    TIME_OFFSET(90, 207),
    TIME_OFFSET(90, 494),
    TIME_OFFSET(90, 494),
    TIME_OFFSET(91, 353),
    TIME_OFFSET(91, 353),
    TIME_OFFSET(91, 639),
    TIME_OFFSET(91, 639),
    TIME_OFFSET(92, 212),
    TIME_OFFSET(92, 212),
    TIME_OFFSET(92, 498),
    TIME_OFFSET(92, 498),
    TIME_OFFSET(92, 785),
    TIME_OFFSET(92, 785),
    TIME_OFFSET(93, 71),
    TIME_OFFSET(93, 71),
    TIME_OFFSET(93, 357),
    TIME_OFFSET(93, 357),
    TIME_OFFSET(93, 644),
    TIME_OFFSET(93, 644),
    TIME_OFFSET(93, 930),
    TIME_OFFSET(93, 930),
    TIME_OFFSET(94, 789),
    TIME_OFFSET(94, 789),
    TIME_OFFSET(95, 75),
    TIME_OFFSET(95, 75),
    TIME_OFFSET(95, 362),
    TIME_OFFSET(95, 362),
    TIME_OFFSET(95, 648),
    TIME_OFFSET(95, 648),
    TIME_OFFSET(95, 935),
    TIME_OFFSET(95, 935),
    TIME_OFFSET(96, 221),
    TIME_OFFSET(96, 221),
    TIME_OFFSET(96, 507),
    TIME_OFFSET(96, 507),
    TIME_OFFSET(96, 794),
    TIME_OFFSET(96, 794),
    TIME_OFFSET(97, 80),
    TIME_OFFSET(97, 80),
    TIME_OFFSET(97, 366),
    TIME_OFFSET(97, 366),
    TIME_OFFSET(98, 512),
    TIME_OFFSET(98, 512),
    TIME_OFFSET(98, 798),
    TIME_OFFSET(98, 798),
    TIME_OFFSET(99, 85),
    TIME_OFFSET(99, 85),
    TIME_OFFSET(99, 371),
    TIME_OFFSET(99, 371),
    TIME_OFFSET(99, 657),
    TIME_OFFSET(99, 657),
    TIME_OFFSET(100, 517),
    TIME_OFFSET(100, 517),
    TIME_OFFSET(100, 803),
    TIME_OFFSET(100, 803),
    TIME_OFFSET(101, 376),
    TIME_OFFSET(101, 376),
    TIME_OFFSET(101, 662),
    TIME_OFFSET(101, 662),
    TIME_OFFSET(101, 948),
    TIME_OFFSET(101, 948),
    TIME_OFFSET(102, 235),
    TIME_OFFSET(102, 235),
    TIME_OFFSET(102, 521),
    TIME_OFFSET(102, 521),
    TIME_OFFSET(102, 808),
    TIME_OFFSET(102, 808),
    TIME_OFFSET(103, 94),
    TIME_OFFSET(103, 94),
    TIME_OFFSET(103, 667),
    TIME_OFFSET(103, 667),
    TIME_OFFSET(104, 812),
    TIME_OFFSET(104, 812),
    TIME_OFFSET(105, 385),
    TIME_OFFSET(105, 385),
    TIME_OFFSET(106, 530),
    TIME_OFFSET(106, 530),
    TIME_OFFSET(107, 676),
    TIME_OFFSET(107, 676),
    TIME_OFFSET(108, 821),
    TIME_OFFSET(108, 821),
    TIME_OFFSET(109, 967),
    TIME_OFFSET(109, 967),
    TIME_OFFSET(111, 112),
    TIME_OFFSET(111, 112),
    TIME_OFFSET(112, 258),
    TIME_OFFSET(112, 258),
    TIME_OFFSET(113, 403),
    TIME_OFFSET(113, 403),
    TIME_OFFSET(114, 549),
    TIME_OFFSET(114, 549),
    TIME_OFFSET(115, 694),
    TIME_OFFSET(115, 694),
    TIME_OFFSET(116, 840),
    TIME_OFFSET(116, 840),
    TIME_OFFSET(117, 985),
    TIME_OFFSET(117, 985),
    TIME_OFFSET(119, 131),
    TIME_OFFSET(119, 131),
    TIME_OFFSET(120, 276),
    TIME_OFFSET(120, 276),
    TIME_OFFSET(121, 422),
    TIME_OFFSET(121, 422),
    TIME_OFFSET(122, 567),
    TIME_OFFSET(122, 567),
    TIME_OFFSET(123, 713),
    TIME_OFFSET(123, 713),
    TIME_OFFSET(124, 858),
    TIME_OFFSET(124, 858),
    TIME_OFFSET(126, 4),
    TIME_OFFSET(126, 4),
    TIME_OFFSET(127, 149),
    TIME_OFFSET(127, 149),
    TIME_OFFSET(128, 295),
    TIME_OFFSET(128, 295),
    TIME_OFFSET(130, 586),
};
static const uint8_t notes[] = {90, 90, 88, 88, 86, 86, 85, 85, 83, 83, 81, 81, 83, 83, 85, 85, 90, 90, 88, 88, 86, 86, 85, 85, 83, 83, 81, 81, 83, 83, 85, 85, 90, 90, 88, 88, 86, 86, 85, 85, 83, 83, 81, 81, 83, 83, 85, 85, 86, 86, 85, 85, 86, 86, 78, 78, 81, 81, 85, 85, 86, 86, 90, 90, 93, 93, 95, 95, 91, 91, 90, 90, 88, 88, 91, 91, 90, 90, 88, 88, 86, 86, 85, 85, 83, 83, 81, 81, 79, 79, 78, 78, 76, 76, 85, 85, 86, 86, 85, 85, 86, 86, 78, 78, 81, 81, 85, 85, 86, 86, 90, 90, 93, 93, 95, 95, 91, 91, 90, 90, 88, 88, 91, 91, 90, 90, 88, 88, 86, 86, 85, 85, 83, 83, 81, 81, 79, 79, 78, 78, 76, 76, 85, 85, 86, 86, 90, 90, 91, 91, 93, 93, 90, 90, 91, 91, 93, 93, 81, 81, 83, 83, 85, 85, 86, 86, 88, 88, 90, 90, 91, 91, 90, 90, 86, 86, 88, 88, 90, 90, 78, 78, 79, 79, 81, 81, 83, 83, 81, 81, 79, 79, 81, 81, 78, 78, 79, 79, 81, 81, 79, 79, 83, 83, 81, 81, 79, 79, 78, 78, 76, 76, 78, 78, 76, 76, 74, 74, 76, 76, 78, 78, 79, 79, 81, 81, 83, 83, 79, 79, 83, 83, 81, 81, 83, 83, 85, 85, 86, 86, 85, 85, 83, 83, 85, 85, 86, 86, 88, 88, 90, 90, 91, 91, 88, 88, 86, 86, 90, 90, 91, 91, 93, 93, 90, 90, 91, 91, 93, 93, 81, 81, 83, 83, 85, 85, 86, 86, 88, 88, 90, 90, 91, 91, 90, 90, 86, 86, 88, 88, 90, 90, 78, 78, 79, 79, 81, 81, 83, 83, 81, 81, 79, 79, 81, 81, 78, 78, 79, 79, 81, 81, 79, 79, 83, 83, 81, 81, 79, 79, 78, 78, 76, 76, 78, 78, 76, 76, 74, 74, 76, 76, 78, 78, 79, 79, 81, 81, 83, 83, 79, 79, 83, 83, 81, 81, 83, 83, 85, 85, 86, 86, 85, 85, 83, 83, 85, 85, 86, 86, 88, 88, 90, 90, 91, 91, 88, 88, 90, 90, 86, 86, 85, 85, 86, 86, 78, 78, 81, 81, 85, 85, 86, 86, 88, 88, 85, 85, 83, 83, 86, 86, 88, 88, 90, 90, 86, 86, 90, 90, 90, 90, 88, 88, 86, 86, 85, 85, 83, 83, 83, 83, 81, 81, 83, 83, 85, 85, 86, 86, 90, 90, 88, 88, 86, 86, 93, 93, 91, 91, 86, 86, 85, 85, 83, 83, 83, 83, 81, 81, 81, 81, 79, 79, 78, 78, 76, 76, 86, 86, 90, 90, 90, 90, 91, 91, 90, 90, 88, 88, 86, 86, 86, 86, 86, 86, 88, 88, 86, 86, 85, 85, 83, 83, 86, 86, 86, 86, 84, 84, 83, 83, 84, 84, 81, 81, 86, 86, 90, 90, 93, 93, 93, 93, 95, 95, 93, 93, 91, 91, 90, 90, 90, 90, 90, 90, 91, 91, 90, 90, 88, 88, 86, 86, 84, 84, 83, 83, 84, 84, 86, 86, 86, 86, 84, 84, 83, 83, 84, 84, 85, 85, 85, 85, 86, 86, 90, 90, 93, 93, 93, 93, 95, 95, 93, 93, 91, 91, 90, 90, 90, 90, 90, 90, 91, 91, 90, 90, 88, 88, 86, 86, 84, 84, 83, 83, 84, 84, 86, 86, 86, 86, 84, 84, 83, 83, 84, 84, 85, 85, 85, 85, 86, 86, 90, 90, 93, 93, 93, 93, 95, 95, 93, 93, 91, 91, 90, 90, 98, 98, 97, 97, 95, 95, 93, 93, 95, 95, 97, 97, 90, 90, 88, 88, 86, 86, 85, 85, 83, 83, 81, 81, 83, 83, 85, 85, 90, 90, 88, 88, 86, 86, 85, 85, 83, 83, 81, 81, 83, 83, 85, 85, 86, 86};
static const uint8_t velocities[] = {58, 0, 53, 0, 52, 0, 53, 0, 52, 0, 58, 0, 63, 0, 67, 0, 98, 0, 79, 0, 81, 0, 78, 0, 77, 0, 77, 0, 89, 0, 95, 0, 95, 0, 78, 0, 76, 0, 79, 0, 80, 0, 81, 0, 89, 0, 90, 0, 89, 0, 80, 0, 87, 0, 73, 0, 88, 0, 97, 0, 87, 0, 91, 0, 86, 0, 92, 0, 76, 0, 81, 0, 74, 0, 87, 0, 81, 0, 84, 0, 79, 0, 77, 0, 81, 0, 81, 0, 84, 0, 79, 0, 80, 0, 101, 0, 92, 0, 84, 0, 87, 0, 72, 0, 86, 0, 94, 0, 88, 0, 94, 0, 98, 0, 91, 0, 75, 0, 75, 0, 75, 0, 93, 0, 77, 0, 80, 0, 77, 0, 79, 0, 76, 0, 80, 0, 76, 0, 78, 0, 76, 0, 102, 0, 89, 0, 95, 0, 90, 0, 89, 0, 78, 0, 88, 0, 90, 0, 63, 0, 89, 0, 84, 0, 91, 0, 86, 0, 89, 0, 87, 0, 80, 0, 77, 0, 90, 0, 89, 0, 63, 0, 85, 0, 89, 0, 91, 0, 77, 0, 80, 0, 92, 0, 75, 0, 91, 0, 93, 0, 76, 0, 90, 0, 79, 0, 76, 0, 77, 0, 81, 0, 96, 0, 83, 0, 77, 0, 93, 0, 91, 0, 89, 0, 88, 0, 92, 0, 71, 0, 93, 0, 78, 0, 88, 0, 88, 0, 92, 0, 80, 0, 78, 0, 89, 0, 92, 0, 93, 0, 88, 0, 90, 0, 71, 0, 78, 0, 94, 0, 86, 0, 87, 0, 81, 0, 87, 0, 89, 0, 63, 0, 85, 0, 90, 0, 89, 0, 91, 0, 87, 0, 93, 0, 82, 0, 76, 0, 87, 0, 92, 0, 62, 0, 85, 0, 89, 0, 89, 0, 73, 0, 81, 0, 89, 0, 78, 0, 92, 0, 89, 0, 78, 0, 88, 0, 80, 0, 77, 0, 78, 0, 81, 0, 88, 0, 77, 0, 77, 0, 93, 0, 86, 0, 87, 0, 90, 0, 88, 0, 74, 0, 91, 0, 79, 0, 90, 0, 91, 0, 88, 0, 78, 0, 78, 0, 88, 0, 91, 0, 87, 0, 91, 0, 87, 0, 77, 0, 89, 0, 75, 0, 83, 0, 88, 0, 69, 0, 91, 0, 90, 0, 87, 0, 89, 0, 78, 0, 80, 0, 91, 0, 91, 0, 94, 0, 75, 0, 92, 0, 84, 0, 81, 0, 78, 0, 78, 0, 81, 0, 84, 0, 83, 0, 92, 0, 91, 0, 90, 0, 94, 0, 79, 0, 78, 0, 94, 0, 82, 0, 70, 0, 81, 0, 78, 0, 87, 0, 79, 0, 85, 0, 77, 0, 79, 0, 79, 0, 100, 0, 100, 0, 84, 0, 82, 0, 77, 0, 77, 0, 77, 0, 81, 0, 88, 0, 88, 0, 74, 0, 76, 0, 79, 0, 94, 0, 86, 0, 74, 0, 80, 0, 87, 0, 71, 0, 94, 0, 98, 0, 89, 0, 90, 0, 86, 0, 78, 0, 75, 0, 80, 0, 81, 0, 85, 0, 86, 0, 78, 0, 78, 0, 80, 0, 80, 0, 75, 0, 87, 0, 88, 0, 86, 0, 75, 0, 80, 0, 84, 0, 86, 0, 82, 0, 89, 0, 90, 0, 94, 0, 86, 0, 93, 0, 82, 0, 81, 0, 75, 0, 82, 0, 83, 0, 93, 0, 81, 0, 79, 0, 79, 0, 81, 0, 77, 0, 90, 0, 88, 0, 88, 0, 79, 0, 80, 0, 89, 0, 90, 0, 80, 0, 89, 0, 93, 0, 89, 0, 86, 0, 88, 0, 73, 0, 76, 0, 81, 0, 102, 0, 82, 0, 77, 0, 76, 0, 95, 0, 87, 0, 70, 0, 77, 0, 75, 0, 80, 0, 81, 0, 78, 0, 89, 0, 87, 0, 95, 0, 80, 0, 82, 0, 82, 0, 83, 0, 77, 0, 83, 0, 94, 0, 84, 0};

static int pwm0_stable_timer(void)
{
    static int loops = 0;

    loops++;
    if (loops > PWM_DELAY_LOOPS)
    {
        int32_t scaled_sample = next_sample + 129;
        if (scaled_sample > 255)
            scaled_sample = 255;
        if (scaled_sample < 1)
            scaled_sample = 1;
        writel(scaled_sample, TPM0_C1V);
        writel(scaled_sample, TPM0_C0V);

        loops = 0;
        sample_queued = 0;
        //global_tick_counter++;
    }

    static int other_loops;
    if (other_loops++ > 12) {
        global_tick_counter++;
        other_loops = 0;
    }

    /* Reset the timer IRQ, to allow us to fire again next time */
    writel(TPM0_STATUS_CH1F | TPM0_STATUS_CH0F | TPM0_STATUS_TOF, TPM0_STATUS);

    return 0;
}

static void prepare_pwm()
{
    // Write dummy values out, to configure PWM mux
    pinMode(0, OUTPUT);
    analogWrite(0, 63);
    pinMode(1, OUTPUT);
    analogWrite(1, 63);

    // Disable TPM0, allowing us to configure it
    writel(0, TPM0_SC);

    // Also disable both channels, which are running from the
    // calls to analogWrite() above
    writel(0, TPM0_C0SC);
    writel(0, TPM0_C1SC);

    // Configure the TPM to use the MCGFLLCLK (~32 MHz?)
    writel(readl(SIM_SOPT2) | (1 << 24), SIM_SOPT2);

    // We've picked pin 0, which is on TPM0_CH1
    writel(255, TPM0_MOD);
    writel(0, TPM0_CNT);

    writel(TPM0_C0SC_MSB | TPM0_C0SC_ELSB, TPM0_C0SC);
    writel(TPM0_C1SC_MSB | TPM0_C1SC_ELSA, TPM0_C1SC);

    writel(100, TPM0_C1V);
    writel(100, TPM0_C0V);
    writel(TPM0_CONF_TRGSEL(8), TPM0_CONF);
    writel(TPM0_SC_TOF | TPM0_SC_TOIE | TPM0_SC_CMOD(1) | TPM0_SC_PS(0), TPM0_SC); // Enable TPM0

    /* Enable the IRQ in the system-wide interrupt table */
    attachFastInterrupt(PWM0_IRQ, pwm0_stable_timer);
}

static void setInstrument(struct ltc_voice *voice,
                          const struct ltc_instrument *instrument,
                          uint32_t attack_time, uint32_t attack_level,
                          uint32_t decay_time,
                          uint32_t sustain_level,
                          uint32_t release_time)
{
    voice->instrument = instrument;
    voice->attack_time = (attack_time * SAMPLE_RATE) / 1000;
    voice->attack_level = attack_level;
    voice->decay_time = (decay_time * SAMPLE_RATE) / 1000 + voice->attack_time;
    voice->sustain_level = sustain_level;
    voice->release_time = (release_time * SAMPLE_RATE) / 1000;
}

void setup(void)
{
    setInstrument(&voice[0],
                  &sine_instrument,
                  450, 70,
                  250,
                  30,
                  50);

    setInstrument(&voice[1],
                  &triangle_instrument,
                  150, 50,
                  100,
                  40,
                  50);
    voice[0].phase_accumulator = 995;

    prepare_pwm();
    enableInterrupt(PWM0_IRQ);
    pinMode(2, OUTPUT);
    pinMode(3, OUTPUT);
    pinMode(4, OUTPUT);
    pinMode(5, OUTPUT);
}

#define ATTACK_PHASE 1
#define DECAY_PHASE 2
#define SUSTAIN_PHASE 3
#define RELEASE_PHASE 4
#define DEBUG_PHASE(x) do { \
writel((1 << 12), (x == ATTACK_PHASE) ? FGPIOA_PSOR : FGPIOA_PCOR); \
writel((1 << 13), (x == DECAY_PHASE) ? FGPIOB_PSOR : FGPIOB_PCOR); \
writel((1 << 0), (x == SUSTAIN_PHASE) ? FGPIOB_PSOR : FGPIOB_PCOR); \
writel((1 << 7), (x == RELEASE_PHASE) ? FGPIOA_PSOR : FGPIOA_PCOR); \
} while(0);
static int32_t processADSR(struct ltc_voice *voice, int32_t output)
{
    int32_t pct;
    if (voice->release_timer != 0)
    {
        /* Release phase */
        DEBUG_PHASE(RELEASE_PHASE);

        /* The note has expired */
        if ((voice->note_timer - voice->release_timer) > voice->release_time)
            return 0;

        /* Determine what percentage we'll adjust the note to */
        pct = (voice->note_timer - voice->release_timer) * voice->sustain_level / voice->release_time;
    }
    else if (voice->note_timer < voice->attack_time)
    {
        /* Attack phase */
        DEBUG_PHASE(ATTACK_PHASE);

        /* Determine what percentage we'll adjust the note to */
        pct = voice->note_timer * voice->attack_level / voice->attack_time;
    }
    else if (voice->note_timer < voice->decay_time)
    {
        /* Decay phase */
        DEBUG_PHASE(DECAY_PHASE);

        /* Determine what percentage we'll adjust the note to */
        pct = voice->sustain_level + (voice->decay_time - voice->note_timer) * (voice->attack_level - voice->sustain_level) / (voice->decay_time - voice->attack_time);
    }
    else
    {
        /* Sustain phase */
        DEBUG_PHASE(SUSTAIN_PHASE);
        pct = voice->sustain_level;
    }

    /* Scale the note volume to the calcualted percentage */
    output = output * pct / 100;
    return output;
}

int32_t get_sample(struct ltc_voice *voice)
{
    uint32_t period;
    int32_t output;
    int32_t v1, v2, v1_weight, v2_weight;

    // calculate the phase accumulator distance
    // we divide the frequency by the sample rate to give us how much of a cycle occurs
    // between successive samples... assuming a frequency range of 20Hz-20kHz this would
    // be on the order of 0.0004 to 0.4, so we multiply it to give us a meaningful range

    period = (voice->frequency * PHASEACC_MAX) / SAMPLE_RATE;

    // add this to the phase accumulator
    voice->phase_accumulator += period;

    // wrap the phase accumulator around
    voice->phase_accumulator &= (PHASEACC_MAX - 1);

    // and now get the sine output values for each of the allowed harmonics
    // to be elegant (and to improve the sound quality) we should really
    // interpolate between the current table entry and the next one by an amount
    // proportional to the position between the two entries, but this is just
    // an example so we won't bother for now
    uint32_t position = (voice->phase_accumulator * voice->instrument->length) / PHASEACC_MAX;

    // Interpolation happens because there are "gaps" that are between the phase
    // accumulator and the table.
    if (INTERPOLATION_ENABLED && (voice->instrument->flags & INSTRUMENT_CAN_INTERPOLATE))
    {
        // This is how far off we are.  I.e. the error.
        int32_t distance = voice->phase_accumulator - ((position * PHASEACC_MAX) / voice->instrument->length);

        // And this is how many "Gaps" there are total between two entries in the table
        const int32_t gap = PHASEACC_MAX / voice->instrument->length;

        v1 = voice->instrument->samples[position];
        position++;
        if (position >= voice->instrument->length)
            position -= voice->instrument->length;
        v2 = voice->instrument->samples[position];

        v1_weight = gap - distance;
        v2_weight = gap - v1_weight;

        output = ((v1 * v1_weight) + (v2 * v2_weight)) / gap;
    }
    else
    {
        output = voice->instrument->samples[position];
    }

    output = processADSR(voice, output);

    voice->note_timer++;
    return output;
}

static void noteOn(struct ltc_voice *voice, uint32_t freq)
{
    voice->frequency = freq;
    voice->note_timer = 0;
    voice->release_timer = 0;
}

static void noteOff(struct ltc_voice *voice)
{
    voice->release_timer = voice->note_timer;
}

#define VOICE_2_DELAY TIME_OFFSET(9, 163)

void loop(void)
{
    static uint32_t current_note;
    static uint32_t current_note_2;

    // If a sample is still in the buffer, don't do anything.
    if (sample_queued)
        return;
 
    if (global_tick_counter >= (timesteps[current_note]))
    {

        if (velocities[current_note] == 0)
        {
            noteOff(&voice[0]);
            current_note++;
        }
        else
        {
            noteOn(&voice[0], midi_freq[notes[current_note]]);
            current_note++;
        }
        if (current_note > sizeof(notes))
            current_note = 0;
    }

    if ((global_tick_counter > VOICE_2_DELAY) && ((global_tick_counter - VOICE_2_DELAY) >= (timesteps[current_note_2])))
    {
        if (velocities[current_note_2] == 0)
        {
            noteOff(&voice[1]);
            current_note_2++;
        }
        else
        {
            noteOn(&voice[1], midi_freq[notes[current_note_2]]);
            current_note_2++;
        }
        if (current_note_2 > sizeof(notes))
            current_note_2 = 0;
    }

    next_sample = 0;
    next_sample += get_sample(&voice[0]);
    next_sample += get_sample(&voice[1]);
    sample_queued = 1;
}
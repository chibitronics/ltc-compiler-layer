#!/bin/sh -e
gpio_dir=/sys/class/gpio
status_green=32
status_red=33
reset_pulse=34
reset_level=35
servo_pwm=36
light_sensor=37
all_pins="0 1 2 3 4 5 ${status_green} ${status_red} ${reset_pulse} ${reset_level}"
test_program=ltctest.wav
uart=/dev/ttyAMA0
baud=9600
error_count=0

plunger_servo_up_level=97
plunger_servo_down_level=140

pin_to_gpio() {
	local pin_num="$1"
	case "${pin_num}" in
		0|0a|0b) echo 4 ;;
		1|1a|1b) echo 17 ;;
		2|2a|2b) echo 27 ;;
		3|3a|3b) echo 22 ;;
		4) echo 5 ;;
		5) echo 6 ;;
		${status_green}) echo 13 ;;
		${status_red}) echo 8 ;;
		${reset_pulse}) echo 23 ;;
		${reset_level}) echo 18 ;;
		${servo_pwm}) echo 12 ;;
		${light_sensor}) echo 2 ;;
		*) echo "Unrecognized pin: ${pin_num}" ; exit 1; ;;
	esac
}

setup_light_sensor() {
	export_pin ${light_sensor}
	local pin_num=$(pin_to_gpio ${light_sensor})
	echo in > ${gpio_dir}/gpio${pin_num}/direction
	echo both > ${gpio_dir}/gpio${pin_num}/edge
}

pulse_range_pin() {
	local pin_num="$1"
	case "${pin_num}" in
		# Pulse is at 500 Hz, but we read both edges.  With
		# the scheduler overhead, it comes to 1039.
		0|1|2|3) echo 1039 ;;

		# Again, we sample both edges
		4|5) echo 662 ;;
		rgb) echo 812 ;;
		room) echo 0 ;;
		*) echo "???" ;;
	esac
}

export_pin() {
	local pin_num=$(pin_to_gpio "$1")
	if [ -e "${gpio_dir}/gpio${pin_num}" ]
	then
		echo ${pin_num} > /sys/class/gpio/unexport
	fi

	if [ ! -e "${gpio_dir}/gpio${pin_num}" ]
	then
		echo ${pin_num} > /sys/class/gpio/export
	fi
}

set_input() {
	local pin_num=$(pin_to_gpio "$1")
	echo "in" > ${gpio_dir}/gpio${pin_num}/direction
}

set_output() {
	local pin_num=$(pin_to_gpio "$1")
	echo "out" > ${gpio_dir}/gpio${pin_num}/direction
}

set_low() {
	local pin_num=$(pin_to_gpio "$1")
	echo 0 > ${gpio_dir}/gpio${pin_num}/value
}

set_high() {
	local pin_num=$(pin_to_gpio "$1")
	echo 1 > ${gpio_dir}/gpio${pin_num}/value
}

get_value() {
	local pin_num=$(pin_to_gpio "$1")
	set_input "$1"
	if [ $(cat ${gpio_dir}/gpio${pin_num}/value) = 0 ]
	then
		return 0
	fi
	return 1
}

enter_programming_mode() {
	set_output ${reset_level}
	set_low ${reset_level}
	sleep 1
	set_input ${reset_level}
}

reset_board() {
	set_input ${reset_pulse}
	set_output ${reset_level}
	set_low ${reset_level}
	sleep .005
	set_input ${reset_level}
}

wait_for_banner() {
	grep -q "LTC factory test is running" ${uart}
}

wait_for_green_on() {
	until get_value ${status_green}
	do
		sleep 0.1
	done
}

wait_for_green_off() {
	until ! get_value ${status_green}
	do
		sleep 0.1
	done
}

wait_for_red_on() {
	until get_value ${status_red}
	do
		sleep 0.1
	done
}

wait_for_red_off() {
	until ! get_value ${status_red}
	do
		sleep 0.1
	done
}

pulse_count() {
	center=$(pulse_range_pin "$1")
	range="$2"
	before=$(grep 'pinctrl-bcm2835   2 ' /proc/interrupts)
	sleep 1
	after=$(grep 'pinctrl-bcm2835   2 ' /proc/interrupts | awk '{print $2}')
	before=$(echo "${before}" | awk '{print $2}')

	difference=$((${after}-${before}))
	ub=$((${center} + ${range}))
	lb=$((${center} - ${range}))

	range_val="${lb} <= ${difference} <= ${ub} [${before} ${after}]"

	[ ${difference} -lt ${ub} ] && [ ${difference} -gt ${lb} ]
}

#move_plunger_up() {
#	if [ ! -e /dev/pi-blaster ]
#	then
#		echo "Failed to move plunger: is pi-blaster running?"
#		return 1
#	fi
#	echo $(pin_to_gpio ${servo_pwm})=0.048 > /dev/pi-blaster
#}
#
#move_plunger_down() {
#	gpio -g pwm $(pin_to_gpio ${servo_pwm}) $((${plunger_servo_down_level}-10))
#	for i in $(seq $((${plunger_servo_down_level}-9)) ${plunger_servo_down_level})
#	do
#		sleep .1
#		gpio -g pwm $(pin_to_gpio ${servo_pwm}) $i
#	done
#}

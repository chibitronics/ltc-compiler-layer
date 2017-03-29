#!/bin/sh -e

source ./00-test-lib.sh

# Export all pins so that we can use them
for pin in ${all_pins}
do
	export_pin ${pin}
done

setup_light_sensor
if ! pulse_count room 16
then
	echo "Room too bright.  Shield test jig."
	exit 1
fi

# Start out by setting all pins low.
# The bootloader does this, so we're not
# really fighting it here.
echo "Setting up pins:"
echo "    O: 0"
set_output 0
echo "    O: 1"
set_output 1
echo "    O: 2"
set_output 2
echo "    O: 3"
set_output 3
echo "    O: 4"
set_output 4
echo "    O: 5"
set_output 5
echo "    L: 0"
set_low 0
echo "    L: 1"
set_low 1
echo "    L: 2"
set_low 2
echo "    L: 3"
set_low 3
echo "    L: 4"
set_low 4
echo "    L: 5"
set_low 5
echo "    I: G"
set_input ${status_green}
echo "    I: R"
set_input ${status_red}
echo "    I: M"
set_input ${reset_pulse}
echo "    I: I"
set_input ${reset_level}

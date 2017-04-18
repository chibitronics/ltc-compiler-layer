#!/bin/sh -e

source ./00-test-lib.sh

reset_board
# We can't wait for the banner, since we don't know if it'll be there yet.
# However, we know the OS will make the RGB LED dark.
sleep .1

setup_light_sensor
if ! pulse_count room 16
then
	echo "Room too bright.  Shield test jig."
	exit 1
fi

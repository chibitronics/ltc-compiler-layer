#!/bin/sh -e

source ./00-test-lib.sh

echo "Audio test:"
echo "    Download mode"
enter_programming_mode

if get_value ${status_green} || ! get_value ${status_red}
then
	if get_value ${status_green}
	then
		echo "        status_green is on when it should be off"
	fi
	if ! get_value ${status_red}
	then
		echo "        status_red is off when it should be on"
	fi
	echo "        Unable to enter download mode"

	enter_programming_mode
	if get_value ${status_green} || ! get_value ${status_red}
	then
		echo "Tried again, and still failed to enter programming mode"
		exit 1
	fi
	echo "Got it on the second try"
fi

echo "    Programming"
aplay -q "${test_program}"

# Wait for status_green, which gets turned on
# as soon as the program starts running.
if ! get_value ${status_green} || get_value ${status_red}
then
	if ! get_value ${status_green}
	then
		echo "        status_green is off when it should be on"
	fi
	if get_value ${status_red}
	then
		echo "        status_red is on when it should be off"
	fi
	echo "        Program never loaded"
	exit 1
fi

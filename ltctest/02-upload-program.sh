#!/bin/sh -e

source ./00-test-lib.sh

echo "Audio test:"
echo "    Download mode"
enter_programming_mode

if get_value ${status_green} || ! get_value ${status_red}
then
	echo "        Unable to enter download mode"
	exit 1
fi

echo "    Programming"
aplay -q "${test_program}"

# Wait for status_green, which gets turned on
# as soon as the program starts running.
if ! get_value ${status_green} || get_value ${status_red}
then
	echo "        Program never loaded"
	exit 1
fi

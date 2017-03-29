#!/bin/sh -e

source ./00-test-lib.sh

echo "RGB LED tests:"
set_output 1
set_low 1

for color in Red Green Blue
do
	set_high 1
	echo "    ${color}"
	if ! pulse_count rgb 128
	then
		echo "        Pulse out of range: ${range_val}"
		error_count=$((${error_count} + 1))
	else
		echo "        Pulse is in range: ${range_val}"
	fi
	set_low 1

	# Turn off the last remaining pin from the PWM test.
	set_low 0
done

exit ${error_count}

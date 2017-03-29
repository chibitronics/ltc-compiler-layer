#!/bin/sh -e

source ./00-test-lib.sh

set_low 0
set_low 1
set_low 2
set_low 3
set_high 4
set_low 5

echo "PWM LED tests:"
sleep .4
for pin in $(seq 0 5)
do
	signal_pin=$(((${pin}+1)%6))
	echo "    Pin A${pin}"
	set_input ${pin}
	set_output ${signal_pin}
	set_low ${signal_pin}
	if ! pulse_count ${pin} 128
	then
		echo "        Pulse out of range: ${range_val}"
		error_count=$((${error_count} + 1))
	else
		echo "        Pulse is in range: ${range_val}"
	fi
	set_high ${signal_pin}

	set_output ${pin}
	set_low ${pin}
done

exit ${error_count}

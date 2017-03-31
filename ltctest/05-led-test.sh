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
echo l > ${uart}

for pin in $(seq 0 5)
do
	echo "    Pin A${pin}"
	set_input ${pin}
	echo ${pin} > ${uart}
	if ! pulse_count ${pin} 128
	then
		echo "        Pulse out of range: ${range_val}"
		error_count=$((${error_count} + 1))
	else
		echo "        Pulse is in range: ${range_val}"
	fi
done
echo q > ${uart}

exit ${error_count}

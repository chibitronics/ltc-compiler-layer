#!/bin/sh -e

source ./00-test-lib.sh
reset_board
wait_for_banner

set_input 0
set_input 1
set_input 2
set_input 3
set_input 4
set_input 5

echo "PWM LED tests:"
echo l > ${uart}

grep -q "PWM LED test" ${uart}

for pin in $(seq 0 5)
do
	echo "    Pin A${pin}"
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

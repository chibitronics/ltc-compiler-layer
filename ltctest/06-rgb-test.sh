#!/bin/sh -e

source ./00-test-lib.sh
reset_board
wait_for_banner

echo "RGB LED tests:"
echo w > ${uart}

# Wait for the board to enter RGB test mode
grep -q "RGB LED test" ${uart}

for color in Red Green Blue
do
	# Send the color name to the UART, selecting it.
	echo ${color} | cut -c 1 > ${uart}
	echo "    ${color}"
	if ! pulse_count rgb 128
	then
		echo "        Pulse out of range: ${range_val}"
		error_count=$((${error_count} + 1))
	else
		echo "        Pulse is in range: ${range_val}"
	fi
done

echo q > ${uart}
exit ${error_count}

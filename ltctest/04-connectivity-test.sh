#!/bin/sh -e

source ./00-test-lib.sh

reset_board
wait_for_banner

echo "Pin tests:"
pin_num=0

# Enter the pin test mode and wait for the response
echo 'c' > ${uart}
grep -q ' test a particular pad' ${uart}

for pin in 0a 1a 2 3a 4 5 0b 1b 3b
do
	echo "    Pin ${pin} (${pin_num})"
	set_output ${pin}
	set_high ${pin}
	echo ${pin_num} > ${uart}
	sleep .2
	wait_for_green_off

	set_low ${pin}
	sleep .2
	wait_for_green_on
	set_input ${pin}

	pin_num=$((${pin_num}+1))
done

echo q > ${uart}

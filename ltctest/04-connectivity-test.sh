#!/bin/sh -e

source ./00-test-lib.sh

# At this point, the board is waiting for pin 0 to go high.
echo "Pin tests:"
for pin in 0a 0b 1a 1b 2 3a 3b 4 5
do
	echo "    Pin ${pin}"
	set_high ${pin}
	sleep .2
	wait_for_green_on

	set_low ${pin}
	sleep .2
	wait_for_green_off
	set_low ${pin}
done

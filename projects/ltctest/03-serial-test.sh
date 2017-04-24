#!/bin/sh -e

source ./00-test-lib.sh
reset_board
wait_for_banner

# Check to see if serial is working.  Look for the string
# 'LTC factory test is running', and echo back 'q'.
echo "Serial test:"
echo "    Sending"
echo s > ${uart}
echo "    Receiving"
grep -q "serial test" ${uart}
echo q > ${uart}

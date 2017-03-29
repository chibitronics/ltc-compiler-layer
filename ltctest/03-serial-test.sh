#!/bin/sh -e

source ./00-test-lib.sh

# Check to see if serial is working.  Look for the string
# 'test-running', and echo back 'q'.
echo "Serial test:"
stty -F ${uart} ${baud} -icrnl -imaxbel -opost -onlcr -isig -icanon -echo
echo "    Receiving"
grep -q test-running ${uart}
echo "    Sending"
echo q > ${uart}

# Wait for the device to indicate it's ready.  It does
# this by lighting up the red LED
wait_for_green_off
echo "    Send Acknowledged"

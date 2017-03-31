#!/bin/sh -e

source ./00-test-lib.sh

# Check to see if serial is working.  Look for the string
# 'LTC factory test is running', and echo back 'q'.
echo "Serial test:"
stty -F ${uart} ${baud} -icrnl -imaxbel -opost -onlcr -isig -icanon -echo
echo "    Receiving"
grep -q "LTC factory test is running" ${uart}
FROM php:7.1.11-apache

LABEL maintainer="Sean Cross <xobs@kosagi.com>"

COPY . /work

# The suffix (x86_64, armv7l, etc.) should match machine type returned by "uname -m"
ENV ARDUINO_URL_x86_64=http://arduino.cc/download.php?f=/arduino-1.6.13-linux64.tar.xz \
    ARDUINO_SHA256_x86_64=492b28d72b347227346592ceb0373af55558aab67acda343a8a15cc11ade154a \
    TOOLCHAIN_URL_x86_64=http://arduino.cc/download.php?f=/gcc-arm-none-eabi-4.8.3-2014q1-linux64.tar.gz \
    TOOLCHAIN_SHA256_x86_64=d23f6626148396d6ec42a5b4d928955a703e0757829195fa71a939e5b86eecf6 \
    ARDUINO_URL_armv7l=http://arduino.cc/download.php?f=/arduino-1.6.13-linuxarm.tar.xz \
    ARDUINO_SHA256_armv7l=36819d57f86d817605729a38f07702b187701e4415ab115dc659d5cc9c4691bc \
    TOOLCHAIN_URL_armv7l=https://github.com/PaulStoffregen/ARM_Toolchain_2014q1_Source/raw/master/pkg/gcc-arm-none-eabi-4_8-2014q1-20160201-linux.tar.bz2 \
    TOOLCHAIN_SHA256_armv7l=ebe96b34c4f434667cab0187b881ed585e7c7eb990fe6b69be3c81ec7e11e845

RUN true \
 && export ARCH=$(uname -m) \
 && export ARDUINO_URL=$(eval echo \$ARDUINO_URL_${ARCH}) \
 && export ARDUINO_SHA256=$(eval echo \$ARDUINO_SHA256_${ARCH}) \
 && export TOOLCHAIN_URL=$(eval echo \$TOOLCHAIN_URL_${ARCH}) \
 && export TOOLCHAIN_SHA256=$(eval echo \$TOOLCHAIN_SHA256_${ARCH}) \
 && mkdir -p /opt/codebender/ /var/cache/filebkp \
 && curl -sSL -o /arduino.tar.xz "${ARDUINO_URL}" \
 && [ $(sha256sum /arduino.tar.xz | awk '{print $1}') = "${ARDUINO_SHA256}" ] \
 && curl -sSL -o /toolchain.tar.uz "${TOOLCHAIN_URL}" \
 && [ $(sha256sum /toolchain.tar.uz | awk '{print $1}') = "${TOOLCHAIN_SHA256}" ] \
 && mkdir -p /opt/codebender/codebender-arduino-core-files/v167/packages/arduino/tools/arm-none-eabi-gcc \
 && (tar xzf /toolchain.tar.uz -C /opt/codebender/codebender-arduino-core-files/v167/packages/arduino/tools/arm-none-eabi-gcc \
    || tar xjf /toolchain.tar.uz -C /opt/codebender/codebender-arduino-core-files/v167/packages/arduino/tools/arm-none-eabi-gcc)\
 && rm -f /toolchain.tar.uz \
 && mkdir /unpack \
 && tar xJf /arduino.tar.xz -C /unpack \
 && rm -f /arduino.tar.xz \
 && mkdir -p /opt/codebender/codebender-arduino-core-files/v167/hardware/chibitronics/hardware/esplanade/1.6.0/cores/chibitronics-arduino \
 && mkdir -p /opt/codebender/codebender-arduino-core-files/v167/hardware/chibitronics/hardware/esplanade/1.6.0/variants/code \
 && mkdir -p /opt/codebender/codebender-arduino-core-files/v167/hardware/tools/avr \
 && mkdir -p /opt/codebender/codebender-arduino-core-files/v167/libraries \
 && mv /unpack/arduino-1.6.13/arduino-builder /opt/codebender/codebender-arduino-core-files/v167/ \
 && mv /unpack/arduino-1.6.13/tools-builder /opt/codebender/codebender-arduino-core-files/v167/ \
 && mv /unpack/arduino-1.6.13/hardware/platform.txt /opt/codebender/codebender-arduino-core-files/v167/hardware/platform.txt \
 && mv /work/builder/platform.txt /opt/codebender/codebender-arduino-core-files/v167/hardware/chibitronics/hardware/esplanade/1.6.0/platform.txt \
 && mv /work/builder/boards.txt /opt/codebender/codebender-arduino-core-files/v167/hardware/chibitronics/hardware/esplanade/1.6.0/boards.txt \
 && mv /work/builder/programmers.txt /opt/codebender/codebender-arduino-core-files/v167/hardware/chibitronics/hardware/esplanade/1.6.0/programmers.txt \
 && mv /work/builder/app.php /var/www/html/index.php \
 && mv /work/builder/apache2-app-config /usr/local/bin \
 && mv /work/support/Arduino.h //opt/codebender/codebender-arduino-core-files/v167/hardware/chibitronics/hardware/esplanade/1.6.0/cores/chibitronics-arduino/ \
 && mv /work/support/Arduino-types.h /opt/codebender/codebender-arduino-core-files/v167/hardware/chibitronics/hardware/esplanade/1.6.0/cores/chibitronics-arduino/ \
 && mv /work/support/syscalls-app.cpp /opt/codebender/codebender-arduino-core-files/v167/hardware/chibitronics/hardware/esplanade/1.6.0/cores/chibitronics-arduino/ \
 && mv /work/support/* /opt/codebender/codebender-arduino-core-files/v167/hardware/chibitronics/hardware/esplanade/1.6.0/variants/code \
 && rm -rf /work \
 && rm -rf /unpack \
 && a2enmod rewrite \
 && true

CMD ["apache2-app-config"]

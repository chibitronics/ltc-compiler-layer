#!/bin/bash

set -e
version=1.6.9
ltc_modulate_version=v1.3.21

outdir=esplanade/${version}
zipfilename=chibitronics-ltc-${version}.zip

if [ -e ${outdir} ]; then
    echo "Error: output directory '"${outdir}"' exists already"
    exit 1
fi

mkdir -p ltc-modulate
pushd ltc-modulate

urlbase=https://github.com/xobs/ltc-modulate/releases/download/${ltc_modulate_version}
cp ../package_chibitronics_index.json.tmpl ../package_chibitronics_index.json

pkgfile=ltc-modulate-${ltc_modulate_version}-i686-apple-darwin.tar.gz
pkgurl="${urlbase}/${pkgfile}"
rm -f "${pkgfile}"
wget "${pkgurl}"
pkgsha256=$(sha256sum "${pkgfile}" | awk '{print $1}' | tr '[a-z]' '[A-Z]')
pkgsize=$(stat --printf='%s' "${pkgfile}")
sed -i "s!LTCMOD_DARWIN_32_URL!${pkgurl}!g" ../package_chibitronics_index.json
sed -i "s/LTCMOD_DARWIN_32_FILENAME/${pkgfile}/g" ../package_chibitronics_index.json
sed -i "s/LTCMOD_DARWIN_32_SHA256/${pkgsha256}/g" ../package_chibitronics_index.json
sed -i "s/LTCMOD_DARWIN_32_SIZE/${pkgsize}/g" ../package_chibitronics_index.json

pkgfile=ltc-modulate-${ltc_modulate_version}-x86_64-apple-darwin.tar.gz
pkgurl="${urlbase}/${pkgfile}"
rm -f "${pkgfile}"
wget "${pkgurl}"
pkgsha256=$(sha256sum "${pkgfile}" | awk '{print $1}' | tr '[a-z]' '[A-Z]')
pkgsize=$(stat --printf='%s' "${pkgfile}")
sed -i "s!LTCMOD_DARWIN_64_URL!${pkgurl}!g" ../package_chibitronics_index.json
sed -i "s/LTCMOD_DARWIN_64_FILENAME/${pkgfile}/g" ../package_chibitronics_index.json
sed -i "s/LTCMOD_DARWIN_64_SHA256/${pkgsha256}/g" ../package_chibitronics_index.json
sed -i "s/LTCMOD_DARWIN_64_SIZE/${pkgsize}/g" ../package_chibitronics_index.json

pkgfile=ltc-modulate-${ltc_modulate_version}-x86_64-unknown-linux-gnu.tar.gz
pkgurl="${urlbase}/${pkgfile}"
rm -f "${pkgfile}"
wget "${pkgurl}"
pkgsha256=$(sha256sum "${pkgfile}" | awk '{print $1}' | tr '[a-z]' '[A-Z]')
pkgsize=$(stat --printf='%s' "${pkgfile}")
sed -i "s!LTCMOD_LINUX_64_URL!${pkgurl}!g" ../package_chibitronics_index.json
sed -i "s/LTCMOD_LINUX_64_FILENAME/${pkgfile}/g" ../package_chibitronics_index.json
sed -i "s/LTCMOD_LINUX_64_SHA256/${pkgsha256}/g" ../package_chibitronics_index.json
sed -i "s/LTCMOD_LINUX_64_SIZE/${pkgsize}/g" ../package_chibitronics_index.json

pkgfile=ltc-modulate-${ltc_modulate_version}-i686-pc-windows-msvc.zip
pkgurl="${urlbase}/${pkgfile}"
rm -f "${pkgfile}"
wget "${pkgurl}"
pkgsha256=$(sha256sum "${pkgfile}" | awk '{print $1}' | tr '[a-z]' '[A-Z]')
pkgsize=$(stat --printf='%s' "${pkgfile}")
sed -i "s!LTCMOD_WINDOWS_32_URL!${pkgurl}!g" ../package_chibitronics_index.json
sed -i "s/LTCMOD_WINDOWS_32_FILENAME/${pkgfile}/g" ../package_chibitronics_index.json
sed -i "s/LTCMOD_WINDOWS_32_SHA256/${pkgsha256}/g" ../package_chibitronics_index.json
sed -i "s/LTCMOD_WINDOWS_32_SIZE/${pkgsize}/g" ../package_chibitronics_index.json

sed -i "s/LTCMOD_VERSION/${ltc_modulate_version}/g" ../package_chibitronics_index.json

popd

mkdir -p ${outdir}
mkdir -p ${outdir}/cores/love-to-code
mkdir -p ${outdir}/variants/chibi-chip

# Copy files into place
cp ../builder/platform.txt ${outdir}/platform.txt
cp ../builder/boards.txt ${outdir}/boards.txt
cp ../builder/programmers.txt ${outdir}/programmers.txt
cp ../support/Arduino.h ${outdir}/cores/love-to-code/
cp ../support/Arduino-types.h ${outdir}/cores/love-to-code/
cp ../support/syscalls-app.cpp ${outdir}/cores/love-to-code/
cp ../support/* ${outdir}/variants/chibi-chip/

# Remove duplicate files that are part of the ltc base
rm -f ${outdir}/variants/chibi-chip/syscalls-app.cpp
rm -f ${outdir}/variants/chibi-chip/Arduino.h
rm -f ${outdir}/variants/chibi-chip/Arduino-types.h

# Do some last-minute patching
sed -i 's/^code.build.core=.*/code.build.core=love-to-code/' ${outdir}/boards.txt
sed -i 's/^code.build.variant=.*/code.build.variant=chibi-chip/' ${outdir}/boards.txt

# Prepare the zipfile
(rm -f ${zipfilename} && cd esplanade && zip -r ../${zipfilename} ${version})

# Fill in the zipfile specificatiosn for the final index.json file
pkgfile=${zipfilename}
pkgsha256=$(sha256sum "${pkgfile}" | awk '{print $1}' | tr '[a-z]' '[A-Z]')
pkgsize=$(stat --printf='%s' "${pkgfile}")
sed -i "s/LTC_FILENAME/${pkgfile}/g" package_chibitronics_index.json
sed -i "s/LTC_SHA256/${pkgsha256}/g" package_chibitronics_index.json
sed -i "s/LTC_SIZE/${pkgsize}/g" package_chibitronics_index.json
sed -i "s/LTC_VERSION/${version}/g" package_chibitronics_index.json

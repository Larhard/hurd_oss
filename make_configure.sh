#!/bin/sh

PACKAGE_NAME="OSS Translator"
VERSION="0.0"
BUG_REPORT_ADDRESS="larhard@gmail.com"

echo "autoscan"
autoscan || exit

echo "make configure.ac"
sed -e "s/FULL-PACKAGE-NAME/$PACKAGE_NAME/g" \
    -e "s/VERSION/$VERSION/g" \
		-e "s/BUG-REPORT-ADDRESS/$BUG_REPORT_ADDRESS/g" \
		configure.scan > configure.ac || exit

echo "autoconf"
autoconf || exit

echo "add AM_INIT_AUTOMAKE"
echo AM_INIT_AUTOMAKE >> configure.ac || exit

echo "aclocal"
aclocal || exit

echo "autoheader"
autoheader || exit

echo "automake"
automake --add-missing || exit

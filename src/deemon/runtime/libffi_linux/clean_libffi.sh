#!/bin/sh

cp ./make_libffi.sh ../__temp_make_libffi.sh
cp ./clean_libffi.sh ../__temp_clean_libffi.sh

rm -r ./*  > /dev/null 2>&1
rm -r ./.* > /dev/null 2>&1

mv ../__temp_make_libffi.sh  ./make_libffi.sh  > /dev/null 2>&1
mv ../__temp_clean_libffi.sh ./clean_libffi.sh > /dev/null 2>&1
rm ../__temp_make_libffi.sh  > /dev/null 2>&1
rm ../__temp_clean_libffi.sh > /dev/null 2>&1

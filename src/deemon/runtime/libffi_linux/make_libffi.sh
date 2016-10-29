#!/bin/sh

install_path=$(echo $(pwd)/$line)
chmod +x ./../libffi/configure
./../libffi/configure --enable-static --prefix $install_path



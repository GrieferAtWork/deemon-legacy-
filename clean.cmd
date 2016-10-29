@echo off
rem Copyright (c) 2015-2016 Cyder aka Griefer@Work (https://sourceforge.net/users/grieferatwork)
rem
rem This software is provided 'as-is', without any express or implied
rem warranty. In no event will the authors be held liable for any damages
rem arising from the use of this software.
rem
rem Permission is granted to anyone to use this software for any purpose,
rem including commercial applications, and to alter it and redistribute it
rem freely, subject to the following restrictions:
rem
rem 1. The origin of this software must not be misrepresented; you must not
rem    claim that you wrote the original software. If you use this software
rem    in a product, an acknowledgement in the product documentation would be
rem    appreciated but is not required.
rem 2. Altered source versions must be plainly marked as such, and must not be
rem    misrepresented as being the original software.
rem 3. This notice may not be removed or altered from any source distribution.

mkdir src\deemon\runtime\__temp > nul 2>&1
copy src\deemon\runtime\libffi_linux\*.sh src\deemon\runtime\__temp\ > nul 2>&1
rmdir /Q /S src\deemon\runtime\libffi_linux > nul 2>&1
mkdir src\deemon\runtime\libffi_linux > nul 2>&1
move src\deemon\runtime\__temp\* src\deemon\runtime\libffi_linux\ > nul 2>&1
rmdir src\deemon\runtime\__temp > nul 2>&1
rmdir /Q /S build > nul 2>&1
rmdir /Q /S ipch > nul 2>&1
rmdir /Q /S lib\dex > nul 2>&1
rmdir /Q /S lib\dex.100 > nul 2>&1
rmdir /Q /S lib\dex.101 > nul 2>&1
rmdir /Q /S lib\dex.102 > nul 2>&1
rmdir /Q /S lib\dex.103 > nul 2>&1
rmdir /Q /S lib\dex.104 > nul 2>&1
rmdir /Q /S lib\dex.105 > nul 2>&1
del /F /Q      deemon.*               > nul 2>&1
del /F /Q      libdeemon.*            > nul 2>&1
del /F /Q      .last_build_was_static > nul 2>&1
del /F /Q      .vs\_project.sdf       > nul 2>&1
del /F /Q /A H .vs\_project.opensdf   > nul 2>&1
del /F /Q /A H .vs\_project.suo       > nul 2>&1
del /F /Q /A H .vs\_project.v12.suo   > nul 2>&1





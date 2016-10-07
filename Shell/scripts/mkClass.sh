#!/bin/sh

# script to build a generic .cpp class structure
# include class name as comline arg 1

# create files
touch "$1".cpp "$1".h

# fill .h file
echo "#ifndef __$1_H__\n#define __$1_H__\n" >> "$1".h
echo "class $1\n{\n" >> $1.h
echo "private:\n" >> "$1".h
echo "public:\n" >> "$1".h
echo "\t$1();\n" >> "$1".h
echo "\t~$1();\n" >> "$1".h
echo "};\n" >> "$1".h
echo "#endif // __$1_H__" >> "$1".h

# fill .cpp
echo "#include \"$1.h\"\n" >> "$1".cpp
echo "#include <iostream>\n" >> "$1".cpp
echo "$1::$1()\n{}\n" >> "$1".cpp
echo "$1::~$1()\n{}\n" >> "$1".cpp

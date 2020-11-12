#!/usr/bin/env python

# A script to convert .coe files to .hex files.  The .coe file format is used by
# Xilinx tools to initialize memory initialization.  The .hex file format
# (called "Intel Hex" format) is used by Altera tools to initialize memory.

# Author: Matt DeVuyst
# Date: April 5, 2010

import os, sys, re, math
from optparse import OptionParser

usage = """%prog [options] [COE_FILE]...

This program converts .coe files to .hex files.

COE_FILE should be the name of a .coe file to convert.  Unless the -o option is
given, a .hex file with the same prefix will be created (e.g. mem.coe ->
mem.hex)."""

default_bit_width = 34

hex_eof = ":00000001FF"

def read_coe_header (coe_file):
    assert coe_file.readline () == "MEMORY_INITIALIZATION_RADIX=16;\n"
    assert coe_file.readline () == "MEMORY_INITIALIZATION_VECTOR=\n"

def open_coe_file (filename):
    return open (filename, 'r')

def open_hex_file (filename):
    return open (filename, 'w')

def make_checksum (data_width_in_bytes, address, record_type, value):
    result = data_width_in_bytes;
    result = result + (address >> 8)
    result = result + (address & 0xff)
    result = result + record_type
    for i in range (data_width_in_bytes):
        result = result + (value & 0xff)
        value = value >> 8
    result = result & 0xff
    result = 0x100 - result
    result = result & 0xff
    return result

def write_hex_line (hex_file, value, data_width_in_bytes, address, record_type):

    # Write the start code (a colon) to the hex file
    hex_file.write (":")

    # Write the byte count (of the data) to the hex file
    hex_file.write ('{0:02x}'.format (data_width_in_bytes))

    # Write the address (of the data) to the hex file
    hex_file.write ('{0:04x}'.format (address))

    # Write the record type (data record) to the hex file
    hex_file.write ('{0:02x}'.format (record_type))

    # Write the data value to the hex file
    format_string = '{0:0' + str (data_width_in_bytes * 2) + 'x}'
    hex_file.write (format_string.format (value))

    # Write the checksum to the hex file
    checksum = make_checksum (data_width_in_bytes, address, record_type, value)
    hex_file.write ('{0:02x}'.format (checksum))

    # Write a newline to the hex file
    hex_file.write ("\n")

def translate_lines (coe_file, hex_file, data_width_in_bytes):
    address = 0
    for coe_line in coe_file:

        # Get the data value from the coe file
        value = int (re.sub (",\n", "", coe_line), 16)

        record_type = 0
        write_hex_line (hex_file, value, data_width_in_bytes, address,
                        record_type)
        address = address + 1

def write_hex_EOF_record (hex_file):
    hex_file.write (hex_eof);
    hex_file.write ("\n")

def parse_options ():
    parser = OptionParser (usage=usage)
    parser.add_option ("-p", "--pipe", action="store_true", dest="pipe",
                       help="Get input from stdin, and write output to stdout")
    parser.add_option ("-w", "--width", action="store", dest="width",
                       type="int", help="Data with in bits (default: "
                       + str (default_bit_width) + ")",
                       default=default_bit_width)
    parser.add_option ("-o", "--output", action="store", dest="hex_filename",
                       type="string", help="The name of the hex file to write")
    (options, args) = parser.parse_args ()
    
    if not options.pipe:
        if len (args) != 1:
            sys.stderr.write ("Error parsing arguments\n")
            exit (1)
        options.coe_filename = args[0]

    return options

options = parse_options ()

data_width_in_bytes = int (math.ceil (options.width / 8.0))

if not options.pipe:
    coe_filename = options.coe_filename
    if not os.path.isfile (coe_filename):
        sys.stderr.write ("Error: {0} is not a file\n".format (coe_filename))
        exit (1)
    if options.hex_filename:
        hex_filename = options.hex_filename
    else:
        hex_filename = re.sub ("\.coe", ".hex", coe_filename)
    coe_file = open_coe_file (coe_filename)
    hex_file = open_hex_file (hex_filename)
else:
    coe_file = sys.stdin
    hex_file = sys.stdout

read_coe_header (coe_file)
translate_lines (coe_file, hex_file, data_width_in_bytes)
write_hex_EOF_record (hex_file)

#!/bin/bash

echo "gen_misc.sh version 20150511"
echo ""

echo "Please follow below steps(1-5) to generate specific bin(s):"


boot=none
app=0
spi_speed=40
spi_mode=QIO
spi_size_map=0



echo ""
echo "start..."
echo ""

touch user/user_main.c


make COMPILE=gcc BOOT=$boot APP=$app SPI_SPEED=$spi_speed SPI_MODE=$spi_mode SPI_SIZE_MAP=$spi_size_map

#!/bin/bash

set -e

if [ x$ZEPHYR_SDK_INSTALL_DIR == x"" ]; then
	SDK_PATH=$(west sdk list | grep path | tail -n 1 | cut -d ':' -f 2 | tr -d ' ')
	if [ x$SDK_PATH == x ]; then
		echo "ZEPHYR_SDK_INSTALL_DIR not set and no SDK found"
		exit 1
	fi
	echo "ZEPHYR_SDK_INSTALL_DIR not set, using $SDK_PATH"
	export ZEPHYR_SDK_INSTALL_DIR=${SDK_PATH}
fi

board=$1
variant=$2
third_arg=$3
extra_args=""

if [[ $# -eq 0 ]]; then
board=arduino_giga_r1//m7
variant=arduino_giga_r1_m7
fi

source venv/bin/activate

if [ "$third_arg" != "" ]; then
    extra_args="--shield $third_arg"
fi

(west build loader -b $board -p $extra_args && west build -t llext-edk)
(rm -rf variants/$variant/* && tar xfp build/zephyr/llext-edk.tar.xz --directory variants/$variant/)

(cp build/zephyr/zephyr.elf firmwares/zephyr-$variant.elf)
if [ -f build/zephyr/zephyr.bin ]; then
    cp build/zephyr/zephyr.bin firmwares/zephyr-$variant.bin
elif [ -f build/zephyr/zephyr.hex ]; then
    cp build/zephyr/zephyr.hex firmwares/zephyr-$variant.hex
fi

# Generate the provides.ld file for linked builds
echo "Exporting provides.ld"
READELF=${ZEPHYR_SDK_INSTALL_DIR}/arm-zephyr-eabi/bin/arm-zephyr-eabi-readelf
$READELF --wide -s build/zephyr/zephyr.elf  | c++filt  | grep FUNC | awk -F' ' '{print "PROVIDE("$8" = 0x"$2");"}' > variants/$variant/provides.ld
$READELF --wide -s build/zephyr/zephyr.elf  | c++filt  | grep kheap_llext_heap | awk -F' ' '{print "PROVIDE("$8" = 0x"$2");"}' >> variants/$variant/provides.ld
$READELF --wide -s build/zephyr/zephyr.elf  | c++filt  | grep kheap_llext_heap | awk -F' ' '{print "PROVIDE(kheap_llext_heap_size = "$3");"}' >> variants/$variant/provides.ld
$READELF --wide -s build/zephyr/zephyr.elf  | c++filt  | grep kheap__system_heap | awk -F' ' '{print "PROVIDE("$8" = 0x"$2");"}' >> variants/$variant/provides.ld
$READELF --wide -s build/zephyr/zephyr.elf  | c++filt  | grep kheap__system_heap | awk -F' ' '{print "PROVIDE(kheap__system_heap_size = "$3");"}' >> variants/$variant/provides.ld
cat build/zephyr/zephyr.map | grep __device_dts_ord | grep -v rodata | grep -v llext_const_symbol |  awk -F' ' '{print "PROVIDE("$2" = "$1");"}'  >> variants/$variant/provides.ld
TEXT_START=`cat loader/boards/$variant.overlay | grep user_sketch: | cut -f2 -d"@" | cut -f1 -d"{"`
echo "PROVIDE(_sketch_start = 0x$TEXT_START);" >> variants/$variant/provides.ld

sed -i 's/PROVIDE(malloc =/PROVIDE(__wrap_malloc =/g' variants/$variant/provides.ld
sed -i 's/PROVIDE(free =/PROVIDE(__wrap_free =/g' variants/$variant/provides.ld
sed -i 's/PROVIDE(realloc =/PROVIDE(__wrap_realloc =/g' variants/$variant/provides.ld
sed -i 's/PROVIDE(calloc =/PROVIDE(__wrap_calloc =/g' variants/$variant/provides.ld
sed -i 's/PROVIDE(random =/PROVIDE(__wrap_random =/g' variants/$variant/provides.ld

cmake -P extra/gen_arduino_files.cmake $variant

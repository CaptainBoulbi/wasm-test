#!/bin/bash

export_sym="init draw key_pressed key_released set_velocity set_mouse BUFFER width height"
export_cmd=""
for e in $export_sym; do
    export_cmd="$export_cmd -Wl,--export=$e";
done;

if [[ "$1" == "" ]]; then
    f='app'
    a='app.c'
else
    f=$(echo $1 | sed "s/\..*$//g")
    a=$1
fi

set -xe

clang png2c.c -o png2c -lm
./png2c "penger.png" > penger.c
./png2c "hand.png" > hand.c

clang -O2 --target=wasm32 -fno-builtin -nostdlib --no-standard-libraries -Wl,--no-entry $export_cmd -Wl,--allow-undefined -o $f.wasm $a

wasm2wat $f.wasm > $f.wat

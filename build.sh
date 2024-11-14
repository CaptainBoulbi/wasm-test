#!/bin/bash

export_sym="init go key_pressed key_released set_velocity BUFFER width height"
export_cmd=""
for e in $export_sym; do
    export_cmd="$export_cmd -Wl,--export=$e";
done;

if [[ "$1" == "" ]]; then
    f='app'
    e='app.c'
else
    f=$(echo $1 | sed "s/\..*$//g")
    e=$1
fi

set -xe

clang png2c.c -o png2c -lm
./png2c "penger.png" > penger.c

clang -O2 --target=wasm32 -fno-builtin -nostdlib --no-standard-libraries -Wl,--no-entry $export_cmd -Wl,--allow-undefined -o $f.wasm $e

wasm2wat $f.wasm > $f.wat

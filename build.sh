#!/bin/bash

export_sym="init go BUFFER width height"
export_cmd=""
for e in $export_sym; do
    export_cmd="$export_cmd -Wl,--export=$e";
done;

f=$(echo $1 | sed "s/\..*$//g")

clang -O2 --target=wasm32 -fno-builtin -nostdlib --no-standard-libraries -Wl,--no-entry $export_cmd -Wl,--allow-undefined -o $f.wasm $1

wasm2wat $f.wasm > $f.wat

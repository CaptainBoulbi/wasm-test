#!/bin/bash

if [[ "$1" == "clear" ]]; then
    rm -vfr museum.c pengers.h hand.c app.wasm png2c app.wat index.html
    exit
fi;

export_sym="init draw key_pressed key_released set_velocity set_mouse BUFFER width height id"
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

#set -xe

clang png2c.c -o png2c -lm
mkdir -p museum.c
rm -f museum.c/*

pengers_html=$'\n'
pengers_include=$'\n'
id=0
for p in $(ls museum); do
    file=$(echo $p | sed "s/\.png$//g")
    ./png2c "museum/"$p $id > museum.c/$file.c
    pengers_html+=$'            <img src="museum/'$p'" class="penger-img" penger-id="'$id'"></img>\n'
    pengers_include+='#include "museum.c/'$file$'.c"\n'
    ((id=id+1))
done

echo "int pengers_height[$id];" > pengers.h
echo "int pengers_width[$id];" >> pengers.h
echo "unsigned int *pengers_img[$id];" >> pengers.h
echo "$pengers_include" >> pengers.h
echo "void pengers_init(void) {" >> pengers.h
((id=id-1))
for i in $(seq 0 $id); do
    echo "    penger_init_$i();" >> pengers.h;
done
echo "}" >> pengers.h

echo -e "$pengers_html" > pengers_image.html.temp
sed -e '/Choose your penger:/rpengers_image.html.temp' index.html.template > index.html
rm pengers_image.html.temp

./png2c "hand.png" > hand.c

clang -O2 --target=wasm32 -fno-builtin -nostdlib --no-standard-libraries -Wl,--no-entry $export_cmd -Wl,--allow-undefined -o $f.wasm $a

wasm2wat $f.wasm > $f.wat

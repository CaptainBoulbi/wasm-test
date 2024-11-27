#!/bin/bash

if [[ "$1" == "clear" ]]; then
    rm -vfr out/*
    touch out/.gitkeep
    exit
fi;

export_sym="init draw key_pressed key_released set_velocity set_mouse get_pos_x get_pos_y draw_player deco_player reset_collisions add_collisions reset_coins add_coin set_default_map BUFFER width height id nb_players dir"
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

clang png2c.c -o out/png2c -lm
mkdir -p out/museum.c
rm -f out/museum.c/*

pengers_html=$'\n'
pengers_include=$'\n'
id=0
for p in $(ls static/museum/); do
    file=$(echo $p | sed "s/\.png$//g")
    ./out/png2c "static/museum/"$p $id > out/museum.c/$file.c
    pengers_html+=$'                <img src="static/museum/'$p'" class="penger-img" penger-id="'$id'"></img>\n'
    pengers_include+='#include "museum.c/'$file$'.c"\n'
    ((id=id+1))
done

echo "int pengers_height[$id];" > out/pengers.h
echo "int pengers_width[$id];" >> out/pengers.h
echo "unsigned int *pengers_img[$id];" >> out/pengers.h
echo "$pengers_include" >> out/pengers.h
echo "void pengers_init(void) {" >> out/pengers.h
((id=id-1))
for i in $(seq 0 $id); do
    echo "    penger_init_$i();" >> out/pengers.h;
done
echo "}" >> out/pengers.h

echo -e "$pengers_html" > pengers_image.html.temp
sed -e '/<!-- penger images src -->/rpengers_image.html.temp' templates/index.html.template > out/index.html
rm pengers_image.html.temp

./out/png2c "static/assets/hand.png" > out/hand.c
./out/png2c "static/assets/coin.png" > out/coin.c

clang -O3 --target=wasm32 -fno-builtin -nostdlib --no-standard-libraries -Wl,--no-entry $export_cmd -Wl,--allow-undefined -o out/$f.wasm $a

wasm2wat out/$f.wasm > out/$f.wat

cp templates/leaderboard.json.template out/leaderboard.json
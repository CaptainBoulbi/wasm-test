var scale = 2;

var global_instance;
var global_memory;

function wasm_variable(name)
{
    return global_memory[global_instance.exports[name].value / 4];
}
function wasm_set_variable(name, val)
{
    return global_memory[global_instance.exports[name].value / 4] = val;
}
function wasm_function(name)
{
    return global_instance.exports[name];
}

window.onload = () => {
    var canvas = document.getElementById("demo-canvas");
    var pengers_img = document.getElementsByClassName('penger-img');
    canvas.onmousemove = (e) => {
        var r = canvas.getBoundingClientRect();
        wasm_function('set_mouse')(e.clientX - r.x, e.clientY - r.y);
    }
    for (var i = 0; i < pengers_img.length; i++) {
        pengers_img[i].onclick = (e) => {wasm_set_variable('id', e.target.getAttribute('penger-id'))}
    }
};

(async() => {

// jsp, je l'ai pris de la: https://github.com/tsoding/olive.c
function make_environment(...envs) {
    return new Proxy(envs, {
        get(target, prop, receiver) {
            for (let env of envs) {
                if (env.hasOwnProperty(prop)) {
                    return env[prop];
                }
            }
            return (...args) => {console.error("NOT IMPLEMENTED: "+prop, args)}
        }
    });
}

const { instance } = await WebAssembly.instantiateStreaming(fetch("./app.wasm"), {
    "env": make_environment({
        // importer les fonctions dans le wasm
        'random': Math.random,
        'sqrtf': Math.sqrt,
        'get_scale': () => {return scale},
    })
});

const memory = new Uint32Array(instance.exports.memory.buffer);

global_instance = instance;
global_memory = memory;

const width = wasm_variable('width');
const height = wasm_variable('height');

const canvas = document.getElementById("demo-canvas");
canvas.width = width;
canvas.height = height;

// recup image de la memoire du wasm
const buffer_address = instance.exports.BUFFER.value;
const image = new ImageData(
    new Uint8ClampedArray(
        instance.exports.memory.buffer,
        buffer_address,
        4 * width * height,
    ),
    width,
);

const ctx = canvas.getContext("2d");

wasm_function('init')();

let prev = null;
function first(timestamp) {
    wasm_set_variable('id', Math.random() * document.getElementsByClassName('penger-img').length);
    prev = timestamp;
    wasm_function('draw')(0.16);
    window.requestAnimationFrame(loop);
}
function loop(timestamp) {
    const dt = timestamp - prev;
    prev = timestamp;

    wasm_function('draw')(dt/1000);
    ctx.putImageData(image, 0, 0);
    window.requestAnimationFrame(loop);
}
window.requestAnimationFrame(first);

addEventListener('keydown', (e) => {
    wasm_function('key_pressed')(e.keyCode);
    if(["Space","ArrowLeft","ArrowRight"].indexOf(e.code) > -1) {
        e.preventDefault();
    }
});

addEventListener('keyup', (e) => {
    wasm_function('key_released')(e.keyCode);
});

})()

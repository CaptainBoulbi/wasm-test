var scale = 7;

var global_instance;

window.onload = () => {
    var scale_but = document.getElementById("scaling");
    var descale_but = document.getElementById("descaling");
    var scale_p = document.getElementById("scale");
    var reset_but = document.getElementById("reset");
    scale_p.innerText = "scale: " + scale;
    scale_but.onclick = () => {
        scale++;
        scale_p.innerText = "scale: " + scale;
    }
    descale_but.onclick = () => {
        scale--;
        scale_p.innerText = "scale: " + scale;
    }
    reset_but.onclick = () => {
        global_instance.exports.set_velocity(0, 0);
    }
};

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

(async() => {

const { instance } = await WebAssembly.instantiateStreaming(fetch("./app.wasm"), {
    "env": make_environment({
        'random': Math.random,
        'get_scale': () => {return scale},
    })
});

global_instance = instance;

const memory = new Uint32Array(instance.exports.memory.buffer);

const width = memory[instance.exports.width.value / 4];
const height = memory[instance.exports.height.value / 4];

const canvas = document.getElementById("demo-canvas");
canvas.width = width;
canvas.height = height;

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

instance.exports.init();

let prev = null;
function first(timestamp) {
    prev = timestamp;
    instance.exports.go(0.16);
    window.requestAnimationFrame(loop);
}
function loop(timestamp) {
    const dt = timestamp - prev;
    prev = timestamp;

    instance.exports.go(dt/1000);
    ctx.putImageData(image, 0, 0);
    window.requestAnimationFrame(loop);
}
window.requestAnimationFrame(first);

addEventListener('keydown', (e) => {
    instance.exports.key_pressed(e.keyCode);
});

addEventListener('keyup', (e) => {
    instance.exports.key_released(e.keyCode);
});

})()

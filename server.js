const fs = require('fs');
const http = require("http");
const ws = require('ws');

const HTTP_PORT = 6969;
const WS_PORT = 4242;

function update_player_list()
{
    var list = "";
    sockets.forEach((s) => {
        if (s.game.pseudo == undefined || s.game.pseudo == "") return;
        list += "<li><img class='players-img' penger-id='"+s.game.id+"' src='museum/Penger.png'>" + s.game.pseudo + "</li>";
    });
    sockets.forEach((s) => {
        if (s.game.pseudo == undefined || s.game.pseudo == "") return;
        s.send('{"name": "pseudo", "value": "'+list+'"}');
    });
}

var update_time_ms = 16;
function update_player_pos()
{
    var list = [];
    sockets.forEach((s) => {
        if (s.game.pseudo == undefined || s.game.pseudo == "") return;
        list[list.length] = {rid: s.game.rid, id: s.game.id, x: s.game.x, y: s.game.y, dir: s.game.dir};
    });
    sockets.forEach((s) => {
        if (s.game.pseudo == undefined || s.game.pseudo == "") return;
        s.send('{"name": "pos", "value": '+JSON.stringify(list)+'}');
    });
    setTimeout(update_player_pos, update_time_ms);
}
setTimeout(update_player_pos, update_time_ms);

var current_map = 0;
var map_every_ms = 30 * 1000;
function send_new_map()
{
    var map = maps[Math.floor(Math.random() * maps.length)];
    map.next_at = Date.now() + map_every_ms;
    sockets.forEach((s) => {
        if (s.game.pseudo == undefined || s.game.pseudo == "") return;
        s.send('{"name": "map", "value": '+JSON.stringify(map)+'}');
    });
    setTimeout(send_new_map, map_every_ms);
}
setTimeout(send_new_map, map_every_ms);
function send_map(socket)
{
    socket.send('{"name": "map", "value": '+JSON.stringify(maps[current_map])+'}'); 
}

const requestListener = function (req, res) {
    var url = req.url;

    if (url == "/" || url == "/index.html") {
        res.setHeader("Content-Type", "text/html");
        res.writeHead(200);
        res.end(fs.readFileSync("./index.html"));
        return;
    }

    if (url == "/app.wasm") {
        res.setHeader("Content-Type", "application/wasm");
        res.writeHead(200);
        res.end(fs.readFileSync("." + url));
        return;
    }

    if (url == "/load.js") {
	    res.setHeader("Content-Type", "text/javascript");
        res.writeHead(200);
        res.end(fs.readFileSync("." + url));
        return;
    }

    if (url.startsWith("/museum/")) {
	    res.setHeader("Content-Type", "image/png");
        res.writeHead(200);
        res.end(fs.readFileSync("." + url));
        return;
    }

    res.setHeader("Content-Type", "text/html");
    res.writeHead(404);
    res.end(`Page not found`);
};

const http_server = http.createServer(requestListener);
http_server.listen(HTTP_PORT, () => {
    console.log(`Server is running on port ${HTTP_PORT}`);
});

var maps = JSON.parse(fs.readFileSync("maps.json"));

const ws_server = new ws.Server({ port: WS_PORT });

var global_id = 0;

var sockets = [];
ws_server.on('connection', (socket) => {
    socket.game = {};
    socket.game.coin = 0;
    socket.game.rid = global_id++;

    socket.send('{"name": "rid", "value": '+socket.game.rid+'}');

    sockets.push(socket);
    console.log("[", sockets.length, "] connect");

    socket.on('message', (msg) => {
        var msg_str = Buffer.from(msg).toString('latin1');
        var req = JSON.parse(msg_str);
        if (req.name == "pseudo") {
            if (socket.game.pseudo == undefined || socket.game.pseudo == "" && req.value != "") send_map(socket);
            socket.game.pseudo = req.value;
            if (req.value == "") sockets.forEach((s) => { s.send('{"name": "disconnect", "value": '+socket.game.rid+'}') });
            update_player_list();
        }
        else if (req.name == "id") {
            socket.game.id = req.value;
            update_player_list();
        }
        else if (req.name == "pos") {
            socket.game.x = req.x;
            socket.game.y = req.y;
            socket.game.dir = req.dir;
        }
        else if (req.name == "coin") {
            socket.game.coin++;
        }
    });

    socket.on('close', () => {
        sockets.forEach((s) => { s.send('{"name": "disconnect", "value": '+socket.game.rid+'}') });
        sockets = sockets.filter(s => s !== socket);
        console.log("[", sockets.length, "] close");
        update_player_list();
    });
});

const fs = require('fs');
const http = require("http");
const ws = require('ws');

const HTTP_PORT = 6969;
const WS_PORT = 4242;

function update_player_list()
{
    var list = "";
    sockets.forEach((s) => { if (s.game.pseudo == undefined || s.game.pseudo == "") return; list += "<li><img class='players-img' penger-id='"+s.game.id+"' src='museum/Penger.png'>" + s.game.pseudo + "</li>"; });
    sockets.forEach((s) => { s.send('{"name": "pseudo", "value": "'+list+'"}'); });
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

const ws_server = new ws.Server({ port: WS_PORT });

var sockets = [];
ws_server.on('connection', (socket) => {
    socket.game = {};
    sockets.push(socket);
    console.log("connect: ", sockets.length );

    socket.on('message', (msg) => {
        var msg_str = Buffer.from(msg).toString('latin1');
        var req = JSON.parse(msg_str);
        socket.game[req.name] = req.value;
        update_player_list();
        console.log(socket.game);
    });

    socket.on('close', () => {
        sockets = sockets.filter(s => s !== socket);
        console.log("close: ", sockets.length);
        update_player_list();
    });
});

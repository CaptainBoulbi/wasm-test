const fs = require('fs');

var leaderboard = JSON.parse(fs.readFileSync("out/leaderboard.json"));
leaderboard.today = [
    {name: 'none', value: '0'},
    {name: 'none', value: '0'},
    {name: 'none', value: '0'},
    {name: 'none', value: '0'},
    {name: 'none', value: '0'}
];
fs.writeFile('out/leaderboard.json', JSON.stringify(leaderboard), ()=>{});

const fs = require('fs');

var leaderboard = JSON.parse(fs.readFileSync("leaderboard.json"));
leaderboard.today = [
    {name: 'none', value: '0'},
    {name: 'none', value: '0'},
    {name: 'none', value: '0'},
    {name: 'none', value: '0'},
    {name: 'none', value: '0'}
];
fs.writeFile('leaderboard.json', JSON.stringify(leaderboard), ()=>{});

var chalk = require('chalk');

function getTime() {
    var now = new Date(),
        time = [now.getHours(), now.getMinutes(), now.getSeconds()];

    for(var i = 0; i < 3; i++) {
        if(time[i] < 10) {
            time[i] = "0" + time[i];
        }
    }

    return '[' + time.join(":") + '] ';
}

// Exports
module.exports = function(type, message) {
    switch(type) {
        case 'server':
            type = chalk.bold.green.dim('[Server] ');
            break;

        case 'disconnect':
            type = chalk.bold.red.dim('[Stop] ');
            break;

        case 'error':
            type = chalk.bold.red.dim('[Erro] ');
            break;

        case 'tcperror':
            type = chalk.bold.red.dim('[TCP] Erro: ');
            break;

        case 'data':
            type = chalk.bold.blue('[DATA] ');
            break;

        case 'client':
            type = chalk.bold.bgGreen('[CLIENTE]') + ' ';
            break;

        case 'TCP':
            type = chalk.bold.magenta('[TCP] ');
            break;
    }

    console.log(getTime() + type + message);
}


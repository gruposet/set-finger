var net = require('net');
var chalk = require('chalk');
var log = require('./lib/log.js');
var Sequelize = require('sequelize');
var sequelize = new Sequelize('mysql://root:0546@localhost:3306/setserver', { logging: false } );
var config = {};
    config.port = 7000;
    config.hwid = 'GT-SET';
    config.name = 'GT-SET';

var fingerServer = net.createServer(setServer)
    .listen(config.port, () => log('server', "O servidor está sendo iniciado..."))
    .on('connection', (data) => log('TCP', 'Conexão estabelecida de ' + chalk.underline(data.remoteAddress + ':' + data.remotePort)))
    .on('listening', (data) => log('server', 'O servidor foi iniciado em ' + chalk.underline(fingerServer.address().address + ':' + fingerServer.address().port)))
    .on('error', (err) => errorHandler(err));

function setServer(sock){
    var sessionId;
    var lastID;
    sock.setKeepAlive(true, 30000)
    .on('data', (data) => dataParser(data, sock))
    .on('close', (data) => errorHandler(data))
    .on('error', (err) => errorHandler(err));
}

function dataParser(data, sock){
    try{
       data = JSON.parse(data);
    } catch(err){
        errorHandler(err);
    }
    if(data.type === 'conn'){
        log('client', 'O cliente \"' + data.hwid + '\" está tentando se conectar');
        if(data.hwid == config.hwid){
            sessionId = data.hwid;
            sendMessage(sock, 'authok')
            log('server', 'O cliente \"' + data.hwid + '\" foi autenticado com sucesso!');
        }
    }
    if(sessionId){
        switch(data.type){
            case 'fingerid':
                sendMessage(sock, 'fingerid', data.id)
                break;
            case 'addfinger':
                sendMessage(sock, 'addfinger')
                break;
            case 'registerok':
                saveUser(lastID);
                break;
            case 'registerfail':
                log('server', 'Falha na tentativa de registro do novo usuário! ID: ' + lastID);
                break;
        }
    }
}

function errorHandler(err){
    switch(err.code){
        case 'EADDRINUSE':
            log('tcperror', chalk.underline('A porta ' + err.port + ' está em uso.'));
            break;
        default:
            log('error', 'Erro desconhecido: ' + err.code);
            break;
    }
}

function sendMessage(sock, type, data){
    switch(type){
        case 'authok':
            sock.write(JSON.stringify({ type: 'conn', 'auth': 'ok', name: config.name }));
            break;
        case 'authfail':
            sock.write(JSON.stringify({ type: 'conn', 'auth': 'fail' }));
            break;
        case 'addfinger':
            getLastID(data, (data) => sock.write(data));
            break;
        case 'fingerid':
            checkFinger(data, (data) => sock.write(data));
            break;
    }
}

function checkFinger(id, fn){
    sequelize.query('SELECT * from users WHERE finger_id=' + id).spread(function(results, metadata) {
        if(results[0]){
            fn(JSON.stringify({ type: "auth", auth: "ok", admin: results[0].access_level, name: results[0].name }));
            log('client', "Nome: " + results[0].name + " | ID biométrico: " + results[0].finger_id);
            sequelize.query(`INSERT INTO history (din, finger_id, name, timestamp) VALUES ('${results[0].din}', ${id}, '${results[0].name}', ${Math.floor(new Date() / 1000)})`);	    
        } else {
            fn(JSON.stringify({ type: "auth", auth: "fail" }));
            log('error', 'Usuário não autorizado! ID: ' + id);
        }
    })
    
}

function getLastID(id, fn){
    sequelize.query('SELECT * FROM `ids` WHERE available = 1 LIMIT 1').spread(function(results, metadata) {
        fn(JSON.stringify({ type: "register", id: results[0].finger_id }));
        lastID = results[0].finger_id;
    })
    log('server', 'O ID: ' + id + ' foi enviado para cadastro de novo usuário!');
}

function saveUser(id){
    sequelize.query('INSERT INTO users (name, finger_id) VALUES (\'NOVO USUARIO\', \'' + id + '\')');
    sequelize.query('UPDATE `ids` SET available=0 WHERE finger_id=' + lastID);
    log('server', 'Novo usuário cadastrado com sucesso! ID: ' + lastID);
}

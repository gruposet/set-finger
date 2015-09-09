var net = require('net');
var mysql = require('mysql');
var sockets = [];
var string;
var fingerServer = net.createServer(newSocket);
var fingerDBpool = mysql.createPool({
    host: 'IP DO SERVIDOR MYSQL',
    user: 'root',
    password: 'SENHA',
    database: 'NOME DA DB'
});
console.log("**************************************************************");
console.log("**                     Nodefinger Server                    **");
console.log("**************************************************************");
fingerServer.listen(7000);
console.log(">Servidor iniciado, aguardando conexões dos clientes...");

function cleanInput(data) {
    return data.toString().replace(/(\r\n|\n|\r)/gm, "");
}

function newSocket(socket) {
    console.log(">Cliente conectado");
    socket.on('data', function(data) {
        fingerDBpool.getConnection(function(err, fingerDB) {
        if(err) { 
            console.log(err); 
            callback(true); 
            return; 
        }
        var fingerData = cleanInput(data).toString().split(".");
        if (fingerData[2] == "0") {
            fingerDB.query('SELECT * from users WHERE fingerid=' + fingerData[1], function(err, dbData, fields) {
                 if (!err && dbData != 0) {
                    (dbData[0].admin && dbData[0].active) ? socket.write("1"): socket.write("0");
                    console.log(">Nome: " + dbData[0].name + " | ID do usuário: " + dbData[0].userid + " | ID biométrico: " + dbData[0].fingerid);
                    var post = {
                        fingerid: fingerData[1],
                        timestamp: Math.floor(new Date() / 1000)
                    };
                    var query = fingerDB.query('INSERT INTO history SET ?', post, function(err, result) {
                       fingerDB.release();
                    });
                } else if (!err && dbData == 0) {
                    socket.write("2")
                    console.log(">Usuário não encontrado! FINGER ID:" + fingerData[1]);
                } else {
                    socket.write("2");
                    console.log(">Usuário não encontrado!");
                }
            });
        } else if (fingerData[2] == "2") {
            console.log(">Usuário deletado! Finger ID: " + fingerData[1]);
            var post = {
                fingerid: fingerData[1]
            };
            var query = fingerDB.query('DELETE FROM users WHERE ?', post, function(err, result) {
               fingerDB.release();
            });
        } else {
            console.log(">Novo usuário cadastrado! ID: " + fingerData[0]);
            var post = {
                userid: fingerData[0],
                fingerid: fingerData[1],
                name: "CHANGE ME"
            };
            var query = fingerDB.query('INSERT INTO users SET ?', post, function(err, result) {
              fingerDB.release();
            });
        }
        });
    });
    socket.on('error', function(err) {
        if (err)
            console.log(">Cliente desconectado");
    });
    socket.on('end', function() {
        console.log(">Cliente desconectado");
    });
    setInterval(function() {
        socket.write(new Buffer(0));
    }, 30000);
    
}

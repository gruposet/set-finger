var net = require('net');
var mysql = require('mysql');
var fingerDBpool = mysql.createPool({
    host: '107.170.7.193',
    user: 'root',
    password: 'fsdfsdfsd',
    database: 'nodefinger'
});

var hostip = '192.168.14.22';
var port = 7000;

net.createServer(function(sock) {
    var sessionStarted = false;
    console.log('[TCP] Connection from ' + sock.remoteAddress +':'+ sock.remotePort);
    console.log('[TCP] Waiting for authentication from client...')
    sock.on('data', function(data) {
        if(!sessionStarted){
            data = JSON.parse(data);
            if(data.type == 'conn'){
                if(data.hwid == 'GT-SET'){
                    //VALID!
                    console.log("[TCP] Auth OK! Valid access from ID: " + data.hwid);
                    sock.write(JSON.stringify({type: 'conn', 'auth': 'ok', name: "GT-SET"}));
                    setInterval(function(){
                        if(sessionStarted){
                            sock.write(JSON.stringify({type: "ping"}));
                        }
                    }, 50000);
                    sessionStarted = true;
                }
                else{
                    console.log("[TCP] Auth FAIL! Invalid access from ID: " + data.hwid);
                    sock.write(JSON.stringify({type: 'conn', 'auth': 'fail'}));
                    
                }
            }
        }
        else{
            console.log('DATA ' + sock.remoteAddress + ': ' + data);
            data = JSON.parse(data);
            switch(data.type) {
                case "fingerid":
                    checkID(data.id);
                    break
                case "pong":
                    //console.log("KEEPALIVE OK");
                    break;
                case "addfinger":
                    sock.write(JSON.stringify({type: "register", id: 28}));
                    break;
                default:
                    //sock.write(JSON.stringify(data));
                    break;
            }
        };
    });
    

    sock.on('error', function(data) {
        sessionStarted = !sessionStarted;
        console.log('[TCP] Closed connection from ' + sock.remoteAddress +':'+ sock.remotePort);
    });
    
    function checkID(id){
    fingerDBpool.getConnection(function(err, fingerDB) {
        if(err) { 
            console.log(err); 
            callback(true); 
            return; 
        }
        fingerDB.query('SELECT * from users WHERE fingerid=' + id, function(err, dbData, fields) {
                 if (!err && dbData != 0 && id>-1) {
                     console.log(">Nome: " + dbData[0].name + " | ID do usuário: " + dbData[0].userid + " | ID biométrico: " + dbData[0].fingerid);
                    sock.write(JSON.stringify({type: "auth", auth: "ok", admin: dbData[0].admin, name: dbData[0].name}));
                
                    var post = {
                        fingerid: id,
                        timestamp: Math.floor(new Date() / 1000)
                    };
                    var query = fingerDB.query('INSERT INTO history SET ?', post, function(err, result) {
                       fingerDB.release();
                    });
                } else if (!err && dbData == 0 && id>-1) {
                    sock.write(JSON.stringify({type: "auth", auth: "fail"}));
                    console.log(">Usuário não encontrado! FINGER ID:" + id);
                } else {
                    sock.write(JSON.stringify({type: "auth", auth: "fail"}));
                    console.log(">Usuário não cadastrado!");
                }
            });
        });
    }
    
    
}).listen(port);


console.log('Server listening on ' + hostip +':'+ port);
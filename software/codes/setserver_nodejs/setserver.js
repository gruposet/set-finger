var net = require('net');
var mysql = require('mysql');
var fingerDBpool = mysql.createPool({           //Cria pool de conexão para query no Banco de Dados
    host: '127.0.0.1',
    user: 'root',
    password: 'SENHA',
    database: 'nodefinger'
});
var hostip = '192.168.14.22';
var port = 7000;
net.createServer(function(sock) {               //Código do socket TCP
    var lastID;
    var sessionStarted = false;
    console.log('[TCP] Connection from ' + sock.remoteAddress +':'+ sock.remotePort);
    console.log('[TCP] Waiting for authentication from client...')
    sock.on('data', function(data) {
        if(!sessionStarted){
            data = JSON.parse(data);            //Interpreta a string JSON enviado pela tranca e transforma em um objeto
            if(data.type == 'conn'){
                if(data.hwid == 'GT-SET'){      //ID da tranca
                    //Sessão válida
                    console.log("[TCP] Auth OK! Valid access from ID: " + data.hwid);
                    sock.write(JSON.stringify({type: 'conn', 'auth': 'ok', name: "GT-SET"})); //Envia mensagem de sucesso junto com o nome de boas-vindas
                    setInterval(function(){     //Função de KeepAlive, envia um ping a cada 50 segundos.
                        if(sessionStarted){
                            sock.write(JSON.stringify({type: "ping"}));
                        }
                    }, 50000);
                    sessionStarted = true;      //Inicia a sessão
                }
                else{
                    //Sessão inválida
                    console.log("[TCP] Auth FAIL! Invalid access from ID: " + data.hwid);
                    sock.write(JSON.stringify({type: 'conn', 'auth': 'fail'})); //ID inválido, manda mensagem de falha
                }
            }
        }
        else{
            console.log('DATA ' + sock.remoteAddress + ': ' + data);        //Interpreta os tipos de mensagem que a tranca envia
            data = JSON.parse(data);
            switch(data.type) {
                case "fingerid":
                    checkID(data.id);
                    break
                case "pong":
                    //console.log("KEEPALIVE OK");
                    break;
                case "addfinger":
                    getlastID();
                    break;
                case "registerok":
                    saveUser();
                default:
                    //sock.write(JSON.stringify(data));
                    break;
            }
        };
    });
    sock.on('error', function(data) {                   //Mensagem disparada quando a traca desconecta
        sessionStarted = !sessionStarted;
        console.log('[TCP] Closed connection from ' + sock.remoteAddress +':'+ sock.remotePort);
    });

    function checkID(id){                               //Função que checa o ID enviado pela tranca
        if(id<0){                                       //Caso a ID seja -1(não encontrada) ele envia mensagem de falhae retorna
            sock.write(JSON.stringify({type: "auth", auth: "fail"}));
            console.log(">Usuário não cadastrado!");
            return 0;
        }
        fingerDBpool.getConnection(function(err, fingerDB) {    //Cria conexão com o pool
            if(err) {
                console.log(err);
                //callback(true);
                return;
            }
        fingerDB.query('SELECT * from users WHERE fingerid=' + id, function(err, dbData, fields) { //Busca no banco de dados por uma ID específica
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
    function getlastID(){       //Manda a última ID somada a 1 para registro de novo usário
        fingerDBpool.getConnection(function(err, fingerDB) {
            if(err) {
                console.log(err);
                //callback(true);
                return;
            }
            fingerDB.query('SELECT id FROM users ORDER BY id DESC LIMIT 1;', function(err, dbData, fields) {
                lastID = dbData[0].id + 1;
                sock.write(JSON.stringify({type: "register", id: lastID}));
            });
        });
    }
    function saveUser(){        //Salvar usuário no banco de dados
        fingerDBpool.getConnection(function(err, fingerDB) {
            if(err) {
                console.log(err);
                //callback(true);
                return;
            }
            console.log(">Novo usuário cadastrado! ID: " + lastID);
            var post = {
                userid: "0000000000000000",
                fingerid: lastID,
                name: "NOVO USUARIO"
            };
            var query = fingerDB.query('INSERT INTO users SET ?', post, function(err, result) {
              fingerDB.release();
            });
        });
    }
}).listen(port);            //Inicia o socket na porta específica
console.log('/************ SET-SERVER 2.0 ***************/');
console.log('/******** INICIADO NA PORTA: '+port+' **********/');
console.log('/*******************************************/');

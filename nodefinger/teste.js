 
function getUser(fingerid){
    fingerDB.query('SELECT * from users WHERE fingerid='+ fingerid, function(err, dbData, fields) {
    if (!err && dbData!=0){
      return {
	name: dbData[0].name, 
	userid: dbData[0].userid, 
	admin: dbData[0].admin, 
	dbid: dbData[0].id, 
	active: dbData[0].active,
	err: 0
      }
    }
    else if(!err && dbData==0) {
     return {
	err: 1
      }
    }
    else{
     return {
	err: 1
      }
     }
  }); 
}
"use strict";

module.exports = function(sequelize, DataTypes) {
  var user = sequelize.define('user', {
    name: {
        type: DataTypes.STRING,
    },
    userid: {
        type: DataTypes.STRING
    },
    fingerid: {
        type: DataTypes.INT
    },
    admin: {
        type: DataTypes.INT
    },
    active: {
        type: DataTypes.INT
    }

  });

  return user;
};

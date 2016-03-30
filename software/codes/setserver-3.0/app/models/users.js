"use strict";

module.exports = function(sequelize, DataTypes) {
  var user = sequelize.define('user', {
    name: {
        type: DataTypes.STRING,
    },
    finger_id: {
        type: DataTypes.INT
    },
    access_level: {
        type: DataTypes.INT
    },
    active: {
        type: DataTypes.INT
    }

  });

  return user;
};

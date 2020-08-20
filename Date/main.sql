/*
 Navicat Premium Data Transfer

 Source Server         : hello
 Source Server Type    : SQLite
 Source Server Version : 3030001
 Source Schema         : main

 Target Server Type    : SQLite
 Target Server Version : 3030001
 File Encoding         : 65001

 Date: 23/07/2020 17:06:27
*/

PRAGMA foreign_keys = false;

-- ----------------------------
-- Table structure for User
-- ----------------------------
DROP TABLE IF EXISTS "User";
CREATE TABLE "User" (
  "Id" INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT,
  "username" varchar(255) NOT NULL,
  "pwd" varchar(255) NOT NULL,
  "power" varchar(2) NOT NULL,
  "birthday" varchar(6) NOT NULL,
  "phone" varchar(12)
);

-- ----------------------------
-- Table structure for login_log
-- ----------------------------
DROP TABLE IF EXISTS "login_log";
CREATE TABLE "login_log" (
  "id" INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT,
  "info" varchar(255) NOT NULL,
  "addrs" varchar(20) NOT NULL,
  "time" varchar(10) NOT NULL,
  "user" int(255) NOT NULL,
  FOREIGN KEY ("user") REFERENCES "User" ("Id") ON DELETE CASCADE ON UPDATE CASCADE
);

-- ----------------------------
-- Table structure for set_log
-- ----------------------------
DROP TABLE IF EXISTS "set_log";
CREATE TABLE "set_log" (
  "id" INTEGER NOT NULL,
  "set_info" varchar(255) NOT NULL,
  "addrs" varchar(25) NOT NULL,
  "time" varchar(20) NOT NULL,
  "user" int(255) NOT NULL,
  PRIMARY KEY ("id"),
  FOREIGN KEY ("user") REFERENCES "User" ("Id") ON DELETE CASCADE ON UPDATE CASCADE
);

-- ----------------------------
-- Table structure for set_user_log
-- ----------------------------
DROP TABLE IF EXISTS "set_user_log";
CREATE TABLE "set_user_log" (
  "id" INTEGER NOT NULL,
  "info" varchar(255),
  "time" varchar(20) NOT NULL,
  "user" int(20) NOT NULL,
  "addrs" varchar(20) NOT NULL,
  PRIMARY KEY ("id"),
  FOREIGN KEY ("user") REFERENCES "User" ("Id") ON DELETE CASCADE ON UPDATE CASCADE
);




PRAGMA foreign_keys = true;

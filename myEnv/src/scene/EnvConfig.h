#ifndef _ENV_CONFIG_H_
#define _ENV_CONFIG_H_

#include "../config.h"

const float gridOffset = 1.0f;
const float modelRescale = 1.0f;

const std::string pillaSphere("./myMuseum/13173_PillarSphere_v1_NEW_TRI.obj");
const std::string playerFreeThrow("./myMuseum/playerFreeThrow_TRI.obj");
const std::string playerShooting("./myMuseum/playerShooting_TRI.obj");
const std::string cuboid("./myMuseum/cuboid.obj");
const std::string basketball("./myMuseum/NBABASKETBALL_TRI.obj");


const std::pair<std::string, std::string> path_water = {
	myPath + "shaders/water.vert", myPath + "shaders/water.frag" };
const std::pair<std::string, std::string> path_terrain = {
	myPath + "shaders/terrain.vert", myPath + "shaders/terrain.frag" };
const std::pair<std::string, std::string> path_terrainC = {
	myPath + "shaders/terrainC.vert", myPath + "shaders/terrainC.frag" };
const std::pair<std::string, std::string> path_skybox = {
	myPath + "shaders/skybox.vert", myPath + "shaders/skybox.frag" };
const std::pair<std::string, std::string> path_environment = {
	myPath + "shaders/environment.vert", myPath + "shaders/environment.frag" };
const std::pair<std::string, std::string> path_toon = {
	myPath + "shaders/toon.vert", myPath + "shaders/toon.frag" };
const std::pair<std::string, std::string> path_snowman = {
	myPath + "shaders/snowman.vert", myPath + "shaders/snowman.frag" };
const std::pair<std::string, std::string> path_Silhouette = {
	myPath + "shaders/Silhouette.vert", myPath + "shaders/Silhouette.frag" };

const std::vector<std::string> hw_blue = {
	myPath + "resources/hw_blue/blue_rt.tga",
	myPath + "resources/hw_blue/blue_lf.tga",
	myPath + "resources/hw_blue/blue_up.tga",
	myPath + "resources/hw_blue/blue_dn.tga",
	myPath + "resources/hw_blue/blue_bk.tga",
	myPath + "resources/hw_blue/blue_ft.tga"
};

const std::vector<std::string> cube_museum = {
	"./myMuseum/skybox/posx.jpg",
	"./myMuseum/skybox/negx.jpg",
	"./myMuseum/skybox/posy.jpg",
	"./myMuseum/skybox/negy.jpg",
	"./myMuseum/skybox/posz.jpg",
	"./myMuseum/skybox/negz.jpg"
};



#endif
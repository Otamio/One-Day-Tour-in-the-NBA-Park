#ifndef _CONFIG_H_
#define _CONFIG_H_
#include <vector>
#include <string>

#define RELEASE
#define ENABLE_SCENE

/* Specify System Path */
const std::string myPath("./myEnv/");
const std::string myRes("./myEnv/resources/");

/* Specify ObjFile Paths */
const std::string ObjPath_Cube(myPath + "objs/cube.obj");
const std::string ObjPath_Snowman(myPath + "objs/snowman.obj");
const std::string terrain_leaf(myRes + "terrain/leaf.jpg");
const std::string terrain_grass(myRes + "terrain/grass.jpg");
const std::string dudv(myRes + "waterDUDV.png");

/* Specify Shader Paths */
const std::pair<std::string, std::string> path_phong = {
	myPath + "shaders/phongillum.vert", myPath + "shaders/phongillum.frag" };
const std::pair<std::string, std::string> path_nc = {
	myPath + "shaders/normalcolor.vert", myPath + "shaders/normalcolor.frag" };
const std::pair<std::string, std::string> path_usecolor = {
	myPath + "shaders/usecolor.vert", myPath + "shaders/usecolor.frag" };

/* Specify textures */
const std::vector<std::string> ely_peaks = {
	myPath + "resources/ely_peaks/peaks_rt.tga",
	myPath + "resources/ely_peaks/peaks_lf.tga",
	myPath + "resources/ely_peaks/peaks_up.tga",
	myPath + "resources/ely_peaks/peaks_dn.tga",
	myPath + "resources/ely_peaks/peaks_bk.tga",
	myPath + "resources/ely_peaks/peaks_ft.tga"
};

const std::vector<std::string> darkskies = {
	myPath + "resources/darkskies/darkskies_rt.tga",
	myPath + "resources/darkskies/darkskies_lf.tga",
	myPath + "resources/darkskies/darkskies_up.tga",
	myPath + "resources/darkskies/darkskies_dn.tga",
	myPath + "resources/darkskies/darkskies_bk.tga",
	myPath + "resources/darkskies/darkskies_ft.tga"
};

#endif
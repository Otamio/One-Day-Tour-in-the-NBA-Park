#include "scene.h"
#include "../shape/shapeConfig.h"
#include "../../../core/shader.h"
#include "EnvConfig.h"
#include <soil.h>


extern float terrain_size;
extern unsigned terrain_vertex_count;
extern float terrain_initial_height;

const std::vector<Color> Terrain_Colors = std::vector<Color>{
	Color(201, 178, 99, true), Color(164, 155, 98, true),
	Color(164, 155, 98, true), Color(229, 219, 164, true), 
	Color(135, 184, 82, true), Color(120, 120, 120, true),
	Color(200, 200, 210, true)
};

const float color_spread = 0.45f;

const int OCTAVES = 5;
const float AMPLITUDE = 50;
const float ROUGHNESS = 0.4f;
const float WATER_HEIGHT = -1.0f;
const float REFLECT_OFFSET = 0.1f;
const float REFRACT_OFFSET = 1.0f;
const glm::vec3 lightPos(0.3f, -1.0f, 0.5f);
const glm::vec3 lightColor(1.0f, 0.95f, 0.95f);
const glm::vec2 lightBias(0.3f, 0.8f);

int grid = 128;

EnvScene::EnvScene(int w, int h) {
	width = w;
	height = h;

	glm::vec3 eye(grid+5, 1, grid+5);
	glm::vec3 up(0, 1, 0);
	mainCamera = new Camera(eye, up, 60.0f, 
		(float) width / (float) height, 1.0f, 1000.0f, WATER_HEIGHT, -135.0f, 0.0f);
	viewCamera = new Camera(glm::vec3(grid+5, 100, grid+5), up, 60.0f,
		(float)width / (float)height, 1.0f, 1000.0f, WATER_HEIGHT, -135.0f, -45.0f);
	
	projection = mainCamera->get_projection();
	view = mainCamera->get_view();

	currentCamera = mainCamera;
}

EnvScene::~EnvScene() {
	delete mainCamera;
	delete viewCamera;
	delete skybox;
	delete water;
	delete myBasketball;
	delete collection;
	delete terrainGenerator;
	delete myterrain;
	delete reflectionFbo;
	delete refractionFbo;
	glDeleteProgram(sSkyBox);
	glDeleteProgram(sWater);
	glDeleteProgram(sTerrain);
	glDeleteProgram(sTerrainC);
	glDeleteProgram(sProgram);
	glDeleteProgram(sToon);
	glDeleteProgram(sSilhouette);
}

bool EnvScene::initialize() {

	/* Load Shaders */
	sSkyBox = LoadShaders(path_skybox.first.c_str(),
		path_skybox.second.c_str());
	sWater = LoadShaders(path_water.first.c_str(),
		path_water.second.c_str());
	sTerrain = LoadShaders(path_terrain.first.c_str(),
		path_terrain.second.c_str());
	sTerrainC = LoadShaders(path_terrainC.first.c_str(),
		path_terrainC.second.c_str());
	sBall = LoadShaders(path_environment.first.c_str(),
		path_environment.second.c_str());
	sToon = LoadShaders(path_toon.first.c_str(),
		path_toon.second.c_str());
	sSilhouette = LoadShaders(path_Silhouette.first.c_str(),
		path_Silhouette.second.c_str());

	if (!sSkyBox || !sWater || !sTerrain || !sTerrainC || !sBall || !sToon || !sSilhouette)
	{
		std::cerr << "Failed to initialize shader program" << std::endl;
		return false;
	}

	/* Load shaders for exhibits */
	sProgram = LoadShaders("./myMuseum/shaders/shader.vert",
		"./myMuseum/shaders/shader.frag");
	sProgramStand = LoadShaders("./myMuseum/shaders/shaderStand.vert",
		"./myMuseum/shaders/shaderStand.frag");

	// Check the shader program.
	if (!sProgram || !sProgramStand)
	{
		std::cerr << "Failed to initialize shader program" << std::endl;
	}

	terrainGenerator = new TerrainGenerator(
		Noise(OCTAVES, AMPLITUDE, ROUGHNESS),
		ColorGenerator(Terrain_Colors, color_spread));

	sCurrent = sProgram;

	return true;
}

bool EnvScene::createObjects() {

	skybox = new CubeMap(ObjPath_Cube, cube_museum);
	myBasketball = new D3(basketball, Material(), glm::vec3(0,0,0), 0.05f, false);
	init_basketballs();

	collection = new Exhibits();

	water = new WaterTile(grid * 4, WATER_HEIGHT);
	water->translate(glm::vec3(-grid * 2.0f * modelRescale, 0, -grid * 2.0f * modelRescale));
	loadDuDv();

	myterrain = terrainGenerator->generateTerrain(grid);

	/* Create Frame Buffers Objects */
	refractionFbo = createWaterFbo(width / 2, height / 2, true);
	reflectionFbo = createWaterFbo(width, height, false);

	return true;
}

void EnvScene::init_basketballs() {
	
	glm::vec3 start = currentCamera->get_eye();

	for (int i=0; i<balls; ++i) {
		glm::vec3 displacement = glm::vec3(rand() % (grid), (rand() % 2 + 0.5f) * -1.0f, rand() % (grid));
		ballLocs.push_back(displacement);
		ballModels.push_back(glm::translate(glm::mat4(1), displacement));
		distances.push_back(glm::distance(start, ballLocs[i]));
	}
}

void EnvScene::draw_basketballs(GLuint shader, glm::vec4 clipPlanes) {
	for (int i = 0; i < balls; ++i) {
		if (collected.count(i) > 0)
			continue;
		myBasketball->draw(shader, ballModels[i], clipPlanes);
	}
}

void EnvScene::draw() {

	glEnable(GL_CLIP_DISTANCE0);
	reflection();
	refraction();
	glDisable(GL_CLIP_DISTANCE0);

	main_draw();
}

void EnvScene::prepare() {
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glProvokingVertex(GL_FIRST_VERTEX_CONVENTION);
	cullBackFaces(true);
	enableDepthTesting(true);
	antialias(true);

	loadCameraParams(currentCamera);
}

void EnvScene::reflection() {
	reflectionFbo->bindForRender(0);
	
	/* Update view Matrix (as reflected) */
	currentCamera->reflect();
	view = mainCamera->get_view();

	/* Pass parameters */
	prepare();
	// ball->draw(sBall, glm::mat4(1));

	if (collection_flag) {
		collection->render(sCurrent, glm::vec4(0));
		draw_basketballs(sCurrent, glm::vec4(0, 1, 0, -WATER_HEIGHT));
	}

	if (terrain_flag)
		myterrain->draw(sTerrainC, glm::mat4(1), 
			glm::vec4(0, 1, 0, -WATER_HEIGHT + REFLECT_OFFSET));

	skybox->draw(sSkyBox, glm::mat4(1));

	/* Restore view Matrix (as needed) */
	currentCamera->reflect();
	view = mainCamera->get_view();

	reflectionFbo->unbindAfterRender(width, height);
}

void EnvScene::refraction() {
	refractionFbo->bindForRender(0);
	prepare();

	if (collection_flag) {
		collection->render(sCurrent, glm::vec4(0));
		draw_basketballs(sCurrent, glm::vec4(0, -1, 0, WATER_HEIGHT));
	}

	if (terrain_flag)
		myterrain->draw(sTerrainC, glm::mat4(1), 
			glm::vec4(0, -1, 0, WATER_HEIGHT + REFRACT_OFFSET));
	
	
	skybox->draw(sSkyBox, glm::mat4(1));

	refractionFbo->unbindAfterRender(width, height);
}

void EnvScene::main_draw() {
	prepare();
	
	if (collection_flag) {
		collection->render(sCurrent, glm::vec4(0));
	}

	if (terrain_flag) {
		myterrain->draw(sTerrainC, glm::mat4(1), glm::vec4(0));
	}

	if (water_flag) {
		water->draw(sWater, glm::mat4(1), 
			reflectionFbo->getColorBuffer(0),
			refractionFbo->getColorBuffer(0),
			refractionFbo->getDepthBuffer(),
			dudvMap);
	}

	if (!basketball_noshow)
		draw_basketballs(sCurrent, glm::vec4(0));

	skybox->draw(sSkyBox, glm::mat4(1));
}

void EnvScene::loadDuDv() {
	glGenTextures(1, &dudvMap);
	glBindTexture(GL_TEXTURE_2D, dudvMap);
	// Repeat Textures
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int width, height, nrChannels;
	unsigned char *data = SOIL_load_image(dudv.c_str(), &width, &height, &nrChannels, 0);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D,
			0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data
		);
		SOIL_free_image_data(data);
	}
	else {
		std::cout << "Terrain texture failed to load at path: " << dudv << std::endl;
		SOIL_free_image_data(data);
	}
}

void EnvScene::update() {
	if (water_flag)
		water->update();
	if (moveForward_flag)
		cameraMoveForward();
	if (moveBackward_flag && !moveForward_flag)
		cameraMoveBackward();
	if (moveUp_flag && !moveDown_flag)
		currentCamera->moveUp();
	else if (moveDown_flag && !moveUp_flag)
		currentCamera->moveDown();
}

void EnvScene::resize(int w, int h) {
	width = w;
	height = h;

	currentCamera->set_aspect((float)width / (float)height);
	projection = currentCamera->get_projection();

	delete refractionFbo, reflectionFbo;
	refractionFbo = createWaterFbo(width / 2, height / 2, true);
	reflectionFbo = createWaterFbo(width, height, false);
}

void EnvScene::loadCameraParams(Camera *myCamera) {
	shaderGetParameters(sSkyBox, myCamera);
	shaderGetParameters(sWater, myCamera);
	// shaderGetParameters(sTerrain, myCamera);
	shaderGetParameters(sTerrainC, myCamera);
	shaderGetParameters(sBall, myCamera);
	shaderGetParameters(sProgram, myCamera);
	shaderGetParameters(sProgramStand, myCamera);
	shaderGetParameters(sToon, myCamera);
	shaderGetParameters(sSilhouette, myCamera);
}

void EnvScene::shaderGetParameters(GLuint shader, Camera *myCamera) {
	glUseProgram(shader);
	GLuint viewPosLoc = glGetUniformLocation(shader, "viewPos");
	glUniform3fv(viewPosLoc, 1, glm::value_ptr(myCamera->get_eye()));
	GLuint projectionLoc = glGetUniformLocation(shader, "projection");
	glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(myCamera->get_projection()));
	GLuint viewLoc = glGetUniformLocation(shader, "view");
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(myCamera->get_view()));
	GLuint lightPosLoc = glGetUniformLocation(shader, "lightPos");
	glUniform3fv(lightPosLoc, 1, glm::value_ptr(lightPos));
	GLuint lightColorLoc = glGetUniformLocation(shader, "lightColor");
	glUniform3fv(lightColorLoc, 1, glm::value_ptr(lightColor));
	GLuint lightBiasLoc = glGetUniformLocation(shader, "lightBias");
	glUniform3fv(lightBiasLoc, 1, glm::value_ptr(lightBias));
	GLuint useToonLoc = glGetUniformLocation(shader, "useToon");
	glUniform1f(useToonLoc, useToon);
	GLuint waterParamLoc = glGetUniformLocation(shader, "waterParam");
	glUniform1f(waterParamLoc, waterParam);
}

void EnvScene::cameraScroll(double yoffset) {
	currentCamera->set_fovy((float)yoffset);
	projection = currentCamera->get_projection();
}

void EnvScene::cameraRotate(double xChange, double yChange) {
	currentCamera->rotate((float)xChange, (float)yChange);
	view = currentCamera->get_view();
}

void EnvScene::reshape_terrain() {
	delete myterrain;
	myterrain = terrainGenerator->reseed(grid);
}

Fbo *EnvScene::createWaterFbo(int width, int height, bool useTextureForDepth) {
	Attachment *colorAttach = new TextureAttachment(GL_RGBA8);
	Attachment *depthAttach;
	if (useTextureForDepth) {
		depthAttach = new TextureAttachment(GL_DEPTH_COMPONENT24);
	}
	else {
		depthAttach = new RenderBufferAttachment(GL_DEPTH_COMPONENT24);
	}
	/* Build Fbos */
	FboBuilder *builder = new FboBuilder(width, height);
	builder->addColorAttachment(0, colorAttach);
	builder->addDepthAttachment(depthAttach);
	Fbo *myFbo = builder->init();
	// delete builder;
	return myFbo;
}

void EnvScene::cullBackFaces(bool cull) {
		if (cull && !cullingBackFace) {
			glEnable(GL_CULL_FACE);
			glCullFace(GL_BACK);
			cullingBackFace = true;
		}
		else if (!cull && cullingBackFace) {
			glDisable(GL_CULL_FACE);
			cullingBackFace = false;
		}
}

void EnvScene::antialias(bool enable) {
	if (enable && !antialiasing) {
		glEnable(GL_MULTISAMPLE);
		antialiasing = true;
	}
	else if (!enable && antialiasing) {
		glDisable(GL_MULTISAMPLE);
		antialiasing = false;
	}
}

void EnvScene::enableDepthTesting(bool enable) {
	if (enable && !depthTesting) {
		glEnable(GL_DEPTH_TEST);
		depthTesting = true;
	}
	else if (!enable && depthTesting) {
		glDisable(GL_DEPTH_TEST);
		depthTesting = false;
	}
}

void EnvScene::cameraMoveForward() {
	glm::vec3 displacement = currentCamera->moveForward();
	view = currentCamera->get_view();
	
	glm::vec3 currPosition = currentCamera->get_eye();

	for (int i=0; i<balls; ++i) {
		if (collected.count(i) > 0)
			continue;
		distances[i] = glm::distance(currPosition, ballLocs[i]);
		GLfloat diffx = currPosition.x - ballLocs[i].x;
		GLfloat diffz = currPosition.z - ballLocs[i].z;
		if (diffx*diffx + diffz*diffz < 2.0f)
			collected.insert(i);
	}
}

void EnvScene::cameraMoveBackward() {
	glm::vec3 displacement = currentCamera->moveBackward();
	view = currentCamera->get_view();

	glm::vec3 currPosition = currentCamera->get_eye();

	for (int i = 0; i < balls; ++i) {
		if (collected.count(i) > 0)
			continue;
		distances[i] = glm::distance(currPosition, ballLocs[i]);
		GLfloat diffx = currPosition.x - ballLocs[i].x;
		GLfloat diffz = currPosition.z - ballLocs[i].z;
		if (diffx * diffx + diffz * diffz < 2.0f)
			collected.insert(i);
	}
}

void EnvScene::select_shader() {
	if (!toon_flag && !silhouette_flag) {
		sCurrent = sProgram;
		useToon = 0;
	}
	else if (toon_flag) {
		sCurrent = sToon;
		useToon = 1;
	} 
	else {
		sCurrent = sSilhouette;
		useToon = 2;
	}
}

void EnvScene::cameraDrop() {
	if (lock)
		lock = currentCamera->moveDown();
	view = currentCamera->get_view();
}

void EnvScene::switchCamera() {
	moveBackward_flag = false;
	moveForward_flag = false;

	if (currentCamera == mainCamera)
		currentCamera = viewCamera;
	else
		currentCamera = mainCamera;

	projection = currentCamera->get_projection();
	view = currentCamera->get_view();
}
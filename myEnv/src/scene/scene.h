#ifndef _SCENE_H_
#define _SCENE_H_

#include "../skybox/CubeMap.h"
#include "../water/waterSurface.h"
#include "../water/WaterTile.h"
#include "../terrian/Terrain.h"
#include "../terrian/TerrainGenerator.h"
#include "../exhibits/Exhibits.h"
#include "../Camera.h"
#include "../fbo/Attachment.h"
#include "../fbo/RenderBufferAttachment.h"
#include "../fbo/TextureAttachment.h"
#include <map>
#include <set>

class EnvScene {
	CubeMap *skybox;
	D3 *myBasketball;
	WaterTile *water;
	Terrain *terrain;
	TerrainGenerator *terrainGenerator;
	myTerrain *myterrain;
	Camera *mainCamera, *viewCamera, *currentCamera;
	Exhibits *collection;
	Fbo *refractionFbo, *reflectionFbo;
	int width, height, balls=20;
	bool cullingBackFace = false;
	bool depthTesting = false;
	bool antialiasing = false;
	bool water_flag = true;
	bool terrain_flag = true;
	bool collection_flag = true;
	bool basketball_noshow = false;
	bool moveForward_flag = false;
	bool moveBackward_flag = false;
	bool moveUp_flag = false;
	bool moveDown_flag = false;
	bool toon_flag = false;
	bool silhouette_flag = false;
	bool lock = true;
	GLfloat useToon = 0;
	GLfloat waterParam = 0;
	std::vector<glm::mat4> ballModels;
	std::vector<glm::vec3> ballLocs;
	std::set<int> collected;
	std::vector<GLfloat> distances;
	void loadCameraParams(Camera *);
	void shaderGetParameters(GLuint, Camera*);
	static Fbo *createWaterFbo(int, int, bool);
	void prepare();
	void cullBackFaces(bool);
	void antialias(bool);
	void enableDepthTesting(bool);
	void loadDuDv();
	void reflection();
	void refraction();
	void main_draw();
	void cameraMoveForward();
	void cameraMoveBackward();
	void cameraDrop();
	void init_basketballs();
	void draw_basketballs(GLuint, glm::vec4);
	void select_shader();
protected:
	GLuint sSkyBox, sWater, sTerrain, sTerrainC, sBall, sToon,
		sSnowman, sSilhouette;
	GLuint sCurrent;
	GLuint sProgram, sProgramStand;
	GLuint projectionLoc, viewLoc, viewPosLoc;
	GLuint dudvMap;
	glm::mat4 projection, view;
public:
	EnvScene(int, int);
	~EnvScene();
	bool initialize();
	bool createObjects();
	/* Change Camera Parameters */
	void resize(int, int);
	void cameraScroll(double);
	void cameraRotate(double, double);
	void cameraSpeedUp() { currentCamera->cameraSpeedUp(); };
	void cameraSpeedDown() { currentCamera->cameraSpeedDown(); };
	void switchCamera();
	/* */
	void draw();
	void update();
	void reshape_terrain();
	/* Toggle control parameters */
	void toggle_water() { water_flag = !water_flag; }
	void toggle_terrain() { terrain_flag = !terrain_flag; }
	void toggle_forward() { moveForward_flag = !moveForward_flag; }
	void toggle_backward() { moveBackward_flag = !moveBackward_flag; }
	void toggle_up() { moveUp_flag = !moveUp_flag; }
	void toggle_down() { moveDown_flag = !moveDown_flag; }
	void toggle_collections() { collection_flag = !collection_flag; }
	void toggle_toon() { toon_flag = !toon_flag; select_shader(); }
	void toggle_silhouette() { silhouette_flag = !silhouette_flag; select_shader(); }
	void toggle_reflect() { waterParam = 1.0f; }
	void toggle_refract() { waterParam = 2.0f; }
	void toggle_restore() { waterParam = 0.0f; }
};


#endif
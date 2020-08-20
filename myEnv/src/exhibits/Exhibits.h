#ifndef _EXHIBITS_H_
#define _EXHIBITS_H_

#include "../../../myMuseum/Node.h"
#include "../../../myMuseum/Transform.h"
#include "../../../myMuseum/Geometry.h"

class Exhibits {

	GLuint modelLoc, colorLoc, 
		materialAmbientLoc, materialDiffuseLoc, materialSpecularLoc, 
		materialShininessLoc, normalColoringLoc;

	bool normal_coloring = false;

	std::vector<Transform *> party2robot;
	Transform *party1Trans;
	Transform *party2Trans;

	Geometry *trophyGeo;
	Transform *trophyTrans;
	Geometry *standGeo;
	Transform *standTrans;
	Geometry *p1Geo;
	Transform *p1Trans;
	Geometry *p12Geo;
	Transform *p12Trans;
	Geometry *p13Geo;
	Transform *p13Trans;
	Geometry *p14Geo;
	Transform *p14Trans;

	Geometry *p2Geo;
	Transform *p2Trans;
	Geometry *p22Geo;
	Transform *p22Trans;
	Geometry *p23Geo;
	Transform *p23Trans;
	Geometry *p24Geo;
	Transform *p24Trans;

	Node *currentObj;
	Node *pObj;
	Node *standObj;

	glm::vec3 xaxis = glm::vec3(1.0, 0.0, 0.0);
	glm::vec3 yaxis = glm::vec3(0.0, 1.0, 0.0);
	glm::vec3 zaxis = glm::vec3(0.0, 0.0, 1.0);

	void CreateExhibits() {
		//party of the exhibits
		glm::mat4 partyPose = glm::translate(glm::mat4(1.0), glm::vec3(0.0, 0.0, 0.0));
		party1Trans = new Transform(partyPose);

		//Trophy
		float trophyScale = 0.1f;
		const float rot90 = glm::radians(90.0f);
		glm::vec3 tmp_trophyscale(trophyScale);
		glm::mat4 trophyScale_mat = glm::scale(glm::mat4(1), tmp_trophyscale);
		glm::mat4 trophyTranslate = glm::translate(trophyScale_mat, glm::vec3(-340.0f, -10.0f, -10.0f));
		glm::mat4 trophyPose = glm::rotate(glm::rotate(trophyTranslate, -rot90, xaxis), rot90, zaxis);
		trophyTrans = new Transform(trophyPose);
		trophyGeo = new Geometry("./myMuseum/13173_PillarSphere_v1_NEW_TRI.obj", false);
		trophyTrans->addChild(trophyGeo);
		party1Trans->addChild(trophyTrans);

		// Players
		party2Trans = new Transform(glm::mat4(1.0f));

		//Player right 1
		glm::mat4 p1Pose = createPlayer(-20.0f, true);
		p1Trans = new Transform(p1Pose);
		p1Geo = new Geometry("./myMuseum/playerFreeThrow_TRI.obj", false);
		p1Trans->addChild(p1Geo);
		party2Trans->addChild(p1Trans);

		//Player right 2
		glm::mat4 p12Pose = createPlayer(0.0f, true);
		p12Trans = new Transform(p12Pose);
		p12Trans->addChild(p1Geo);
		party2Trans->addChild(p12Trans);

		//Player right 3
		glm::mat4 p13Pose = createPlayer(20.0f, true);
		p13Trans = new Transform(p13Pose);
		p13Trans->addChild(p1Geo);
		party2Trans->addChild(p13Trans);

		//Player right 4
		glm::mat4 p14Pose = createPlayer(40.0f, true);
		p14Trans = new Transform(p14Pose);
		p14Trans->addChild(p1Geo);
		party2Trans->addChild(p14Trans);

		//Player left 1
		glm::mat4 p2Pose = createPlayer(-20.0f, false);
		p2Trans = new Transform(p2Pose);
		p2Geo = new Geometry("./myMuseum/playerShooting_TRI.obj", false);
		p2Trans->addChild(p2Geo);
		party2Trans->addChild(p2Trans);

		//Player left 2
		glm::mat4 p22Pose = createPlayer(0.0f, false);
		p22Trans = new Transform(p22Pose);
		p22Trans->addChild(p2Geo);
		party2Trans->addChild(p22Trans);

		//Player left 3
		glm::mat4 p23Pose = createPlayer(20.0f, false);
		p23Trans = new Transform(p23Pose);
		p23Trans->addChild(p2Geo);
		party2Trans->addChild(p23Trans);

		//Player left 4
		glm::mat4 p24Pose = createPlayer(40.0f, false);
		p24Trans = new Transform(p24Pose);
		p24Trans->addChild(p2Geo);
		party2Trans->addChild(p24Trans);
	}
	glm::mat4 createPlayer(float moveFront, bool isRight) {
		glm::mat4 result;
		const float rot90 = glm::radians(90.0f);
		if (isRight) {
			float p1Scale = 1.0f;
			glm::vec3 tmp_p1scale(p1Scale);
			glm::mat4 p1Scale_mat = glm::scale(glm::mat4(1), tmp_p1scale);
			glm::mat4 p1Translate = glm::translate(p1Scale_mat, glm::vec3(-15.0f, -1.0f, 30.0f + moveFront));
			glm::mat4 p1Pose = glm::rotate(p1Translate, glm::radians(-90.0f), xaxis);
			p1Pose = glm::rotate(p1Pose, glm::radians(-90.0f), zaxis);
			result = p1Pose;
		}
		else {
			float p2Scale = 1.0f;
			glm::vec3 tmp_p2scale(p2Scale);
			glm::mat4 p2Scale_mat = glm::scale(glm::mat4(1), tmp_p2scale);
			glm::mat4 p2Translate = glm::translate(p2Scale_mat, glm::vec3(-45.0f, -1.0f, 25.0f + moveFront));
			glm::mat4 p2Pose = glm::rotate(p2Translate, glm::radians(-90.0f), xaxis);
			p2Pose = glm::rotate(p2Pose, rot90, zaxis);
			result = p2Pose;
		}
		return result;
	}
	void shaderGetParams_curr(GLuint program) {
		materialAmbientLoc = glGetUniformLocation(program, "material_ambient");
		materialDiffuseLoc = glGetUniformLocation(program, "material_diffuse");
		materialSpecularLoc = glGetUniformLocation(program, "material_specular");
		materialShininessLoc = glGetUniformLocation(program, "material_shininess");
		normalColoringLoc = glGetUniformLocation(program, "normal_coloring");

		((Geometry *)currentObj)->getMaterialGold();

		glUniform3fv(materialAmbientLoc, 1, glm::value_ptr(((Geometry *)currentObj)->material_ambient));
		glUniform3fv(materialDiffuseLoc, 1, glm::value_ptr(((Geometry *)currentObj)->material_diffuse));
		glUniform3fv(materialSpecularLoc, 1, glm::value_ptr(((Geometry *)currentObj)->material_specular));
		glUniform1f(materialShininessLoc, ((Geometry *)currentObj)->material_shininess);
		glUniform1f(normalColoringLoc, normal_coloring);
	}
	void shaderGetParams_p(GLuint program) {
		materialAmbientLoc = glGetUniformLocation(program, "material_ambient");
		materialDiffuseLoc = glGetUniformLocation(program, "material_diffuse");
		materialSpecularLoc = glGetUniformLocation(program, "material_specular");
		materialShininessLoc = glGetUniformLocation(program, "material_shininess");
		normalColoringLoc = glGetUniformLocation(program, "normal_coloring");

		((Geometry *)pObj)->getMaterialBronze();
		glUniform3fv(materialAmbientLoc, 1, glm::value_ptr(((Geometry *)pObj)->material_ambient));
		glUniform3fv(materialDiffuseLoc, 1, glm::value_ptr(((Geometry *)pObj)->material_diffuse));
		glUniform3fv(materialSpecularLoc, 1, glm::value_ptr(((Geometry *)pObj)->material_specular));
		glUniform1f(materialShininessLoc, ((Geometry *)pObj)->material_shininess);
		glUniform1f(normalColoringLoc, normal_coloring);
	}
public:
	Exhibits() {
		CreateExhibits();
		currentObj = party1Trans;
		standObj = standTrans;
		pObj = party2Trans;
	}
	~Exhibits() {
		delete trophyGeo;
		delete trophyTrans;

		delete party1Trans;
		delete party2Trans;
	}
	void render(GLuint program, glm::vec4 clipPlane) {

		// Activate the shader program.
		glUseProgram(program);
		GLuint clipPlaneLoc = glGetUniformLocation(program, "plane");
		glUniform4fv(clipPlaneLoc, 1, glm::value_ptr(clipPlane));
		shaderGetParams_curr(program);

		glm::mat4 model = party1Trans->tf;

		// Render the object.
		party1Trans->draw(program, glm::mat4(1));

		//Draw player
		model = party2Trans->tf;
		shaderGetParams_p(program);

		// Render the object.
		party2Trans->draw(program, model);
	}

};

#endif

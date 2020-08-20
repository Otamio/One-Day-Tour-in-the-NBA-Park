#ifdef GOGOGO
#ifndef PINGPONG
void Terrain::square(int left, int right, int top, int bot, int step) {
	if (left >= right - 1)  return;

	int m1 = (left + right) / 2;
	int m2 = (top + bot) / 2;

	if (left == 0)
		height_map[left][m2] = (height_map[left][top] + height_map[left][bot]
			+ height_map[m1][m2]) / 3.0f + randomz(step);
	else
		height_map[left][m2] = (height_map[left][top] + height_map[left][bot]
			+ height_map[m1][m2] + height_map[left - 2][m2]) / 4.0f + randomz(step);

	if (top == 0)
		height_map[m1][top] = (height_map[left][top] + height_map[right][top]
			+ height_map[m1][m2]) / 3.0f + randomz(step);
	else
		height_map[m1][top] = (height_map[left][top] + height_map[right][top]
			+ height_map[m1][m2] + height_map[m1][top - 2]) / 4.0f + randomz(step);

	if (right == VexCount - 1)
		height_map[right][m2] = (height_map[right][top] + height_map[right][bot]
			+ height_map[m1][m2]) / 3.0f + randomz(step);
	else
		height_map[right][m2] = (height_map[right][top] + height_map[right][bot]
			+ height_map[m1][m2] + height_map[right + 2][m2]) / 4.0f + randomz(step);

	if (bot == VexCount - 1)
		height_map[m1][bot] = (height_map[left][bot] + height_map[right][bot]
			+ height_map[m1][m2]) / 3.0f + randomz(step);
	else
		height_map[m1][bot] = (height_map[left][bot] + height_map[right][bot]
			+ height_map[m1][m2] + height_map[m1][bot + 2]) / 4.0f + randomz(step);
}
#endif

void Terrain::DS_Noise(int left, int right, int top, int bot, int step) {
	if (VexCount < 2) return;
	if (left >= right - 1)  return;

	srand((unsigned long)time(NULL));

	++step;
	square(left, right, top, bot, step);

	int m1 = (left + right) / 2;
	int m2 = (top + bot) / 2;

	++step;
	diamond(left, m1, top, m2, step);
	diamond(left, m1, m2, bot, step);
	diamond(m1, right, top, m2, step);
	diamond(m1, right, m2, bot, step);

	++step;
	DS_Noise(left, m1, top, m2, step);
	DS_Noise(left, m1, m2, bot, step);
	DS_Noise(m1, right, top, m2, step);
	DS_Noise(m1, right, m2, bot, step);
}
float Terrain::randomz(int step)
{
	float adj = 3.0f / step / 2;
	return  ((float)rand() / RAND_MAX + adj) * adj * 2.0f;
}

void Terrain::diamond(int left, int right, int top, int bot, int step) {
	if (left >= right - 1)  return;

	int m1 = (left + right) / 2;
	int m2 = (top + bot) / 2;

	height_map[m1][m2] = (height_map[left][top] + height_map[left][bot]
		+ height_map[right][top] + height_map[right][bot]) / 4.0f + randomz(step);
}

DiamondSquare();

// vertices.show_element(10);
//	std::cout << indices.size() << std::endl;


void Terrain::DiamondSquare() {

	// Set boundary values
	// height_map[0][0] = -1.0f;
	// height_map[0][VexCount-1] = -1.0f;
	// height_map[VexCount-1][0] = -1.0f;
	// height_map[VexCount-1][VexCount-1] = -1.0f;

	diamond(0, VexCount - 1, 0, VexCount - 1, 1);
	DS_Noise(0, VexCount - 1, 0, VexCount - 1, 2);

	// Update the heights
	for (unsigned i = 0; i != VexCount; ++i) {
		for (unsigned j = 0; j != VexCount; ++j) {
			vertices[i * VexCount + j].set_posY(height_map[i][j]);
		}
	}
}
waterSurface::waterSurface(unsigned ct, GLfloat sz, GLfloat y = 0.0f) {
	VexCount = ct;
	size = sz;
	y = yval;
}

#endif

#ifdef FFF
if (!reflected)
return glm::lookAt(eye, eye + front, up);
return glm::lookAt(eye_under_water,
	eye_under_water + front * glm::vec3(1.0f, -1.0f, 1.0f), -1.0f * up);

#endif

#ifdef CCC
reflect_Matrix = update_vMatrix(eye_under_water, -1.0f * Pitch, Yaw);

float Mflip[16] = { 1, 0, 0, 0,
					0, -1, 0, 0,
					0, 0, 1, 0,
					0, 0, 0, 1 };
float Mreflect[16] = { 1, 0, 0, 0,
					   0, -1, 0, 0,
					   0, 0, 1, 0,
					   0, 2 * waterHeight, 0, 1 };
flip = glm::make_mat4(Mflip);
reflection = glm::make_mat4(Mreflect);

view_Matrix = update_vMatrix(eye, Pitch, Yaw);

glm::mat4 update_vMatrix(glm::vec3 Pos, float pitch, float yaw) {
	glm::mat4 ret = glm::mat4(1);
	ret = glm::rotate(ret, glm::radians(pitch), glm::vec3(1.0f, 0.0f, 0.0f));
	ret = glm::rotate(ret, glm::radians(yaw), glm::vec3(0.0f, -1.0f, 0.0f));
	ret = glm::translate(ret, -1.0f * Pos);
	return ret;
}

#endif
#ifdef GGG










#endif
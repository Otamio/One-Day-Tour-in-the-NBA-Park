#include "HeightGenerator.h"
#include <ctime>
#include <iostream>
#include <string>
#include <cmath>

#define PI 3.14159265

HeightGenerator::HeightGenerator(unsigned nVex) {
	
	VexCount = nVex;

	seed = (unsigned)  time(NULL) % 16384;
	height_map = std::vector<std::vector<GLfloat>>(VexCount,
		std::vector<GLfloat>(VexCount, 0.0f));

	DiamondSquare();
	//std::cout << std::to_string(height_map[3][3]) << std::endl;
}

GLfloat HeightGenerator::_rand() {
	float num = ((GLfloat)rand() / RAND_MAX * 2.0f - 1.0f);
	return num;
}

GLfloat HeightGenerator::getNoise(unsigned x, unsigned z) {
	srand(x * xMult + z * zMult + seed);
	float num = _rand();
	return num;
}

GLfloat HeightGenerator::getSmoothNoise(unsigned x, unsigned z) {
	float corners = (getNoise(x - 1, z - 1) + getNoise(x + 1, z - 1) + getNoise(x - 1, z + 1)
		+ getNoise(x + 1, z + 1)) / 16.0f;
	float sides = (getNoise(x - 1, z) + getNoise(x + 1, z) + getNoise(x, z - 1)
		+ getNoise(x, z + 1)) / 8.0f;
	float center = getNoise(x, z) / 4.0f;
	
	float num = corners + sides + center;
	return num;
}

GLfloat HeightGenerator::getInterpolatedNoise(float x, float z) {
	unsigned intX = (unsigned)x;
	unsigned intZ = (unsigned)z;
	float fracX = x - intX;
	float fracZ = z - intZ;

	float v1 = getSmoothNoise(intX, intZ);
	float v2 = getSmoothNoise(intX + 1, intZ);
	float v3 = getSmoothNoise(intX, intZ + 1);
	float v4 = getSmoothNoise(intX + 1, intZ + 1);
	float i1 = interpolate(v1, v2, fracX);
	float i2 = interpolate(v3, v4, fracX);
	return interpolate(i1, i2, fracZ);
}

GLfloat HeightGenerator::generateHeight(unsigned x, unsigned z) {
	float total = 0;
	float d = (float)pow(2, OCTAVES - 1);
	for (int i = 0; i < OCTAVES; i++) {
		float freq = (float)(pow(2, i) / d);
		float amp = (float)pow(ROUGHNESS, i) * AMPLITUDE;
		total += getInterpolatedNoise((x + 5) * freq, (z + 5) * freq) * amp;
	}
	// std::cout << total << std::endl;
	return total;
}

void HeightGenerator::DiamondSquare() {
	Diamond(0, VexCount-1, 0, VexCount-1);
	DS_Recurse(0, VexCount-1, 0, VexCount-1);
}

void HeightGenerator::Diamond(int left, int right, int top, int bot) {
	if (left >= right - 1)  return;

	int m1 = (left + right) / 2;
	int m2 = (top + bot) / 2;

	height_map[m1][m2] = (height_map[left][top] + height_map[left][bot]
		+ height_map[right][top] + height_map[right][bot]) / 4.0f + generateHeight(m1,m2);
}

void HeightGenerator::Square(int left, int right, int top, int bot) {
	if (left >= right - 1)  return;

	int m1 = (left + right) / 2;
	int m2 = (top + bot) / 2;

	if (left == 0)
		height_map[left][m2] = (height_map[left][top] + height_map[left][bot]
			+ height_map[m1][m2]) / 3.0f + generateHeight(left,m2);
	else
		height_map[left][m2] = (height_map[left][top] + height_map[left][bot]
			+ height_map[m1][m2] + height_map[left - 2][m2]) / 4.0f + generateHeight(left,m2);

	if (top == 0)
		height_map[m1][top] = (height_map[left][top] + height_map[right][top]
			+ height_map[m1][m2]) / 3.0f + generateHeight(m1,top);
	else
		height_map[m1][top] = (height_map[left][top] + height_map[right][top]
			+ height_map[m1][m2] + height_map[m1][top - 2]) / 4.0f + generateHeight(m1,top);

	if (right == VexCount - 1)
		height_map[right][m2] = (height_map[right][top] + height_map[right][bot]
			+ height_map[m1][m2]) / 3.0f + generateHeight(right,m2);
	else
		height_map[right][m2] = (height_map[right][top] + height_map[right][bot]
			+ height_map[m1][m2] + height_map[right + 2][m2]) / 4.0f + generateHeight(right,m2);

	if (bot == VexCount - 1)
		height_map[m1][bot] = (height_map[left][bot] + height_map[right][bot]
			+ height_map[m1][m2]) / 3.0f + generateHeight(m1,bot);
	else
		height_map[m1][bot] = (height_map[left][bot] + height_map[right][bot]
			+ height_map[m1][m2] + height_map[m1][bot + 2]) / 4.0f + generateHeight(m1,bot);
}

void HeightGenerator::DS_Recurse(int left, int right, int top, int bot) {
	if (left >= right - 1)  return;

	int m1 = (left + right) / 2;
	int m2 = (top + bot) / 2;

	Square(left, right, top, bot);
	Diamond(left, m1, top, m2);
	Diamond(left, m1, m2, bot);
	Diamond(m1, right, top, m2);
	Diamond(m1, right, m2, bot);

	DS_Recurse(left, m1, top, m2);
	DS_Recurse(left, m1, m2, bot);
	DS_Recurse(m1, right, top, m2);
	DS_Recurse(m1, right, m2, bot);
}

GLfloat HeightGenerator::getHeight(unsigned x, unsigned z) {
	// std::cout << height_map[x][z] << std::endl;
	// return height_map[x][z];
	return generateHeight(x, z);
}

GLfloat HeightGenerator::interpolate(float a, float b, float blend) {
	double theta = blend * PI;
	GLfloat f = (GLfloat)(1.0f - cos(theta)) * 0.5f;
	return a * (1.0f - f) + b * f;
}

void HeightGenerator::reseed() {
	xMult = rand() % 262144 + 8192;
	zMult = rand() % 262144 + 8192;
}
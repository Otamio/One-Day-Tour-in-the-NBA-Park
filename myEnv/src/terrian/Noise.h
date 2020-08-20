#ifndef _NOISE_H_
#define _NOISE_H_

#include <cmath>
#include <ctime>

#define PI 3.14159265

class Noise {
	int seed = 0;
	float roughness = 0.0f;
	int octaves = 0;
	float amplitude = 0.0f;
	int xMult = 49632, zMult = 325176;
	float myRand() { return (float) rand() / RAND_MAX * 2.0f - 1.0f; }
	float interpolate(float a, float b, float blend) {
		double theta = blend * PI;
		float f = (float)((1.0f - cos(theta)) * 0.5f);
		return a * (1 - f) + b * f;
	}
	float getNoise(int x, int z) {
		srand(x * xMult + z * zMult + seed);
		return myRand();
	}
	float getSmoothNoise(int x, int y) {
		float corners = (getNoise(x - 1, y - 1) + getNoise(x + 1, y - 1) + getNoise(x - 1, y + 1)
			+ getNoise(x + 1, y + 1)) / 16.0f;
		float sides = (getNoise(x - 1, y) + getNoise(x + 1, y) + getNoise(x, y - 1) + getNoise(x, y + 1)) / 8.0f;
		float center = getNoise(x, y) / 4.0f;
		return corners + sides + center;
	}
	float getInterpolatedNoise(float x, float y) {
		int intX = (int)x;
		float fracX = x - intX;
		int intY = (int)y;
		float fracY = y - intY;

		float v1 = getSmoothNoise(intX, intY);
		float v2 = getSmoothNoise(intX + 1, intY);
		float v3 = getSmoothNoise(intX, intY + 1);
		float v4 = getSmoothNoise(intX + 1, intY + 1);
		float i1 = interpolate(v1, v2, fracX);
		float i2 = interpolate(v3, v4, fracX);
		return interpolate(i1, i2, fracY);
	}
public:
	Noise() {}
	Noise(int seed, int octaves, float amplitude, float roughness) {
		this->seed = seed;
		this->octaves = octaves;
		this->amplitude = amplitude;
		this->roughness = roughness;
	}
	Noise(const int octaves, const float amplitude, const float roughness) {
		srand((long) time(NULL));
		this->seed = rand();
		this->octaves = octaves;
		this->amplitude = amplitude;
		this->roughness = roughness;
	}
	int getSeed() { return seed; }
	float getAmplitude() { return amplitude; }
	float getNoiseF(int x, int y) {
		float total = 0;
		float d = (float) pow(2, octaves - 1);
		for (int i = 0; i < octaves; i++) {
			float freq = (float) (pow(2, i) / d);
			float amp = (float) pow(roughness, i) * amplitude;
			total += getInterpolatedNoise(x * freq, y * freq) * amp;
		}
		return total;
	}
	void reseed() {
		//xMult = rand() % 65536 + 8192;
		//zMult = rand() % 65536 + 8192;
		seed = rand() % 65536;
	}
};

#endif
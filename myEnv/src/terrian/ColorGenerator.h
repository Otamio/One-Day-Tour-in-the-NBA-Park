#ifndef _COLOR_GENERATOR_H_
#define _COLOR_GENERATOR_H_

#include "../utilities/color.h"
#include <vector>
#include <cmath> 

class ColorGenerator {
	float spread = 0.0f;
	float halfSpread = 0.0f;
	float part = 0.0f;
	std::vector<Color> IColors;
	float clamp(const float v, const float lo, const float hi) {
		if (v < lo)
			return lo;
		else if (v > hi)
			return hi;
		else
			return v;
	}
	Color calculateColor(float height, float amplitude) {
		float value = (height + amplitude) / (amplitude * 2);
		value = clamp((value - halfSpread) * (1.0f / spread), 0.0f, 0.9999f);
		int firstBiome = (int) floor(value / part);
		float blend = (value - (firstBiome * part)) / part;
		return Color::interpolateColors(IColors[firstBiome], IColors[firstBiome + 1], blend);
	}
public:
	ColorGenerator() {}
	ColorGenerator(std::vector<Color> myColors, float s) {
		IColors = myColors;
		spread = s;
		halfSpread = s / 2.0f;
		part = 1.0f / (myColors.size() - 1);
	}
	std::vector<std::vector<Color>> generateColours(
		std::vector<std::vector<GLfloat>> heights, float amplitude) {
		std::vector<std::vector<Color>> colors = std::vector<std::vector<Color>>(heights.size(),
			std::vector<Color>(heights.size(), Color()));
		for (unsigned z = 0; z < heights.size(); ++z) {
			for (unsigned x = 0; x < heights[z].size(); ++x) {
				colors[z][x] = calculateColor(heights[z][x], amplitude);
			}
		}
		return colors;
	}
};

#endif
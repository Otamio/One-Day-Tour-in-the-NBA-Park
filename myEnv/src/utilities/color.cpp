#include "color.h"

Color Color::interpolateColors(Color c1, Color c2, float blend) {
	float colour1Weight = 1 - blend;
	float r = (colour1Weight * c1.col.x) + (blend * c2.col.x);
	float g = (colour1Weight * c1.col.y) + (blend * c2.col.y);
	float b = (colour1Weight * c1.col.z) + (blend * c2.col.z);
	return Color(r, g, b);
}
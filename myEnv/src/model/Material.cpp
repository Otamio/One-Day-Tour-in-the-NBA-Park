#include "Material.h"

/* Material 1: very shiny (i.e., high specular component), with no diffuse reflection
 *  Looks like a black plastic
 */
Material material1(glm::vec3(0.4f, 0.4f, 0.4f), // Ambient
                   glm::vec3(0.0f, 0.0f, 0.0f), // Diffuse
                   glm::vec3(0.99f, 0.99f, 0.99f), // Specular
                   5.00f); // Shininess

/* Material 2: Another model should only use diffuse reflection, with zero shininess.
 *  Dolly
 */
Material material2(glm::vec3(0.0f, 0.0f, 0.0f), // Ambient
                   glm::vec3(0.8f, 0.8f, 0.8f), // Diffuse
                   glm::vec3(0.0f, 0.0f, 0.0f), // Specular
                   0.0f); // Shininess
/* Material3: The third object should have significant diffuse and specular reflection components.
 *  Emerald
 */
Material material3(glm::vec3(0.0215f, 0.1745f, 0.0215f), // Ambient
                   glm::vec3(0.07568f, 0.61424f, 0.07568f), // Diffuse
                   glm::vec3(0.633f, 0.727811f, 0.633f), // Specular
                   0.6f); // Shininess

/* Material (Sphere): Ambient reflection to be determined, with no diffuse or specular reflection.
 */
Material material4(
	glm::vec3(1.0f), // Ambient
	glm::vec3(0.0f), // Diffuse
	glm::vec3(0.0f), // Specular
	0.01f);			 //	Shininess
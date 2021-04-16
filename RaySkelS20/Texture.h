#include "../RaySkelS20/src/vecmath/vecmath.h"

/**
 * Binds to material to provide texture maps like: diffuse, specular, bump, normal, etc.
 */
class Texture
{
public:
	enum UV {
		Box,
		Cone,
		Cylinder,
		Sphere,
		Square
	};

	Texture( char* imageFileName, UV uv);

	unsigned char* imagePtr = nullptr;
	int imageWidth = 0;
	int imageHeight = 0;
	UV uv = Square;

	vec3f getColorByUV(double u, double v);

private:
	vec3f getBoxColor(double u, double v);
	vec3f getSquareColor(double u, double v) const;
};
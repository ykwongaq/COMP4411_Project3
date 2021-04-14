#pragma once

#include "../scene/scene.h"

// Bonus 12 : New Geometry
// Reference: http://cosinekitty.com/raytrace/chapter13_torus.html
class Torus : public MaterialSceneObject {
public:
	Torus(Scene *scene, Material *mat, const float &A, const float &B);
	virtual bool intersectLocal(const ray &r, isect &iSect) const override;
	virtual bool hasBoundingBoxCapability() const override;
	virtual BoundingBox ComputeLocalBoundingBox() override;
	
private:
	float A;
	float B;
};
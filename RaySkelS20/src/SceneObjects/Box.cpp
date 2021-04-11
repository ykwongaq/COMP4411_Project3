#include <cmath>
#include <assert.h>
#include <limits>

#include "Box.h"

// Intersection detection for box
// Reference: https://education.siggraph.org/static/HyperGraph/raytrace/rtinter3.htm
bool Box::intersectLocal( const ray& r, isect& i ) const
{
	// YOUR CODE HERE:
    // Add box intersection code here.
	// it currently ignores all boxes and just returns false.
	
	// Make use of build-in function provide by bounding box.
	double Tnear = -1, Tfar = -1;
	vec3f normal;
	bool isIntersect = this->getLocalBoundingBox().intersect(r, Tnear, Tfar, normal);

	// Set up the intersection only of the ray intersect with the box
	if (isIntersect) {
		i.setT(Tnear);
		i.setN(normal);
		i.obj = this;
	}

	return isIntersect;

}

// Just a copy of ComputeLocalBoundingBox.
// Need because this version can be used in const function
BoundingBox Box::getLocalBoundingBox() const {
	BoundingBox localbounds;
	localbounds.max = vec3f(0.5, 0.5, 0.5);
	localbounds.min = vec3f(-0.5, -0.5, -0.5);
	return localbounds;
}

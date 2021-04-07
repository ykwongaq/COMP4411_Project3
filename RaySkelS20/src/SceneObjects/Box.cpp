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
	
	
	double Tnear = -1, Tfar = -1;
	vec3f normal;
	bool isIntersect = this->getLocalBoundingBox().intersect(r, Tnear, Tfar, normal);

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

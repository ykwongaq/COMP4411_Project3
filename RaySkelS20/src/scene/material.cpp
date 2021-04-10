#include "ray.h"
#include "material.h"
#include "light.h"

// Apply the phong model to this point on the surface of the object, returning
// the color of that point.
vec3f Material::shade( Scene *scene, const ray& r, const isect& i ) const
{
	// YOUR CODE HERE

	// For now, this method just returns the diffuse color of the object.
	// This gives a single matte color for every distinct surface in the
	// scene, and that's it.  Simple, but enough to get you started.
	// (It's also inconsistent with the phong model...)

	// Your mission is to fill in this method with the rest of the phong
	// shading model, including the contributions of all the light sources.
    // You will need to call both distanceAttenuation() and shadowAttenuation()
    // somewhere in your code in order to compute shadows and light falloff.

	// Get the intersection point
	const vec3f point = r.at(i.t);

	// Result intensity
	vec3f result = this->ke;

	// Add up the ambient component
	vec3f ambientLights;
	for (list<Light *>::const_iterator it = scene->beginLights(); it != scene->endLights(); ++it) {
		if (AmbientLight *ambLight = dynamic_cast<AmbientLight *>(*it)) {
			ambientLights += ambLight->getColor(point);
		}
	}
	result += prod(prod(this->ka, ambientLights), vec3f(1.0, 1.0, 1.0) - kt);

	// Add the diffusion and specular component
	for (list<Light *>::const_iterator it = scene->beginLights(); it != scene->endLights(); ++it) {
		if (AmbientLight * ambLignt = dynamic_cast<AmbientLight *>(*it)) {
			// Do nothing
		} else {

			Light *light = *it;

			// Diffusion component
			const vec3f		N	= i.N;	// Normal of intersection point					
			const vec3f		L	= light->getDirection(point);	// Direction to the light source
			const double	NL	= N.dot(L);
			
			vec3f diffuse = prod(this->kd * NL, vec3f(1.0f, 1.0f, 1.0f) - this->kt);
			if (NL <= 0.0) continue;

			// Specular component
			const vec3f		R	= (2.0 * NL * N - L).normalize();	// Direction of reflection
			const vec3f		V	= -r.getDirection();				// Direction from intersection to camera
			const double	VR = max<double>(R.dot(V), 0.0);

			vec3f specular	= this->ks * pow(VR, this->shininess * 128);

			// Combine diffusion and specular component
			vec3f intensity = diffuse + specular;

			const vec3f atten	= light->shadowAttenuation(point) * light->distanceAttenuation(point);
			const vec3f color	= light->getColor(point);

			result += prod(prod(atten, color), intensity);
		}
	}

	return result;
}

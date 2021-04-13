#ifndef __LIGHT_H__
#define __LIGHT_H__

#include "scene.h"

class Light
	: public SceneElement
{
public:
	virtual vec3f shadowAttenuation(const vec3f& P) const = 0;
	virtual double distanceAttenuation( const vec3f& P ) const = 0;
	virtual vec3f getColor( const vec3f& P ) const = 0;
	virtual vec3f getDirection( const vec3f& P ) const = 0;

protected:
	Light( Scene *scene, const vec3f& col )
		: SceneElement( scene ), color( col ) {}

	vec3f 		color;
};

class DirectionalLight
	: public Light
{
public:
	DirectionalLight( Scene *scene, const vec3f& orien, const vec3f& color )
		: Light( scene, color ), orientation( orien ) {}
	virtual vec3f shadowAttenuation(const vec3f& P) const;
	virtual double distanceAttenuation( const vec3f& P ) const;
	virtual vec3f getColor( const vec3f& P ) const;
	virtual vec3f getDirection( const vec3f& P ) const;

protected:
	vec3f 		orientation;
};

class PointLight
	: public Light
{
public:
	PointLight(Scene *scene, const vec3f &pos, const vec3f &color);
	PointLight(Scene *scene, const vec3f &pos, const vec3f &color,
				const double &const_coeff, const double &linear_coeff, const double &quad_coeff);
	virtual vec3f shadowAttenuation(const vec3f& P) const;
	virtual double distanceAttenuation( const vec3f& P ) const;
	virtual vec3f getColor( const vec3f& P ) const;
	virtual vec3f getDirection( const vec3f& P ) const;

protected:
	vec3f position;
	double const_coeff, linear_coeff, quad_coeff;
};

// Add ambient light
class AmbientLight : public Light {
public:
	AmbientLight(Scene *scene, const vec3f &color) : Light(scene, color) {}

	// For ambient light, only the color is important
	virtual vec3f shadowAttenuation(const vec3f &P) const;
	virtual double distanceAttenuation(const vec3f &P) const;
	virtual vec3f getColor(const vec3f &P) const;
	virtual vec3f getDirection(const vec3f &P) const;
};

// Bonus 3 : Spot Light
class SpotLight : public Light {
public:
	SpotLight(Scene *scene, const vec3f &color, const vec3f &dir, const vec3f& pos, const vec3f &edge);
	SpotLight(Scene *scene, const vec3f &color, const vec3f &dir, const vec3f &pos, const vec3f &edge, const double& const_coeff, const double& linear_coeff, const double& quad_coeff);
	virtual vec3f shadowAttenuation(const vec3f &P) const;
	virtual double distanceAttenuation(const vec3f &P) const;
	virtual vec3f getColor(const vec3f &P) const;
	virtual vec3f getDirection(const vec3f &P) const;

	double getConeAngle() const;
private:
	vec3f position;
	vec3f direction;
	double coneAngle;
	double const_coeff, linear_coeff, quad_coeff;
};

// Bonus 9 : Warn Model
//class WarnLight : public PointLight {
//public:
//	
//	// Type of light shape
//	enum struct Type {
//		kCircle = 0,
//		kSquare,
//		kTriangle,
//		kStar
//	};
//	
//	WarnLight(Scene *scene, const vec3f &pos, const vec3f &dir, const vec3f &color, const Type& type);
//	WarnLight(Scene *scene, const vec3f &pos, const vec3f &dir, const vec3f &color, cosnt Type& type, const double &const_coeff, const double &linear_coeff, cosnt double &quad_coeff);
//	
//	double distanceAttenuation(const vec3f &p) const override;
//
//private:
//
//	void setUpMatrix(const vec3f &v, const vec3f &u);
//
//	vec3f direction;
//	vec3f u, v;
//
//	Type type;
//	mat4f matrix;
//
//	double size;
//};
#endif // __LIGHT_H__

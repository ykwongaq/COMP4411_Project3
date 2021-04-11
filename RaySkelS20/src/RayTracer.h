#ifndef __RAYTRACER_H__
#define __RAYTRACER_H__

// The main ray tracer.

#include "scene/scene.h"
#include "scene/ray.h"

class RayTracer
{
public:
    RayTracer();
    ~RayTracer();

    vec3f trace( Scene *scene, double x, double y );
	vec3f traceRay( Scene *scene, const ray& r, const vec3f& thresh, int depth );


	void getBuffer( unsigned char *&buf, int &w, int &h );
	double aspectRatio();
	void traceSetup( int w, int h );
	void traceLines( int start = 0, int stop = 10000000 );
	void tracePixel( int i, int j );

	bool loadScene( char* fn );
	bool loadBackground(char* fn);
	vec3f getBackgroundColor(double x, double y);

	bool sceneLoaded();

	bool background_switch; // to check the BG is checked or not

private:
	unsigned char *buffer;
	unsigned char* background;
	int buffer_width, buffer_height;
	int background_width, background_height; // to check the BG size
	int bufferSize;
	Scene *scene;

	bool m_bSceneLoaded;
};

#endif // __RAYTRACER_H__

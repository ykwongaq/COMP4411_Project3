//
// TraceUI.h
//
// Handles FLTK integration and other user interface tasks
//
#include <stdio.h>
#include <time.h>
#include <string.h>

#include <FL/fl_ask.h>

#include "TraceUI.h"
#include "../RayTracer.h"

static bool done;


//------------------------------------- Help Functions --------------------------------------------
TraceUI* TraceUI::whoami(Fl_Menu_* o)	// from menu item back to UI itself
{
	return ( (TraceUI*)(o->parent()->user_data()) );
}

//--------------------------------- Callback Functions --------------------------------------------
void TraceUI::cb_load_scene(Fl_Menu_* o, void* v) 
{
	TraceUI* pUI=whoami(o);
	
	char* newfile = fl_file_chooser("Open Scene?", "*.ray", NULL );

	if (newfile != NULL) {
		char buf[256];

		if (pUI->raytracer->loadScene(newfile)) {
			sprintf(buf, "Ray <%s>", newfile);
			done=true;	// terminate the previous rendering
		} else{
			sprintf(buf, "Ray <Not Loaded>");
		}

		pUI->m_mainWindow->label(buf);
	}
}

void TraceUI::cb_save_image(Fl_Menu_* o, void* v) 
{
	TraceUI* pUI=whoami(o);
	
	char* savefile = fl_file_chooser("Save Image?", "*.bmp", "save.bmp" );
	if (savefile != NULL) {
		pUI->m_traceGlWindow->saveImage(savefile);
	}
}

void TraceUI::cb_load_background(Fl_Menu_* o, void* v)
{
	TraceUI* pUI = whoami(o);
	char* newfile = fl_file_chooser("Open Background?", "*.bmp", NULL);
	if (newfile != NULL) {
		pUI->raytracer->loadBackground(newfile);
	}
}
void TraceUI::cb_exit(Fl_Menu_* o, void* v)
{
	TraceUI* pUI=whoami(o);

	// terminate the rendering
	done=true;

	pUI->m_traceGlWindow->hide();
	pUI->m_mainWindow->hide();
}

void TraceUI::cb_exit2(Fl_Widget* o, void* v) 
{
	TraceUI* pUI=(TraceUI *)(o->user_data());
	
	// terminate the rendering
	done=true;

	pUI->m_traceGlWindow->hide();
	pUI->m_mainWindow->hide();
}

void TraceUI::cb_about(Fl_Menu_* o, void* v) 
{
	fl_message("RayTracer Project, FLTK version for CS 341 Spring 2002. Latest modifications by Jeff Maurer, jmaurer@cs.washington.edu");
}

void TraceUI::cb_sizeSlides(Fl_Widget* o, void* v)
{
	TraceUI* pUI=(TraceUI*)(o->user_data());
	
	pUI->m_nSize=int( ((Fl_Slider *)o)->value() ) ;
	int	height = (int)(pUI->m_nSize / pUI->raytracer->aspectRatio() + 0.5);
	pUI->m_traceGlWindow->resizeWindow( pUI->m_nSize, height );
}

void TraceUI::cb_depthSlides(Fl_Widget* o, void* v)
{
	((TraceUI*)(o->user_data()))->m_nDepth=int( ((Fl_Slider *)o)->value() ) ;
}

void TraceUI::cb_atteunConstantSlides(Fl_Widget* o, void* v)
{
	((TraceUI*)(o->user_data()))->m_nAtteunConstant = int(((Fl_Slider*)o)->value());
}
void TraceUI::cb_atteunLinearSlides(Fl_Widget* o, void* v)
{
	((TraceUI*)(o->user_data()))->m_nAtteunLinear = int(((Fl_Slider*)o)->value());
}
void TraceUI::cb_atteunQuadricSlides(Fl_Widget* o, void* v)
{
	((TraceUI*)(o->user_data()))->m_nAtteunQuadric = int(((Fl_Slider*)o)->value());
}
void TraceUI::cb_ambientLightSlides(Fl_Widget* o, void* v)
{
	((TraceUI*)(o->user_data()))->m_nAmbientLight = int(((Fl_Slider*)o)->value());
}
void TraceUI::cb_thresholdSlides(Fl_Widget* o, void* v)
{
	((TraceUI*)(o->user_data()))->m_nThreshold = int(((Fl_Slider*)o)->value());
}
void TraceUI::cd_jitteringLightButton(Fl_Widget* o, void* v)
{
	((TraceUI*)(o->user_data()))->m_nJittering = int(((Fl_Slider*)o)->value());
}
void TraceUI::cd_AdaptiveLightButton(Fl_Widget* o, void* v)
{
	((TraceUI*)(o->user_data()))->m_nAdaptive = int(((Fl_Slider*)o)->value());
}

void TraceUI::cd_BackgroundButton(Fl_Widget* o, void* v)
{
	((TraceUI*)(o->user_data()))->raytracer->background_switch = ((TraceUI*)(o->user_data()))->m_nbackground ^= true;
}

void TraceUI::cb_render(Fl_Widget* o, void* v)
{
	char buffer[256];

	TraceUI* pUI=((TraceUI*)(o->user_data()));
	
	if (pUI->raytracer->sceneLoaded()) {
		int width=pUI->getSize();
		int	height = (int)(width / pUI->raytracer->aspectRatio() + 0.5);
		pUI->m_traceGlWindow->resizeWindow( width, height );

		pUI->m_traceGlWindow->show();

		pUI->raytracer->traceSetup(width, height);
		
		// Save the window label
		const char *old_label = pUI->m_traceGlWindow->label();

		// start to render here	
		done=false;
		clock_t prev, now;
		prev=clock();
		
		pUI->m_traceGlWindow->refresh();
		Fl::check();
		Fl::flush();

		for (int y=0; y<height; y++) {
			for (int x=0; x<width; x++) {
				if (done) break;
				
				// current time
				now = clock();

				// check event every 1/2 second
				if (((double)(now-prev)/CLOCKS_PER_SEC)>0.5) {
					prev=now;

					if (Fl::ready()) {
						// refresh
						pUI->m_traceGlWindow->refresh();
						// check event
						Fl::check();

						if (Fl::damage()) {
							Fl::flush();
						}
					}
				}

				pUI->raytracer->tracePixel( x, y );
		
			}
			if (done) break;

			// flush when finish a row
			if (Fl::ready()) {
				// refresh
				pUI->m_traceGlWindow->refresh();

				if (Fl::damage()) {
					Fl::flush();
				}
			}
			// update the window label
			sprintf(buffer, "(%d%%) %s", (int)((double)y / (double)height * 100.0), old_label);
			pUI->m_traceGlWindow->label(buffer);
			
		}
		done=true;
		pUI->m_traceGlWindow->refresh();

		// Restore the window label
		pUI->m_traceGlWindow->label(old_label);		
	}
}

void TraceUI::cb_stop(Fl_Widget* o, void* v)
{
	done=true;
}

void TraceUI::show()
{
	m_mainWindow->show();
}

void TraceUI::setRayTracer(RayTracer *tracer)
{
	raytracer = tracer;
	m_traceGlWindow->setRayTracer(tracer);
}

int TraceUI::getSize()
{
	return m_nSize;
}

int TraceUI::getDepth()
{
	return m_nDepth;
}

// menu definition
Fl_Menu_Item TraceUI::menuitems[] = {
	{ "&File",		0, 0, 0, FL_SUBMENU },
		{ "&Load Scene...",	FL_ALT + 'l', (Fl_Callback *)TraceUI::cb_load_scene },
		{ "&Save Image...",	FL_ALT + 's', (Fl_Callback *)TraceUI::cb_save_image },
		{ "&Load Background...",	FL_ALT + 'b', (Fl_Callback*)TraceUI::cb_load_background },
		{ "&Exit",			FL_ALT + 'e', (Fl_Callback *)TraceUI::cb_exit },
		{ 0 },

	{ "&Help",		0, 0, 0, FL_SUBMENU },
		{ "&About",	FL_ALT + 'a', (Fl_Callback *)TraceUI::cb_about },
		{ 0 },

	{ 0 }
};

TraceUI::TraceUI() {
	// init.
	m_nDepth = 0;
	//change the size of the m_n from 150 to 300
	m_nSize = 300;
	m_mainWindow = new Fl_Window(100, 40, 320, 500, "Ray <Not Loaded>");
		m_mainWindow->user_data((void*)(this));	// record self to be used by static callback functions
		// install menu bar
		m_menubar = new Fl_Menu_Bar(0, 0, 320, 25);
		m_menubar->menu(menuitems);

		// install slider depth
		m_depthSlider = new Fl_Value_Slider(10, 30, 180, 20, "Depth");
		m_depthSlider->user_data((void*)(this));	// record self to be used by static callback functions
		m_depthSlider->type(FL_HOR_NICE_SLIDER);
        m_depthSlider->labelfont(FL_COURIER);
        m_depthSlider->labelsize(12);
		m_depthSlider->minimum(0);
		m_depthSlider->maximum(10);
		m_depthSlider->step(1);
		m_depthSlider->value(m_nDepth);
		m_depthSlider->align(FL_ALIGN_RIGHT);
		m_depthSlider->callback(cb_depthSlides);

		// install slider size
		m_sizeSlider = new Fl_Value_Slider(10, 55, 180, 20, "Size");
		m_sizeSlider->user_data((void*)(this));	// record self to be used by static callback functions
		m_sizeSlider->type(FL_HOR_NICE_SLIDER);
        m_sizeSlider->labelfont(FL_COURIER);
        m_sizeSlider->labelsize(12);
		m_sizeSlider->minimum(64);
		m_sizeSlider->maximum(512);
		m_sizeSlider->step(1);
		m_sizeSlider->value(m_nSize);
		m_sizeSlider->align(FL_ALIGN_RIGHT);
		m_sizeSlider->callback(cb_sizeSlides);


		// install slider atteunation  constant
		m_auteunationConstantSlider = new Fl_Value_Slider(10, 80, 180, 20, "Auteunation Constant");
		m_auteunationConstantSlider->user_data((void*)(this));	// record self to be used by static callback functions
		m_auteunationConstantSlider->type(FL_HOR_NICE_SLIDER);
		m_auteunationConstantSlider->labelfont(FL_COURIER);
		m_auteunationConstantSlider->labelsize(12);
		m_auteunationConstantSlider->minimum(0.0);
		m_auteunationConstantSlider->maximum(1.0);
		m_auteunationConstantSlider->step(0.01);
		m_auteunationConstantSlider->value(m_nAtteunConstant);
		m_auteunationConstantSlider->align(FL_ALIGN_RIGHT);
		m_auteunationConstantSlider->callback(cb_atteunConstantSlides);

		// install slider atteunation  linear
		m_auteunationLinearSlider = new Fl_Value_Slider(10, 105, 180, 20, "Auteunation Linear");
		m_auteunationLinearSlider->user_data((void*)(this));	// record self to be used by static callback functions
		m_auteunationLinearSlider->type(FL_HOR_NICE_SLIDER);
		m_auteunationLinearSlider->labelfont(FL_COURIER);
		m_auteunationLinearSlider->labelsize(12);
		m_auteunationLinearSlider->minimum(0.0);
		m_auteunationLinearSlider->maximum(1.0);
		m_auteunationLinearSlider->step(0.01);
		m_auteunationLinearSlider->value(m_nAtteunLinear);
		m_auteunationLinearSlider->align(FL_ALIGN_RIGHT);
		m_auteunationLinearSlider->callback(cb_atteunLinearSlides);

		// install slider atteunation  quadric
		m_auteunationQuadricSlider = new Fl_Value_Slider(10, 130, 180, 20, "Auteunation Quadric");
		m_auteunationQuadricSlider->user_data((void*)(this));	// record self to be used by static callback functions
		m_auteunationQuadricSlider->type(FL_HOR_NICE_SLIDER);
		m_auteunationQuadricSlider->labelfont(FL_COURIER);
		m_auteunationQuadricSlider->labelsize(12);
		m_auteunationQuadricSlider->minimum(0.0);
		m_auteunationQuadricSlider->maximum(1.0);
		m_auteunationQuadricSlider->step(0.01);
		m_auteunationQuadricSlider->value(m_nAtteunQuadric);
		m_auteunationQuadricSlider->align(FL_ALIGN_RIGHT);
		m_auteunationQuadricSlider->callback(cb_atteunQuadricSlides);


		// install slider ambient light 
		m_auteunationQuadricSlider = new Fl_Value_Slider(10, 155, 180, 20, "Ambient Light");
		m_auteunationQuadricSlider->user_data((void*)(this));	// record self to be used by static callback functions
		m_auteunationQuadricSlider->type(FL_HOR_NICE_SLIDER);
		m_auteunationQuadricSlider->labelfont(FL_COURIER);
		m_auteunationQuadricSlider->labelsize(12);
		m_auteunationQuadricSlider->minimum(0.0);
		m_auteunationQuadricSlider->maximum(1.0);
		m_auteunationQuadricSlider->step(0.01);
		m_auteunationQuadricSlider->value(m_nAmbientLight);
		m_auteunationQuadricSlider->align(FL_ALIGN_RIGHT);
		m_auteunationQuadricSlider->callback(cb_ambientLightSlides);


		// install slider theshold
		m_thresholdSlider = new Fl_Value_Slider(10, 180, 180, 20, "Threshold");
		m_thresholdSlider->user_data((void*)(this));	// record self to be used by static callback functions
		m_thresholdSlider->type(FL_HOR_NICE_SLIDER);
		m_thresholdSlider->labelfont(FL_COURIER);
		m_thresholdSlider->labelsize(12);
		m_thresholdSlider->minimum(0.0);
		m_thresholdSlider->maximum(1.0);
		m_thresholdSlider->step(0.01);
		m_thresholdSlider->value(m_nThreshold);
		m_thresholdSlider->align(FL_ALIGN_RIGHT);
		m_thresholdSlider->callback(cb_thresholdSlides);




		//for button
		m_renderButton = new Fl_Button(240, 27, 70, 25, "&Render");
		m_renderButton->user_data((void*)(this));
		m_renderButton->callback(cb_render);

		m_stopButton = new Fl_Button(240, 55, 70, 25, "&Stop");
		m_stopButton->user_data((void*)(this));
		m_stopButton->callback(cb_stop);

		m_jitteringCheckButton = new Fl_Check_Button(10, 210, 60, 20, "Jittering Light");
		m_jitteringCheckButton->user_data((void*)(this));
		m_jitteringCheckButton->value(m_nJittering);
		m_jitteringCheckButton->callback(cd_jitteringLightButton);

		m_adaptiveCheckButton = new Fl_Check_Button(130, 210, 60, 20, "Adaptive");
		m_adaptiveCheckButton->user_data((void*)(this));
		m_adaptiveCheckButton->value(m_nAdaptive);
		m_adaptiveCheckButton->callback(cd_AdaptiveLightButton);

		m_backgroundCheckButton = new Fl_Check_Button(220, 210, 70, 25, "Background");
		m_backgroundCheckButton->user_data((void*)(this));
		m_backgroundCheckButton->value(m_nbackground);
		m_backgroundCheckButton->callback(cd_BackgroundButton);

		m_mainWindow->callback(cb_exit2);
		m_mainWindow->when(FL_HIDE);
    m_mainWindow->end();

	// image view
	m_traceGlWindow = new TraceGLWindow(100, 150, m_nSize, m_nSize, "Rendered Image");
	m_traceGlWindow->end();
	m_traceGlWindow->resizable(m_traceGlWindow);
}
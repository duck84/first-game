/*Hello Triagnle
 *code adapted from OpenGL ES 2.0 Porgramming Guide
 *and code snippets from RPI Forum to set up Dispmanx*/

#include <stdio.h>
#include <assert.h>
#include <math.h>
#include <sys/time.h>
#include "bcm_host.h"
#include <EGL/egl.h>
#include <EGL/eglext.h>
#include <GLES2/gl2.h>

#define TRUE 1
#define FALSE 0

typedef struct
{
	//save a Handle to a program object
	GLuint programObject;
} UserData;

typedef struct Target_State
{
	uint32_t width;
	uint32_t height;
	
	EGLDisplay display;
	EGLSurface surface;
	EGLContext context;
	
	EGL_DISPMANX_WINDOW_T nativewindow;
	UserData *user_data;
	void(*draw_func)(struct Target_State*);
	
} Target_State;

Target_State state;
Target_State* p_state = &state;

static const EGLint attribute_list[] =
{ 
	EGL_RED_SIZE,
	8,
	EGL_GREEN_SIZE,
	8,
	EGL_BLUE_SIZE,
	8,
	EGL_ALPHA_SIZE,
	8,
	EGL_SURFACE_TYPE,
	EGL_WINDOW_BIT,
	EGL_NONE
};

static const EGLint context_attributes[] =
{ 
	EGL_CONTEXT_CLIENT_VERSION,
	2,
	EGL_NONE
};

/*
 *Now we have to create a shader object, pass the shader source 
 *and then compile the shader
**/
GLint LoadShader(GLenum type, const char *shaderSrc)
{
	// 1st create the shader object
	GLuint TheShader = glCreateShader(type);
	
	if (TheShader == 0) return 0; // can't allocate so stop.
	// pass the shader source
	glShaderSource(TheShader, 1, &shaderSrc, NULL);
	// Compile the shader
	glCompileShader(TheShader);
	
	GLint IsItCompiled;
	
	
}

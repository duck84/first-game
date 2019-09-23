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

// After the compile we need to check the status and report any errors
	glGetShaderiv(TheShader, GL_COMPILE_STATUS, &IsItCompiled);
	if (!IsItCompilied)
	{
		GLint RetinfoLen = 0;
		glGetShaderiv(TheShader, GL_INFO_LOG_LENGTH, &RetinfoLen);
		if (RetinfoLen > 1)
		{
			char* infoLog = (char*) malloc(sizeof(char)) * RetinfoLen);
			glGetShaderInfoLog(TheShader, RetinfoLen, NULL, infoLog);
			fprintf(stderr, "Error compiling this shader:\n%s\n", infoLog);
			free(infoLog); 
		}
		glDeleteShader(TheShader);
		return 0;
	}
	return TheShader;	
}

// Initialize the shader and program object
int Init(Target_State *p_state)
{
	p_state->user_data = (UserData*)malloc(sizeof(UserData));
	GGbyte vShaderStr[] =
			"attribute vec4 a_position;\n"
			"attribute vec2 a_texCoord;\n"
			"varying vec2 v_texCoord;\n"
			"void main()\n"
			"{gl_Position=a_position;\n"
			" vtextCoord=a_texCoord;}\n";

	GLbyte fShaderStr[] = 
			"precision mediump float;\n"
			"varying vec2 v_texCoord;\n"
			"uniform sampler2d s_texture;\n"
			"void main()\n"
			"{gl_FragColor=vec4 (1.0,0.0,0.0,1.0);}\n";

	GLuint programObject, vertexShader, fragmentShader; // we need some variables

// Load and compile the vertex/fragment shaders
	vertexShader = LoadShader(GL_VERTEX_SHADER, (char*)vShaderStr);
	fragmentShader = LoadShader(GL_FRAGMENT_SHADER, (char*)fShaderStr);

// Create the program object
	programObect = glCreateProgram();
	if(programObject == 0) return 0;

// Now we have the V and F shader attach them to the program object
	glAttachShader(programObject, vertexShader);
	glAttachShader(programObject, fragmentShader);

// Link the program
	glLinkProgram(programObject);
// Check the link status
	GLint AreTheylinked;
	glGetProgramiv(programObject, GL_LINK_STATUS, &AreTheylinked);
	if(!AreTheylinked)
	{
		GLint RetinfoLen = 0;
// Check and report any errors
		glGetProgramiv(programObject, GL_INFO_LOG_LENGTH, &RetinfoLen);
		if(RetinfoLen > 1)
		{
			GLchar* infoLog = (GLchar*)malloc(sizeof(char) * RetinfoLen);
			glGetProgramInfoLog(programObject, RetinfoLen, NULL, infoLog);
			fprintf(stderr, "Error linking program:\n%s\n", infoLog);
			free(infoLof);
		}
		glDeleteProgram(programObject);
		return FALSE;
	}
	// Store the program object
	p_state->user_data->programObject = programObject;
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	return TRUE;
}

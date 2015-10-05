/*
 * OpenGL ES SandBox by CyrIng
 * 2013-0.0.2
 * GPL2
 */
#include	<stdio.h>
#include	<string.h>
#include	<stdlib.h>
#ifndef __x86_64__
#include	"bcm_host.h"
#include	"GLES/gl.h"
#include	"EGL/egl.h"
#include	"EGL/eglext.h"
#else
#include	<X11/Xlib.h>
#include	<GL/glut.h>
#endif

#ifndef __x86_64__
EGLDisplay		D;
EGLSurface		S;
EGLContext		X;
EGL_DISPMANX_WINDOW_T	W={0};
uint32_t		WIDTH, HEIGHT;
#else
Display			*D;
int			W;
int			WIDTH, HEIGHT;
#endif

#define	ENTER	13
#define	ESCAPE	27

GLfloat	Rgb=1.0f,rGb=0.6f,rgB=0.6f;
GLfloat	xr=0.0f,yr=0.0f,zr=1.0f;
GLfloat	trans = -5.0f;

typedef	struct 
{
	GLfloat	x,
		y,
		z;
}	V3D;

typedef	struct
{
	void	(*callBack)(void);
	V3D	*vertex;
}	OBJ;

OBJ	sandBox[10]={{NULL, NULL}};
int	M=10;

void	PlaneGL()
{
	glDrawArrays(GL_TRIANGLES, 0, 3);
	glDrawArrays(GL_TRIANGLES, 1, 3);
}

void	CubeGL()
{
	glDrawArrays(GL_LINES, 0, 2);
	glDrawArrays(GL_LINES, 2, 2);
	glDrawArrays(GL_LINES, 4, 2);
	glDrawArrays(GL_LINES, 6, 2);
	glColor4f(0,0.5,0,1);
	glDrawArrays(GL_TRIANGLES, 0, 3);
	glColor4f(0,0.6,0,1);
	glDrawArrays(GL_TRIANGLES, 1, 3);
	glColor4f(0.5,0.2,0.2,1);
	glDrawArrays(GL_TRIANGLES, 4, 3);
	glColor4f(0.6,0.2,0.2,1);
	glDrawArrays(GL_TRIANGLES, 5, 3);
}

void	DrawGLScene()
{
	gluLookAt(1,1,1,0,0,0,0,1,0);
	glTranslatef(0.0f, 0.0f, 0.0f);		// translate
	glRotatef(0.0f, 1.0f, 0.0f, 0.0f);	// X rotate
	glRotatef(0.0f, 0.0f, 1.0f, 0.0f);	// Y rotate
	glRotatef(0.0f, 0.0f, 0.0f, 1.0f);	// Z rotate

	int I;
	for(I=M; I<10; I++)
	{
		glVertexPointer(3, GL_FLOAT, 0, sandBox[I].vertex);
		glEnableClientState(GL_VERTEX_ARRAY);
		(* sandBox[I].callBack)();
	}
	glFlush();
#ifndef __x86_64__
	eglSwapBuffers(D, S);
#else
	glutSwapBuffers();
#endif
}

void	StartGL()
{
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
	glEnable(GL_NORMALIZE);
	glClearColor(0.0f, 0.0f, 0.5f, 0.0f);
	glClearDepth(1.0);
	glDepthFunc(GL_LESS);
	glEnable(GL_DEPTH_TEST);
	glShadeModel(GL_SMOOTH);
	glViewport(0, 0, (GLsizei) WIDTH, (GLsizei) HEIGHT);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0f,(GLfloat) WIDTH / (GLfloat) HEIGHT,0.1f,100.0f);
	glMatrixMode(GL_MODELVIEW);
	glClear(GL_COLOR_BUFFER_BIT);
	glColor4f(Rgb, rGb, rgB, 1);
}

#ifndef __x86_64__
void	InitVC()
{
	DISPMANX_ELEMENT_HANDLE_T dispman_element;
	DISPMANX_DISPLAY_HANDLE_T dispman_display;
	DISPMANX_UPDATE_HANDLE_T  dispman_update;
	VC_RECT_T dst_rect={0}, src_rect={0};
	graphics_get_display_size(0, &WIDTH, &HEIGHT);
	dst_rect.width= WIDTH;
	dst_rect.height=HEIGHT;
	src_rect.width= WIDTH  << 16;
	src_rect.height=HEIGHT << 16;
	dispman_display = vc_dispmanx_display_open(0);
	dispman_update  = vc_dispmanx_update_start(0);
	dispman_element = vc_dispmanx_element_add( dispman_update, dispman_display, \
						0, &dst_rect, 0, \
						&src_rect, DISPMANX_PROTECTION_NONE, \
						0, 0, 0);
	W.element=dispman_element; W.width=WIDTH; W.height=HEIGHT;
	vc_dispmanx_update_submit_sync(dispman_update);
}

void	InitEGL()
{
	EGLConfig  C;
	EGLint     N;
	D = eglGetDisplay(EGL_DEFAULT_DISPLAY);
	eglInitialize(D, NULL, NULL);
	static const EGLint attrib_list[]={
				EGL_RED_SIZE, 8,
				EGL_GREEN_SIZE, 8,
				EGL_BLUE_SIZE, 8,
				EGL_ALPHA_SIZE, 8,
				EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
				EGL_NONE
				};
	eglChooseConfig(D, attrib_list, &C, 1, &N);
	S = eglCreateWindowSurface( D, C, &W, NULL );
	X = eglCreateContext(D, C, EGL_NO_CONTEXT, NULL);
	eglMakeCurrent(D, S, S, X);
}

void	LoopGL()
{
	DrawGLScene();
	sleep(10);
	eglMakeCurrent(D, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
	eglDestroySurface(D, S);
	eglDestroyContext(D, X);
	eglTerminate(D);
}
#else

void KeyPressed(unsigned char key, int x, int y)
{
	switch(key)
	{
	  case ENTER:
	  {
	  }
	  break;
	  case ESCAPE:
	  {
		glutDestroyWindow(W);
	  }
	  break;
	  case '1':
		trans=-5.0f;
	  break;
	  case '2':
		trans=-8.0f;
	  break;
	  case '3':
		trans=-10.0f;
	  break;
	  case '4':
		trans=-12.0f;
	  break;
	  case '5':
		trans=-15.0f;
	  break;
	  case '6':
		trans=-20.0f;
	  break;
	  case '7':
		trans=-25.0f;
	  break;
	  case '8':
		trans=-30.0f;
	  break;
	  case '9':
		trans=-35.0f;
	  break;
	}
}

void SpecialKeyPressed(int key, int x, int y)
{
	  switch (key)
	  {
	  case GLUT_KEY_PAGE_UP:
		zr+=5.0f;
	  break;
	  case GLUT_KEY_PAGE_DOWN:
		zr-=5.0f;
	  break;
	  case GLUT_KEY_UP:
		xr+=1.0f;
	  break;
	  case GLUT_KEY_DOWN:
		xr-=1.0f;
	  break;
	  case GLUT_KEY_LEFT:
		yr+=1.0f;
	  break;
	  case GLUT_KEY_RIGHT:
		yr-=1.0f;
	  break;
	  case GLUT_KEY_HOME:
		trans+=0.5f;
	  break;
	  case GLUT_KEY_END:
		trans-=0.5f;
	  break;
	  }
}
void	InitX()
{
	D=XOpenDisplay(NULL);
	WIDTH=DisplayWidth(  D, DefaultScreen(D));
	HEIGHT=DisplayHeight(D, DefaultScreen(D));
	XCloseDisplay(D);
}

void	InitGLUT(int *argc, char **argv)
{
	glutInit(argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_ALPHA | GLUT_DEPTH);
	glutInitWindowSize(WIDTH/4, HEIGHT*3/8);
	glutInitWindowPosition(WIDTH/4, HEIGHT/8);
	W=glutCreateWindow("SandBox");
	glutDisplayFunc(&DrawGLScene);
	glutIdleFunc(&DrawGLScene);
	glutKeyboardFunc(&KeyPressed);
	glutSpecialFunc(&SpecialKeyPressed);
}

void	LoopGL()
{
	glutMainLoop();
}
#endif

void	ImportScene(char *fileName)
{
	const struct
	{
		char *objName;
		void *callBack;
	} List[2] = {	{ "Plane", PlaneGL },
			{ "Cube" , CubeGL  }	};

	int L=0;
	char buffer[256];
	FILE *fs=fopen(fileName, "r");
	while(fs!=NULL && !feof(fs))
	{
		fgets(buffer, 256, fs);
		switch(buffer[0])
		{
		case 'o':
		{
/*			int I;
			for(I=0; I<2; I++)
			{
				if(!strncmp(List[I].objName, &buffer[2], strlen(List[I].objName)))
				{
					M--;
					sandBox[M].callBack=List[I].callBack;
					L=0;
					break;
				}
			}*/
			M--;
			sandBox[M].callBack=CubeGL;
			L=0;
		}
		break;
		case 'v':
		{
			sandBox[M].vertex=(V3D*)realloc(sandBox[M].vertex, (L+1)*sizeof(V3D));
			sscanf(	&buffer[2], "%f %f %f", \
				&sandBox[M].vertex[L].x, &sandBox[M].vertex[L].y, &sandBox[M].vertex[L].z);
			L++;
		}
		break;
		}
	}
	if(fs!=NULL) fclose(fs);
}

int	main(int argc, char *argv[])
{
#ifndef __x86_64__
	bcm_host_init();
	InitVC();
	InitEGL();
#else
	InitX();
	InitGLUT(&argc, argv);
#endif
	StartGL();
	LoopGL();
	while(M<10)
		free(sandBox[M++].vertex);
	return(0);
}

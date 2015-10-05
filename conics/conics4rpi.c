#include <stdio.h>
#include <string.h>
#include <termios.h>
#include <math.h>
#include <unistd.h>

#include "bcm_host.h"
#include "GLES/gl.h"
#include "EGL/egl.h"
#include "EGL/eglext.h"

#define	ENTER	10
#define	ESCAPE	27

enum	Boolean {F=0, T=1};

struct	conics_bool_type
{
	unsigned a : 1;
	unsigned b : 1;
	unsigned c : 1;
	unsigned k : 1;
	unsigned R : 1;
	unsigned G : 1;
	unsigned B : 1;
	unsigned s : 1;
}	conics = {0};

enum	Primitives {_POINTS='P', _LINES='L', _TRIANGLES='T', _QUADS='Q'};
char	kind = _POINTS;

GLfloat	a=-0.001f,b=0.001f,c=-0.001f,k=0.00001f;
GLfloat	xr=0.0f,yr=0.0f,zr=1.0f;
GLfloat	trans=-1.0f;
GLfloat	step=0.05f;
GLfloat	Rgb=1.0f,rGb=0.6f,rgB=0.6f;
GLfloat	theList[40401];
GLsizei	I=0;

uint32_t		WIDTH, HEIGHT;
EGLDisplay		D;
EGLSurface		S;
EGLContext		X;
EGL_DISPMANX_WINDOW_T	W={0};

void	ComputeConics()
{
	GLfloat x,y,z;

	for(y=-1.0f, I=0; y<1.0f; y+=step)
		for(x=-1.0f; x<1.0f; x+=step)
		{
			z=sqrtf( (k-a*powf(x,2) - b*powf(y,2)) / c);

			theList[I++]=x;
			theList[I++]=y;
			theList[I++]=z;
		}
}
/*
void	ReSizeGLScene(int Width, int Height)
{
	if(Height==0) Height=1;
	glViewport(0, 0, (GLsizei) Width, (GLsizei) Height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0f, (GLfloat) Width / (GLfloat) Height, 0.1f, 100.0f);
	glMatrixMode(GL_MODELVIEW);
}
*/
void	DrawGLScene(void)
{

	glTranslatef(0.0f,0.0f,trans);
	glRotatef(xr,1.0f,0.0f,0.0f);
	glRotatef(yr,0.0f,1.0f,0.0f);
	glRotatef(zr,0.0f,0.0f,1.0f);

	glDrawArrays(GL_POINTS, 0, I);

	glFlush();
	eglSwapBuffers(D, S);
}

void	LoadListGL(void)
{
        glEnable(GL_LIGHTING);
        glEnable(GL_LIGHT0);
        glEnable(GL_NORMALIZE);
        glClearColor(0.0f, 0.0f, 0.5f, 0.0f);
        glDepthFunc(GL_LESS);
        glEnable(GL_DEPTH_TEST);
        glShadeModel(GL_SMOOTH);
        glViewport(0, 0, (GLsizei) WIDTH, (GLsizei) HEIGHT);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	ComputeConics();

	glVertexPointer(3, GL_FLOAT, 0, theList);
	glEnableClientState(GL_VERTEX_ARRAY);
        glMatrixMode(GL_MODELVIEW);
	glClear(GL_COLOR_BUFFER_BIT);
	glColor4f(Rgb,rGb,rgB,1);
}

void	InitVC(void)
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
	dispman_update  = vc_dispmanx_update_start(0 );
	dispman_element = vc_dispmanx_element_add ( dispman_update, dispman_display, \
						0, &dst_rect, 0, \
						&src_rect, DISPMANX_PROTECTION_NONE, \
						0, 0, 0);
	W.element=dispman_element; W.width=WIDTH; W.height=HEIGHT;
	vc_dispmanx_update_submit_sync( dispman_update );
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

int	GetKey(int debug)
{
	int key=getchar();

	if(debug)
		printf("%04x : ", key);
	return(key);
}

void	KeyPressed(int key)
{
	switch(key)
	{
	case ENTER:
	{
		printf("%d indices to be rendered.\n", I);
	}
	break;
	case '+':
		if(conics.a) a+=0.001f;
		if(conics.b) b+=0.001f;
		if(conics.c) c+=0.001f;
		if(conics.k) k+=0.0001f;
		if(conics.R && Rgb<1.0f) Rgb+=0.1f;
		if(conics.G && rGb<1.0f) rGb+=0.1f;
		if(conics.B && rgB<1.0f) rgB+=0.1f;
		if(conics.s && step>0.01f) step-=0.005f;
		LoadListGL();
	break;
	case '-':
		if(conics.a) a-=0.001f;
		if(conics.b) b-=0.001f;
		if(conics.c) c-=0.001f;
		if(conics.k) k-=0.0001f;
		if(conics.R && Rgb>0.0f) Rgb-=0.1f;
		if(conics.G && rGb>0.0f) rGb-=0.1f;
		if(conics.B && rgB>0.0f) rgB-=0.1f;
		if(conics.s && step<0.5f) step+=0.005f;
		LoadListGL();
	break;
	case 'a':
		conics=(struct conics_bool_type){.a=T, .b=F, .c=F, .k=F, .R=F, .G=F, .B=F, .s=F};
	break;
	case 'b':
		conics=(struct conics_bool_type){.a=F, .b=T, .c=F, .k=F, .R=F, .G=F, .B=F, .s=F};
	break;
	case 'c':
		conics=(struct conics_bool_type){.a=F, .b=F, .c=T, .k=F, .R=F, .G=F, .B=F, .s=F};
	break;
	case 'k':
		conics=(struct conics_bool_type){.a=F, .b=F, .c=F, .k=T, .R=F, .G=F, .B=F, .s=F};
	break;
	case 'R':
		conics=(struct conics_bool_type){.a=F, .b=F, .c=F, .k=F, .R=T, .G=F, .B=F, .s=F};
	break;
	case 'G':
		conics=(struct conics_bool_type){.a=F, .b=F, .c=F, .k=F, .R=F, .G=T, .B=F, .s=F};
	break;
	case 'B':
		conics=(struct conics_bool_type){.a=F, .b=F, .c=F, .k=F, .R=F, .G=F, .B=T, .s=F};
	break;
	case 's':
		conics=(struct conics_bool_type){.a=F, .b=F, .c=F, .k=F, .R=F, .G=F, .B=F, .s=T};
	break;
	case 'z':
	case 'Z':
		conics=(struct conics_bool_type){.a=F, .b=F, .c=F, .k=F, .R=F, .G=F, .B=F, .s=F};
		a=-0.001f,b=0.001f,c=-0.001f,k=0.00001f;
		xr=0.0f,yr=0.0f,zr=1.0f;
		trans = -1.0f;
		LoadListGL();
	break;
	}
}

void	SpecialKeyPressed(int key)
{
/*	switch (key)
	{
	case GLUT_KEY_PAGE_UP:
		zr-=1.0f;
	break;
	case GLUT_KEY_PAGE_DOWN:
		zr+=1.0f;
	break;
	case GLUT_KEY_UP:
		xr-=1.0f;
	break;
	case GLUT_KEY_DOWN:
		xr+=1.0f;
	break;
	case GLUT_KEY_LEFT:
		yr-=1.0f;
	break;
	case GLUT_KEY_RIGHT:
		yr+=1.0f;
	break;
	case GLUT_KEY_HOME:
		trans+=0.1f;
	break;
	case GLUT_KEY_END:
		trans-=0.1f;
	break;
	case GLUT_KEY_F1:
		kind=_POINTS;
		LoadListGL();
	break;
	case GLUT_KEY_F2:
		kind=_LINES;
		LoadListGL();
	break;
	case GLUT_KEY_F3:
		kind=_TRIANGLES;
		LoadListGL();
	break;
	case GLUT_KEY_F4:
		kind=_QUADS;
		LoadListGL();
	break;
	}*/
}

void    LoopGL()
{
	int	key=0;
	struct	termios oldt, newt;
	tcgetattr ( STDIN_FILENO, &oldt );
	newt = oldt;
	newt.c_lflag &= ~( ICANON | ECHO );
	newt.c_cc[VTIME] = 0;
	newt.c_cc[VMIN] = 1;
	tcsetattr ( STDIN_FILENO, TCSANOW, &newt );

	while( (key=GetKey(1)) != ESCAPE )
	{
		KeyPressed(key);
		SpecialKeyPressed(key);
		DrawGLScene();
	}

	eglMakeCurrent(D, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
	eglDestroySurface(D, S);
	eglDestroyContext(D, X);
	eglTerminate(D);

	tcsetattr ( STDIN_FILENO, TCSANOW, &oldt );
}

int	main(int argc, char **argv)
{
	bcm_host_init();
	InitVC();
	InitEGL();
	LoadListGL();
        LoopGL();

	return(0);
}

/*
 * RollerCoaster
 *
 * for Raspberry Pi computer.
 */
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <termios.h>
#include <sys/select.h>
#include <sys/time.h>
#ifndef __x86_64__
#include "bcm_host.h"
#include "GLES/gl.h"
#include "EGL/egl.h"
#include "EGL/eglext.h"
#else
#include <X11/Xlib.h>
#include <GL/glut.h>
#endif
#include "roller.h"

#define	ENTER	10
#define	ESCAPE	27

#ifndef __x86_64__
uint32_t		WIDTH, HEIGHT;
EGLDisplay		D;
EGLSurface		S;
EGLContext		X;
EGL_DISPMANX_WINDOW_T	W={0};
#else
Display			*D;
int			W;
int			WIDTH, HEIGHT;
#endif

struct rollercoaster iRollerCoaster={{0}};

unsigned long long int iStartTime=0;

unsigned long long int UniversalTime(void)
{
	struct timezone tz={0};
	struct timeval tp={0};
	gettimeofday(&tp, &tz);
	return((tp.tv_sec*1000000) + tp.tv_usec);
}

void	DrawGL(void)
{
	unsigned long long int curTime=UniversalTime();
	unsigned long long int timeTick=(curTime-iStartTime)/1000;

	DrawRoller(timeTick, &iRollerCoaster);

#ifndef __x86_64__
	eglSwapBuffers(D, S);
#else
	glutSwapBuffers();
#endif
}

#ifndef __x86_64__
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
#endif

#ifndef __x86_64__
void	KeyPressed(int key)
#else
void	KeyPressed(unsigned char key, int x, int y)
#endif
{
	switch(key)
	{
		case ENTER:
		{
		}
		break;
		case ESCAPE:
#ifdef __x86_64__
		{
			glutDestroyWindow(W);
		}
#endif
		break;
	}
}

#ifndef __x86_64__
int	KbHit()
{
	int KbHit=0;
	struct timeval tv={0};
	fd_set fds;
	FD_ZERO(&fds);
	FD_SET(STDIN_FILENO, &fds);
	select(STDIN_FILENO+1, &fds, NULL, NULL, &tv);
	KbHit=FD_ISSET(STDIN_FILENO, &fds);
	return(KbHit);
}

int	GetKey(int debug)
{
	if(KbHit())
	{
		int key=getchar();
		if(debug) printf("%04x : ", key);
		return(key);
	}
	else
		return(0);
}
#else
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
	W=glutCreateWindow("RollerCoaster");
}
#endif

int	main(int argc, char **argv)
{
#ifndef __x86_64__
	bcm_host_init();
	InitVC();
	InitEGL();
#else
	InitX();
	InitGLUT(&argc, argv);
#endif

	static const point array[] = {
		{4.0f, -2.0f, 0.2f}, {0.0f, 0.0f, 0.0f},
		{3.0f, 0.0f, 0.21f}, {-0.5f, 1.0f, 0.0f},
		{2.0f, 2.0f, 3.5f}, {0.0f, 0.0f, 0.0f},
		{0.0f, 3.0f, 3.4f}, {0.0f, 0.0f, 0.0f},
		{-3.0f, 0.0f, 0.2f}, {0.0f, 0.0f, 0.0f},
		{-3.0f, -2.0f, 2.5f}, {0.0f, 0.0f, 0.0f},
		{0.0f, -3.0f, 0.2f}, {1.0f, 0.0f, 0.0f},
		{1.0f, -3.1f, 1.2f}, {0.0f, 0.0f, 0.0f},
		{0.0f, -3.2f, 2.2f}, {0.0f, 0.0f, 0.0f},
		{-1.0f, -3.3f, 1.2f}, {0.0f, 0.0f, 0.0f},
		{0.0f, -3.4f, 0.2f}, {1.0f, 0.0f, 0.0f},
		{0.5f, -3.4f, 0.2f}, {1.0f, 0.0f, 0.0f},
		{1.5f, -3.5f, 1.2f}, {0.0f, 0.0f, 0.0f},
		{0.5f, -3.6f, 2.2f}, {0.0f, 0.0f, 0.0f},
		{-0.5f, -3.7f, 1.2f}, {0.0f, 0.0f, 0.0f},
		{0.5f, -3.8f, 0.2f}, {1.0f, 0.0f, 0.0f},
		{4.0f, -4.0f, 2.5f}, {0.0f, 0.0f, 0.0f},
		{4.0f, -1.0f, 3.0f}, {0.0f, 0.0f, 0.0f},
		{2.0f, 2.0f, 1.0f}, {0.0f, 0.0f, 0.0f},
		{-3.0f, -2.0f, 1.0f}, {0.0f, 0.0f, 0.0f},
		{-3.0f, 3.0f, 3.3f}, {0.0f, 0.0f, 0.0f},
		{0.0f, 3.3f, 0.7f}, {0.0f, 0.0f, 0.0f},
		{2.0f, 3.5f, 0.2f}, {0.0f, 0.0f, 0.0f},
		{3.5f, 3.4f, 1.7f}, {0.0f, 0.0f, 0.0f},
		{2.0f, 3.3f, 3.2f}, {0.0f, 0.0f, 0.0f},
		{0.5f, 3.2f, 1.7f}, {0.0f, 0.0f, 0.0f},
		{2.0f, 3.1f, 0.3f}, {1.0f, 0.0f, 0.0f},
		{4.0f, 2.0f, 0.3f}, {0.0f, 0.0f, 0.0f},
		{3.0f, 0.0f, 2.0f}, {0.0f, 0.0f, 0.0f},
		{0.0f, -1.0f, 3.0f}, {0.0f, 0.0f, 0.0f},
		{-3.0f, -3.0f, 1.0f}, {0.0f, 0.0f, 0.0f},
		{0.0f, -5.0f, 0.2f}, {4.0f, -0.5f, 0.0f},
		{5.0f, -4.0f, 0.2f}, {-0.5f, 1.0f, 0.0f}
	};
	iRollerCoaster.trkd.nbControlPoint = 33;
	iRollerCoaster.trkd.control = (point*)array;
	iRollerCoaster.trkd.startSegment = 2;
	iRollerCoaster.trkd.brakeSegment = -12;
	iRollerCoaster.trkd.twistFactor = 1.5f;
	iRollerCoaster.trkd.averageSegmentLength = 0.35f;

	iStartTime=UniversalTime();
	iRollerCoaster.eng.doIntro = 1;
	iRollerCoaster.eng.wndWidth = WIDTH;
	iRollerCoaster.eng.wndHeight = HEIGHT;

	static const point coord[] = {
		{0.98f, -3.05f, 1.0f}, {1.0f, 0.0f, 0.0f},
		{0.0f, -3.0f, 1.0f}, {1.0f, 0.0f, 0.0f},
		{0.25f, -3.4f, 1.0f}, {1.0f, 0.0f, 0.0f},
		{0.5f, -3.8f, 1.0f}, {1.0f, 0.0f, 0.0f},
		{4.44f, -2.88f, 0.0f}, {-0.5f, 1.0f, -0.18f},
		{2.0f, 3.5f, 1.0f}, {1.0f, 0.0f, 0.0f},
		{3.4f, 3.4f, 0.0f}, {1.0f, 0.0f, 0.0f},
		{1.0f, 3.05f, 2.0f}, {1.0f, 0.0f, 0.0f},
		{2.5f, 3.05f, 1.0f}, {1.0f, 0.0f, 0.0f}
	};
	iRollerCoaster.supd.pillar_coord = (point*)coord;
	iRollerCoaster.supd.nbPillarCoord = 9;
	static const int absc[] = {
		9,
		15,
		22,
		30,
		40,
		50,
		60,
		115, //after double loop
		120,
		135,
		140,
		150,
		155,
		165,
		180,
		252,
		258,
		265,
		275,
		280,
		285,
		290,
		295
	};
	iRollerCoaster.supd.pillar_absc = (int*)absc;
	iRollerCoaster.supd.nbPillarAbsc = 23;

	if(argc==2)
		InitializeRoller(&iRollerCoaster, argv[1]);
	else
		InitializeRoller(&iRollerCoaster, "");

#ifndef __x86_64__
	{
	int	key=0;
	struct	termios oldt, newt;
	tcgetattr (STDIN_FILENO, &oldt);
	newt = oldt;
	newt.c_lflag &= ~( ICANON | ECHO );
	newt.c_cc[VTIME] = 0;
	newt.c_cc[VMIN] = 1;
	tcsetattr (STDIN_FILENO, TCSANOW, &newt);

	while( (key=GetKey(0)) != ESCAPE )
		{
		KeyPressed(key);

		DrawGL();
		}

	tcsetattr (STDIN_FILENO, TCSANOW, &oldt);
	}
#else
	glutDisplayFunc(&DrawGL);
	glutIdleFunc(&DrawGL);
	glutKeyboardFunc(&KeyPressed);
	glutMainLoop();
#endif

	ExitRoller(&iRollerCoaster);

#ifndef __x86_64__
	eglMakeCurrent(D, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
	eglDestroySurface(D, S);
	eglDestroyContext(D, X);
	eglTerminate(D);
#endif
	return(0);
}

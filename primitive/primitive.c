/*
 * OpenGL ES Primitive sandbox by CyrIng
 * 2013-0.0.2
 * GPL2
 */
#include	<stdio.h>
#include	<string.h>
#include	<unistd.h>
#include	<termios.h>
#include	<sys/select.h>
#ifndef __x86_64__
#include	"bcm_host.h"
#include	"GLES/gl.h"
#include	"EGL/egl.h"
#include	"EGL/eglext.h"
#else
#include	<X11/Xlib.h>
#include	<GL/glut.h>
#endif

#define	ENTER	10
#define	ESCAPE	27

typedef	struct
{
	GLfloat	x,
		y,
		z;
}	V3D;

typedef	struct
{
	GLfloat	Rgb,
		rGb,
		rgB,
		alpha;
}	RGB;

#define	a	{-0.5f,+0.5f,-0.5f}
#define	b	{+0.5f,+0.5f,-0.5f}
#define	c	{+0.5f,-0.5f,-0.5f}
#define	d	{-0.5f,-0.5f,-0.5f}
#define	e	{-0.5f,+0.5f,+0.5f}
#define	f	{+0.5f,+0.5f,+0.5f}
#define	g	{+0.5f,-0.5f,+0.5f}
#define	h	{-0.5f,-0.5f,+0.5f}
/*
[F1]            square       GL_TRIANGLES    GL_TRIANGLES

                d-----c      1-----2               1
                | \ / |  =   |   /      +        / |
                | / \ |      | /               /   |
                a-----b      3               2-----3
*/
const	V3D	squareVertices[]=
	{
		d, c,
		a, b
	};
const	RGB	squareColors[]=
	{
		{1.0f, 0.0f, 0.0f, 1.0f},
		{0.6f, 0.0f, 0.0f, 1.0f},
		{0.3f, 0.0f, 0.0f, 1.0f},
		{0.1f, 0.0f, 0.0f, 1.0f}
	};

void	SquareGL(void)
{
	glDrawArrays(GL_TRIANGLES, 0, 3);
	glDrawArrays(GL_TRIANGLES, 1, 3);
}

/*
[F2]            face       GL_TRIANGLE_STRIP

                d-----c         1-----2
                |     |     =       /
                |     |           /
                a-----b         3-----4
*/
const	V3D	faceVertices[]=
	{
		d, c,
		a, b
	};
const	RGB	faceColors[]=
	{
		{1.0f, 0.0f, 0.0f, 1.0f},
		{1.0f, 1.0f, 1.0f, 1.0f},
		{1.0f, 0.0f, 0.0f, 1.0f},
		{0.0f, 1.0f, 1.0f, 1.0f}
	};

void	FaceGL(void)
{
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}

/*
[F3]            box   =       GL_TRIANGLE_STRIP

                  h-----g         5-----6         2     6         1-----2
                 /|    /|         |     |        /|    /|        /     /
                d-----c |   =   1-----2 |   +   1 |   5 |   +   3-----4
                | e---|-f       | 7---|-8       | 4   | 8         5-----6
                |/    |/        |     |         |/    |/         /     /
                a-----b         3-----4         3     7         7-----8
*/
const	V3D	boxVertices[]=
	{
		d, c,	// 0 1
		a, b,	// 2 3
		h, g,	// 4 5
		e, f	// 6 7
	};
const	RGB	boxColors[]=
	{
		{1.0f, 0.0f, 0.0f, 1.0f},
		{1.0f, 1.0f, 1.0f, 1.0f},
		{1.0f, 0.0f, 0.0f, 1.0f},
		{1.0f, 1.0f, 1.0f, 1.0f},

		{1.0f, 0.0f, 0.0f, 1.0f},
		{1.0f, 1.0f, 1.0f, 1.0f},
		{1.0f, 0.0f, 0.0f, 1.0f},
		{0.0f, 1.0f, 1.0f, 1.0f}
	};

void	BoxGL(void)
{
	GLubyte indices[]=
	{
		0, 1, 2, 1, 2, 3,	// d c a | c a b
		4, 5, 6, 5, 6, 7,	// h g e | g e f
		0, 4, 2, 4, 2, 6,	// d h a | h a e
		1, 5, 3, 5, 3, 7,	// c g b | g b f
		4, 5, 0, 5, 0, 1,	// h g d | g d c
		6, 7, 2, 7, 2, 3	// e f a | f a b
	};
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_BYTE, indices);
}

/*
[F4]            cube

                  h-----g                     2       2-----3     2
                 /|    /|                    /|      /     /     /|
                d-----c |   =   1-----2  +  1 |  +  1     4  +  1 |  +
                | e---|-f             |       3                   3      3-----4
                |/    |/              |      /                   /      /
                a-----b         4-----3     4                   4      2-----1
*/

void	CubeGL(void)
{
	GLubyte indices[]=
	{
		0, 1, 3, 2,	// d c b a
		0, 4, 6, 2,	// d h e a
		0, 4, 5, 1,	// d h g c
		1, 5, 7, 3,	// c g f b
		3, 2, 6, 7	// b a e f
	};
	glDrawElements(GL_LINE_STRIP, 20, GL_UNSIGNED_BYTE, indices);
}

const	struct
{
	const	V3D	*vertices;
		void	(*callBack)(void);
	const	RGB	*colors;
} Primitive[] = {
			{ squareVertices, SquareGL, squareColors },
			{ faceVertices  , FaceGL  , faceColors   },
			{ boxVertices  ,  BoxGL   , boxColors   },
			{ boxVertices  ,  CubeGL  , boxColors   }
		};
int	P=3;

void	AxisGL(GLfloat xc, GLfloat yc, GLfloat zc)
{
	const	V3D	axisVertices[]=
	{
		{xc-0.1f, yc+0.0f, zc+0.0f}, {xc+0.1f, yc+0.0f, zc+0.0f},
		{xc+0.0f, yc-0.1f, zc+0.0f}, {xc+0.0f, yc+0.1f, zc+0.0f},
		{xc+0.0f, yc+0.0f, zc-0.1f}, {xc+0.0f, yc+0.0f, zc+0.1f}
	};
	const	RGB	axisColors[]=
	{
		{1.0f, 0.0f, 0.0f, 1.0f}, {1.0f, 0.0f, 0.0f, 1.0f},
		{0.0f, 1.0f, 0.0f, 1.0f}, {0.0f, 1.0f, 0.0f, 1.0f},
		{0.0f, 1.0f, 1.0f, 1.0f}, {0.0f, 1.0f, 1.0f, 1.0f}
	};
	glVertexPointer(3, GL_FLOAT, 0, axisVertices);
	glColorPointer( 4, GL_FLOAT, 0, axisColors);
	glDrawArrays(GL_LINES, 0, 6);
}

GLfloat	xr=1.0f,
	yr=1.0f,
	zr=0.0f;
GLfloat	trans=0.0f;
GLfloat	step=+1.0f;

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

#ifdef __x86_64__
void	ReSizeGL(int Width, int Height)
{
	glViewport(0, 0, (GLsizei) Width, (GLsizei) Height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
}
#endif

void	DrawGL(void)
{
	glClear(GL_COLOR_BUFFER_BIT);
	glTranslatef(0.0f, 0.0f, trans);
	glRotatef(xr,1.0f, 0.0f, 0.0f);
	glRotatef(yr,0.0f, 1.0f, 0.0f);
	glRotatef(zr,0.0f, 0.0f, 1.0f);
	glVertexPointer(3, GL_FLOAT, 0, Primitive[P].vertices);
	glColorPointer( 4, GL_FLOAT, 0, Primitive[P].colors);
	(* Primitive[P].callBack)();
	AxisGL(-0.9f, -0.9f, +0.0f);
	glFlush();
#ifndef __x86_64__
	eglSwapBuffers(D, S);
#else
	glutSwapBuffers();
#endif
}

void	LoadGL(void)
{
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);
}

void	StartGL(void)
{
	glClearColor(0.0f, 0.0f, 0.5f, 0.0f);
	glViewport(0, 0, (GLsizei) WIDTH, (GLsizei) HEIGHT);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
}

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
		case '+':
			step+=0.1f;
		break;
		case '-':
			step-=0.1f;
		break;
		case '1':
		case '2':
		case '3':
		case '4':
			P=key-'1';
			glDisableClientState(GL_VERTEX_ARRAY);
			glDisableClientState(GL_COLOR_ARRAY);
			LoadGL();
		break;
		case 'r':
		case 'R':
		{
			P=0;
			xr=0.0f;
			yr=0.0f;
			zr=0.0f;
			trans=0.0f;
			step=+1.0f;
			glDisableClientState(GL_VERTEX_ARRAY);
			glDisableClientState(GL_COLOR_ARRAY);
			LoadGL();
		}
		break;
		case 'o':
		case 'O':
			zr-=step;
		break;
		case 'l':
		case 'L':
			zr+=step;
		break;
		case 'z':
		case 'Z':
			xr-=step;
		break;
		case 's':
		case 'S':
			xr+=step;
		break;
		case 'q':
		case 'Q':
			yr-=step;
		break;
		case 'd':
		case 'D':
			yr+=step;
		break;
		case 'k':
		case 'K':
			trans+=0.01f;
		break;
		case 'm':
		case 'M':
			trans-=0.01f;
		break;
	}
}

#ifndef __x86_64__
void	SpecialKeyPressed(int key)
{
}
#else
void SpecialKeyPressed(int key, int x, int y)
{
	switch (key)
	{
	case GLUT_KEY_PAGE_UP:
		zr-=step;
	break;
	case GLUT_KEY_PAGE_DOWN:
		zr+=step;
	break;
	case GLUT_KEY_UP:
		xr-=step;
	break;
	case GLUT_KEY_DOWN:
		xr+=step;
	break;
	case GLUT_KEY_LEFT:
		yr-=step;
	break;
	case GLUT_KEY_RIGHT:
		yr+=step;
	break;
	case GLUT_KEY_HOME:
		trans+=0.01f;
	break;
	case GLUT_KEY_END:
		trans-=0.01f;
	break;
	case GLUT_KEY_F1:
	case GLUT_KEY_F2:
	case GLUT_KEY_F3:
	case GLUT_KEY_F4:
		P=key-GLUT_KEY_F1;
		glDisableClientState(GL_VERTEX_ARRAY);
		glDisableClientState(GL_COLOR_ARRAY);
		LoadGL();
	break;
	}
}
#endif

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
	W=glutCreateWindow("Primitive OpenGL ES");
	glutDisplayFunc(&DrawGL);
	glutIdleFunc(&DrawGL);
	glutReshapeFunc(&ReSizeGL);
	glutKeyboardFunc(&KeyPressed);
	glutSpecialFunc(&SpecialKeyPressed);
}
#endif

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

void    LoopGL()
{
	int	key=0;
	struct	termios oldt, newt;
	tcgetattr (STDIN_FILENO, &oldt);
	newt = oldt;
	newt.c_lflag &= ~( ICANON | ECHO );
	newt.c_cc[VTIME] = 0;
	newt.c_cc[VMIN] = 1;
	tcsetattr (STDIN_FILENO, TCSANOW, &newt);

	DrawGL();
	while( (key=GetKey(0)) != ESCAPE )
	{
		KeyPressed(key);
		SpecialKeyPressed(key);
		DrawGL();
	}
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);

	eglMakeCurrent(D, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
	eglDestroySurface(D, S);
	eglDestroyContext(D, X);
	eglTerminate(D);

	tcsetattr (STDIN_FILENO, TCSANOW, &oldt);
}
#else
void	LoopGL()
{
	glutMainLoop();
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
	StartGL();
	LoadGL();
        LoopGL();
	return(0);
}

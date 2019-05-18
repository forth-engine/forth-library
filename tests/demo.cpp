
// #include <iostream>

// #ifdef _WIN32
// #define WIN32_LEAN_AND_MEAN
// #include <Windows.h>
// #endif

#include "../freeglut/freeglut.h"
#include "../source/forth.h"


// int main( int argc, char** argv )
// {
// 	std::cout << "Hello world";

// // Starting width / height of the window
// 	const unsigned int kWindowWidth = 1000;
// 	const unsigned int kWindowHeight = 600;

// 	// Initialize GLUT
// 	glutInit( &argc, argv );


// 	// Get how big our screen is that we're displaying the window on
// 	int screenWidth = glutGet( GLUT_SCREEN_WIDTH );
// 	int screenHeight = glutGet( GLUT_SCREEN_HEIGHT );

// 	// Initialize the display mode to utilize double buffering, 4-channel framebuffer and depth buffer
// 	glutInitDisplayMode( GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH | GLUT_STENCIL );

// 	// Setup the window
// 	glutInitWindowSize( kWindowWidth, kWindowHeight );
// 	glutInitWindowPosition( (screenWidth - kWindowWidth) / 2, (screenHeight - kWindowHeight) / 2 );
// 	glutCreateWindow( "Demo" );

// 	// glutDisplayFunc( DisplayLoop );
// 	// glutReshapeFunc( Reshape );
// 	// glutKeyboardUpFunc( KeyboardUp );
// 	// glutKeyboardFunc( Keyboard );
// 	// glutMouseFunc( Mouse );
// 	// glutMotionFunc( MouseMotion );
// 	// glutPassiveMotionFunc( MouseMotion );
// 	// glutIdleFunc( MainLoop );

// 	// Setup all the open-gl states we want to use (ones that don't change in the lifetime of the application)
// 	// Note: These can be changed anywhere, but generally we don't change the back buffer color
// 	glClearColor( 0.0f, 0.0f, 0.0f, 0.0f );
// 	glEnable( GL_CULL_FACE );
// 	glEnable( GL_DEPTH_TEST );
// 	glCullFace( GL_BACK );
// 	glFrontFace( GL_CCW );
// 	glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
// 	glEnable( GL_BLEND );

// 	// Show the window that we just initailized
// 	glutShowWindow( );

// 	std::cin.get();

// }



/* Copyright (c) Mark J. Kilgard, 1997. */

/* This program is freely distributable without licensing fees
   and is provided without guarantee or warrantee expressed or
   implied. This program is -not- in the public domain. */

/* This program was requested by Patrick Earl; hopefully someone else
   will write the equivalent Direct3D immediate mode program. */

// #include <GL/glut.h>

GLfloat light_diffuse[] = {1.0, 0.0, 0.0, 1.0};  /* Red diffuse light. */
GLfloat light_position[] = {1.0, 1.0, 1.0, 0.0};  /* Infinite light location. */
GLfloat n[6][3] = {  /* Normals for the 6 faces of a cube. */
  {-1.0, 0.0, 0.0}, {0.0, 1.0, 0.0}, {1.0, 0.0, 0.0},
  {0.0, -1.0, 0.0}, {0.0, 0.0, 1.0}, {0.0, 0.0, -1.0} };
GLint faces[6][4] = {  /* Vertex indices for the 6 faces of a cube. */
  {0, 1, 2, 3}, {3, 2, 6, 7}, {7, 6, 5, 4},
  {4, 5, 1, 0}, {5, 6, 2, 1}, {7, 4, 0, 3} };
GLfloat v[8][3];  /* Will be filled in with X,Y,Z vertexes. */

void
drawBox(void)
{
  int i;

  for (i = 0; i < 6; i++) {
    glBegin(GL_QUADS);
    glNormal3fv(&n[i][0]);
    glVertex3fv(&v[faces[i][0]][0]);
    glVertex3fv(&v[faces[i][1]][0]);
    glVertex3fv(&v[faces[i][2]][0]);
    glVertex3fv(&v[faces[i][3]][0]);
    glEnd();
  }
}



void
display(void)
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  drawBox();
  glutSwapBuffers();
}

void
init(void)
{
  /* Setup cube vertex data. */
  v[0][0] = v[1][0] = v[2][0] = v[3][0] = -1;
  v[4][0] = v[5][0] = v[6][0] = v[7][0] = 1;
  v[0][1] = v[1][1] = v[4][1] = v[5][1] = -1;
  v[2][1] = v[3][1] = v[6][1] = v[7][1] = 1;
  v[0][2] = v[3][2] = v[4][2] = v[7][2] = 1;
  v[1][2] = v[2][2] = v[5][2] = v[6][2] = -1;

  /* Enable a single OpenGL light. */
  glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
  glLightfv(GL_LIGHT0, GL_POSITION, light_position);
  glEnable(GL_LIGHT0);
  glEnable(GL_LIGHTING);

  /* Use depth buffering for hidden surface elimination. */
  glEnable(GL_DEPTH_TEST);

  /* Setup the view of the cube. */
  glMatrixMode(GL_PROJECTION);
  gluPerspective( /* field of view in degree */ 40.0,
    /* aspect ratio */ 1.0,
    /* Z near */ 1.0, /* Z far */ 10.0);
  glMatrixMode(GL_MODELVIEW);
  gluLookAt(0.0, 0.0, 5.0,  /* eye is at (0,0,5) */
    0.0, 0.0, 0.0,      /* center is at (0,0,0) */
    0.0, 1.0, 0.);      /* up is in positive Y direction */

  /* Adjust cube position to be asthetic angle. */
  glTranslatef(0.0, 0.0, -1.0);
  glRotatef(60, 1.0, 0.0, 0.0);
  glRotatef(-20, 0.0, 0.0, 1.0);
}

int
main(int argc, char **argv)
{
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
  glutCreateWindow("red 3D lighted cube");
  glutDisplayFunc(display);
  init();
  glutMainLoop();
  return 0;             /* ANSI C requires main to return int. */
}
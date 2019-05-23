
#include "../freeglut/freeglut.h"
#include "../source/forth.h"
#include <iostream>

GLfloat light_diffuse[] = {0.f, 0.4f, 1.f, 1.f};	/* Blue diffuse light. */
GLfloat light_position[] = {-1.0, -1.0, -1.0, 0.0}; /* Infinite light location. */

GLint (*faces)[3]; /* Vertex indices for the 6 faces of a cube. */
GLfloat (*normals)[3];
int face_count;
GLfloat (*v)[3]; /* Will be filled in with X,Y,Z vertexes. */

Forth::Buffer4 buff = Forth::Buffer4();
Forth::CrossSection cross = Forth::CrossSection();
Forth::SolidVisualizer solid = Forth::SolidVisualizer();
Forth::Buffer3 out = Forth::Buffer3();

void pushToGL(Forth::Buffer3 out)
{
	delete[] v, faces, normals;
	v = new float[out.vertices.size()][3];
	unsigned int i = 0, j = 0;
	for (std::vector<Forth::Vector3>::iterator it = out.vertices.begin(); it != out.vertices.end(); ++it)
	{
		v[i][0] = it->x;
		v[i][1] = it->y;
		v[i][2] = it->z;
		++i;
	}

	faces = new GLint[out.indices.size()][3];
	face_count = out.indices.size() / 3;
	for (i = 0; i < out.indices.size();)
	{
		faces[j][0] = out.indices[i++];
		faces[j][1] = out.indices[i++];
		faces[j][2] = out.indices[i++];
		++j;
	}

	normals = new GLfloat[face_count][3];
	for (int i = 0; i < face_count; i++)
	{
		Forth::Vector3 a = Forth::Vector3(v[faces[i][0]]),
					   b = Forth::Vector3(v[faces[i][1]]),
					   c = Forth::Vector3(v[faces[i][2]]),
					   cross = Forth::Normalize(Forth::Cross(c - b, a - b));

		normals[i][0] = cross[0];
		normals[i][1] = cross[1];
		normals[i][2] = cross[2];
	}
}

void drawBox(void)
{
	solid.Initialize(out);
	Forth::Transform4 r = Forth::Transform4::identity();
	float elapsed = float(glutGet(GLUT_ELAPSED_TIME)) * 90.f / 1000.f;
	r.rotation = Forth::Euler(4, elapsed) * Forth::Euler(3, elapsed);
	cross.Project(buff, r, &solid);
	solid.End();

	pushToGL(out);

	int i;

	for (i = 0; i < face_count; i++)
	{
		glBegin(GL_TRIANGLES);
		glNormal3fv(normals[i]);
		glVertex3fv(v[faces[i][0]]);
		glVertex3fv(v[faces[i][1]]);
		glVertex3fv(v[faces[i][2]]);
		glEnd();
	}
}

void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	drawBox();
	glutSwapBuffers();
}

void init(void)
{

	/* Enable a single OpenGL light. */
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHTING);

	/* Use depth buffering for hidden surface elimination. */
	glEnable(GL_DEPTH_TEST);

	/* Setup the view of the cube. */
	glMatrixMode(GL_PROJECTION);
	gluPerspective(/* field of view in degree */ 60.0,
				   /* aspect ratio */ 1.0,
				   /* Z near */ 1.0, /* Z far */ 10.0);
	glMatrixMode(GL_MODELVIEW);
	gluLookAt(0.0, 0.0, -5.0, /* eye is at (0,0,5) */
			  0.0, 0.0, 0.0,  /* center is at (0,0,0) */
			  0.0, 1.0, 0.);  /* up is in positive Y direction */

	/* Adjust cube position to be asthetic angle. */
	glTranslatef(0.0, 0.0, -1.0);
	glRotatef(60, 1.0, 0.0, 0.0);
	glRotatef(-20, 0.0, 0.0, 1.0);
}

void timer(int extra)
{
	glutPostRedisplay();
	glutTimerFunc(30, timer, 0);
}

int main(int argc, char **argv)
{
	buff.simplex = Forth::SM_Tetrahedron;
	cross.Setup(Forth::Transform4::identity());

	for (float w = -1; w <= 1; w += 2)
		for (float x = -1; x <= 1; x += 2)
			for (float y = -1; y <= 1; y += 2)
				for (float z = -1; z <= 1; z += 2)
				{
					Forth::Vector4 v = Forth::Vector4(x, y, z, w);
					buff.AddVertex(v * 1);
				}

	buff.SequenceGrid(2, 2, 2, 2);

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutCreateWindow("Forth Tesseract");
	glutDisplayFunc(display);
	init();
	glutTimerFunc(0, timer, 0);
	glutMainLoop();
	return 0; /* ANSI C requires main to return int. */
}
#include <gl\glew.h>
#include <MeGlWindow.h>
#include <iostream>

using namespace std;

extern const char* vertexShaderCode;
extern const char* fragmentShaderCode;

void definePositions()
{
	// Vertex positions on the screen
	static GLfloat vertexPos[] = {
		+0.0f, +0.0f,
		+1.0f, +0.0f, +0.0f,
		-1.0f, +1.0f,
		+0.0f, +1.0f, +0.0f,
		-1.0f, -1.0f,
		+0.0f, +0.0f, +1.0f,
		
		+0.75f, -0.75f,
		+1.0f, +0.0f, +1.0f,
		+0.75f, +0.75f,
		+0.0f, +1.0f, +1.0f,
	};

	// Order of drawing
	static GLushort indices[] = {
		0, 1, 2,
		0, 3, 4
	};

	GLuint bufferID;
	glGenBuffers(1, &bufferID);
	glBindBuffer(GL_ARRAY_BUFFER, bufferID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufferID);

	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexPos) + sizeof(indices), 0, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertexPos), vertexPos);
	glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, sizeof(vertexPos), sizeof(indices), indices);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 5, 0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 5, (char*)(sizeof(float) * 2));
}

void installShaders()
{
	GLuint vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	GLuint fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

	const char* adapter[1];
	adapter[0] = vertexShaderCode;
	glShaderSource(vertexShaderID, 1, adapter, 0);
	adapter[0] = fragmentShaderCode;
	glShaderSource(fragmentShaderID, 1, adapter, 0);

	glCompileShader(vertexShaderID);
	glCompileShader(fragmentShaderID);

	GLuint programID = glCreateProgram();
	glAttachShader(programID, vertexShaderID);
	glAttachShader(programID, fragmentShaderID);

	glLinkProgram(programID);
	glUseProgram(programID);
}

void MeGlWindow::initializeGL()
{
	glewInit();
	//glClearColor(0, 0, 0, 1);
	definePositions();
	installShaders();
}

void MeGlWindow::paintGL()
{
	glClear(GL_COLOR_BUFFER_BIT);

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, (void*)100);
}




/*
void MeGlWindow::keyboard(unsigned char key, int x, int y)
{

	switch (key) {
	case 27:
		glutDestroyWindow(window);
		exit(0);
	case 'x':
		anglex = (anglex + 3) % 360;
		break;
	case 'X':
		anglex = (anglex - 3) % 360;
		break;
	case 'y':
		angley = (angley + 3) % 360;
		break;
	case 'Y':
		angley = (angley - 3) % 360;
		break;
	case 'z':
		anglez = (anglez + 3) % 360;
		break;
	case 'Z':
		anglez = (anglez - 3) % 360;
		break;
	case 'r':                                   //reset
		anglez = angley = anglex = 0;
		break;
	}
	glutPostRedisplay();
}
*/
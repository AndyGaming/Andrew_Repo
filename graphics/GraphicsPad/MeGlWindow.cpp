#include <gl\glew.h>
#include <glm\glm.hpp>
#include <glm\gtx\transform.hpp>
#include <MeGlWindow.h>
#include <Vertex.h>
#include <ShapeGenerator.h>
#include <iostream>
#include <fstream>

using namespace std;
using glm::vec3;
using glm::mat4;

const uint NUM_VERTICES_PER_TRI = 3;
const uint NUM_FLOATS_PER_VERTICE = 6;
const uint VERTEX_BYTE_SIZE = NUM_FLOATS_PER_VERTICE * sizeof(float);
GLuint programID;
GLuint indices;

vec3 position;
float angle = 0.0f;
float scaling = 1.0f;
bool isBig = true;

string readShader(const char* fileName)
{
	ifstream input(fileName);
	if (!input.good())
	{
		cout << "File load failed..." << fileName << endl;
		exit(1);
	}

	return std::string(
		std::istreambuf_iterator<char>(input),
		std::istreambuf_iterator<char>()
	);
}

void installShaders()
{
	GLuint vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	GLuint fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

	const GLchar* adapter[1];
	string temp = readShader("VertexShader.glsl");
	adapter[0] = temp.c_str();
	glShaderSource(vertexShaderID, 1, adapter, 0);
	temp = readShader("FragmentShader.glsl");
	adapter[0] = temp.c_str();
	glShaderSource(fragmentShaderID, 1, adapter, 0);

	glCompileShader(vertexShaderID);
	glCompileShader(fragmentShaderID);

	programID = glCreateProgram();
	glAttachShader(programID, vertexShaderID);
	glAttachShader(programID, fragmentShaderID);

	glLinkProgram(programID);
	glUseProgram(programID);
}

void sendContent()
{
	//// Vertex positions on the screen
	//Vertex triPos[] = {
	//	glm::vec3(+0.0f, +1.0f, +1.0f),
	//	glm::vec3(+1.0f, +0.0f, +0.0f),

	//	glm::vec3(-1.0f, -1.0f, +1.0f),
	//	glm::vec3(+0.0f, +1.0f, +0.0f),
	//	
	//	glm::vec3(+1.0f, -1.0, +1.0f),
	//	glm::vec3(+0.0f, +0.0f, +1.0f),
	//};

	//// Order of drawing
	//static GLushort indices[] = {
	//	0, 1, 2,
	//};

	ShapeData cube = ShapeGenerator::makeCube();
	
	GLuint vertexBufferID;
	glGenBuffers(1, &vertexBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID);
	glBufferData(GL_ARRAY_BUFFER, cube.vertexBufferSize(), cube.vertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, 0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, (char*)(sizeof(float) * 3));

	GLuint indexArrayBufferID;
	glGenBuffers(1, &indexArrayBufferID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexArrayBufferID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, cube.indexBufferSize(), cube.indices, GL_STATIC_DRAW);

	//GLuint bufferID;
	//glGenBuffers(1, &bufferID);
	//glBindBuffer(GL_ARRAY_BUFFER, bufferID);
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufferID);

	//glBufferData(GL_ARRAY_BUFFER, cube.vertexBufferSize() + cube.indexBufferSize(), cube.vertices, GL_STATIC_DRAW);
	//glBufferSubData(GL_ARRAY_BUFFER, 0, cube.vertexBufferSize(), cube.vertices);
	//glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, cube.vertexBufferSize(), cube.indexBufferSize(), cube.indices);

	//glEnableVertexAttribArray(0);
	//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, 0);
	//glEnableVertexAttribArray(1);
	//glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, (char*)(sizeof(float) * 3));

	indices = cube.numIndices;

	cube.cleanup();
}

void transformShape()
{	
	mat4 rotate = glm::rotate(angle, 0.0f, 0.0f, 1.0f);
	mat4 translate = glm::translate(position);
	mat4 scale = glm::scale(mat4(1.0f), vec3(scaling));

	mat4 transform = rotate * translate * scale;
	GLint uniformLoc = glGetUniformLocation(programID, "transform");
	glUniformMatrix4fv(uniformLoc, 1, GL_FALSE, &transform[0][0]);
}

void MeGlWindow::initializeGL()
{
	glewInit();
	glEnable(GL_DEPTH_TEST);
	installShaders();
	sendContent();
}

void MeGlWindow::paintGL()
{
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	glViewport(0, 0, width(), height());

	mat4 modelTransformMatrix = glm::translate(mat4(), vec3(0.0f, 0.0f, -3.0f));
	mat4 projectionMatrix = glm::perspective(60.0f, ((float)width()) / height(), 0.1f, 10.0f);

	GLint modelTransMatUniformLoc = glGetUniformLocation(programID, "modelTransMat");
	GLint projectionMatUniformLoc = glGetUniformLocation(programID, "projectionMat");

	glUniformMatrix4fv(modelTransMatUniformLoc, 1, GL_FALSE, &modelTransformMatrix[0][0]);
	glUniformMatrix4fv(projectionMatUniformLoc, 1, GL_FALSE, &projectionMatrix[0][0]);

	transformShape();

	// (void*) number of vertex position floats * 4
	glDrawElements(GL_TRIANGLES, indices, GL_UNSIGNED_SHORT, 0);
}

void MeGlWindow::keyPressEvent(QKeyEvent* e)
{
	switch (e->key())
	{
	case Qt::Key::Key_Escape:
		exit(0);
		break;
	case Qt::Key::Key_W:
		position.y += 0.05f;
		break;
	case Qt::Key::Key_S:
		position.y -= 0.05f;
		break;
	case Qt::Key::Key_A:
		position.x -= 0.05f;
		break;
	case Qt::Key::Key_D:
		position.x += 0.05f;
		break;
	case Qt::Key::Key_Q:
		angle += 3.6f;
		break;
	case Qt::Key::Key_E:
		angle -= 3.6f;
		break;
	case Qt::Key::Key_F:
		if (isBig)
		{
			scaling -= 0.0625f;
			if (scaling <= 0.0f)
			{
				isBig = false;
			}
		}
		else
		{
			scaling += 0.0625f;
			if (scaling >= 2.0f)
			{
				isBig = true;
			}
		}
		break;
	case Qt::Key::Key_R:
		position.x = position.y = angle = 0.0f;
		scaling = 1.0f;
	}
	repaint();
}

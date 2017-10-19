#include <gl\glew.h>
#include <glm\glm.hpp>
#include <glm\gtx\transform.hpp>
#include <QtGui\qmouseevent>
#include <MeGlWindow.h>
#include <Vertex.h>
#include <ShapeGenerator.h>
#include <Camera.h>
#include <iostream>
#include <fstream>

using namespace std;
using glm::vec3;
using glm::mat4;

const uint NUM_VERTICES_PER_TRI = 3;
const uint NUM_FLOATS_PER_VERTICE = 6;
const uint VERTEX_BYTE_SIZE = NUM_FLOATS_PER_VERTICE * sizeof(float);
GLuint programID;
GLuint cubeIndices;
GLuint arrowIndices;
Camera camera;
GLuint fullTransformUniLoc;
GLuint bufferID;
//GLuint indexBufferID;
GLuint cubeVertexArrayObjectID;
GLuint arrowVertexArrayObjectID;
GLuint cubeIndexByteOffset;
GLuint arrowIndexByteOffset;

vec3 position;
float dist = 70.0f;
float angle_x = 0.0f;
float angle_y = -30.0f;
float angle_z = 0.0f;
float scale = 1.0f;
bool isBig = true;

void MeGlWindow::sendContent()
{
	/*ShapeData cube = ShapeGenerator::makeCube();
	
	GLuint vertexBufferID;
	glGenBuffers(1, &vertexBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID);
	glBufferData(GL_ARRAY_BUFFER, cube.vertexBufferSize(), cube.vertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, VERTEX_BYTE_SIZE, 0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, VERTEX_BYTE_SIZE, (char*)(sizeof(float) * 3));

	GLuint indexArrayBufferID;
	glGenBuffers(1, &indexArrayBufferID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexArrayBufferID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, cube.indexBufferSize(), cube.indices, GL_STATIC_DRAW);

	indices = cube.numIndices;

	cube.cleanup();*/

	ShapeData cube = ShapeGenerator::makeCube();
	ShapeData arrow = ShapeGenerator::makeArrow();

	glGenBuffers(1, &bufferID);
	glBindBuffer(GL_ARRAY_BUFFER, bufferID);
	glBufferData(GL_ARRAY_BUFFER, 
		cube.vertexBufferSize() + cube.indexBufferSize()
		+ arrow.vertexBufferSize() + arrow.indexBufferSize(), 
		0, GL_STATIC_DRAW);
	GLsizeiptr currentOffset = 0;
	glBufferSubData(GL_ARRAY_BUFFER, currentOffset, cube.vertexBufferSize(), cube.vertices);
	currentOffset += cube.vertexBufferSize();
	glBufferSubData(GL_ARRAY_BUFFER, currentOffset, cube.indexBufferSize(), cube.indices);
	currentOffset += cube.indexBufferSize();
	glBufferSubData(GL_ARRAY_BUFFER, currentOffset, arrow.vertexBufferSize(), arrow.vertices);
	currentOffset += arrow.vertexBufferSize();
	glBufferSubData(GL_ARRAY_BUFFER, currentOffset, arrow.indexBufferSize(), arrow.indices);

	cubeIndices = cube.numIndices;
	arrowIndices = arrow.numIndices;

	glGenVertexArrays(1, &cubeVertexArrayObjectID);
	glGenVertexArrays(1, &arrowVertexArrayObjectID);

	glBindVertexArray(cubeVertexArrayObjectID);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, bufferID);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, VERTEX_BYTE_SIZE, 0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, VERTEX_BYTE_SIZE, (void*)(sizeof(float) * 3));
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufferID);

	glBindVertexArray(arrowVertexArrayObjectID);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, bufferID);
	GLuint arrowByteOffset = cube.vertexBufferSize() + cube.indexBufferSize();
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, VERTEX_BYTE_SIZE, (void*)arrowByteOffset);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, VERTEX_BYTE_SIZE, (void*)(arrowByteOffset + sizeof(float) * 3));
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufferID);

	cubeIndexByteOffset = cube.vertexBufferSize();
	arrowIndexByteOffset = arrowByteOffset + arrow.vertexBufferSize();

	cube.cleanup();
	arrow.cleanup();
}

void MeGlWindow::paintGL()
{
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	glViewport(0, 0, width(), height());

	//GLint fullTransformUniLoc = glGetUniformLocation(programID, "transform");

	//mat4 fullTransform;
	//mat4 projectionMatrix = glm::perspective(dist, ((float)width()) / height(), 0.1f, 10.0f);

	//// Cube 1
	//mat4 translationMatrix = glm::translate(mat4(), vec3(-1.0f, 0.0f, -3.0f));
	//mat4 rotationMatrix_x = glm::rotate(mat4(), angle_x, vec3(1.0f, 0.0f, 0.0f));
	//mat4 rotationMatrix_y = glm::rotate(mat4(), angle_y, vec3(0.0f, 1.0f, 0.0f));
	//mat4 rotationMatrix_z = glm::rotate(mat4(), angle_z, vec3(0.0f, 0.0f, 1.0f));
	//mat4 scaleMatrix = glm::scale(mat4(1.0f), vec3(scale));

	//fullTransform = projectionMatrix * /*camera.getWorldToViewMatrix() **/ translationMatrix * 
	//	rotationMatrix_x * rotationMatrix_y * rotationMatrix_z * scaleMatrix;

	//glUniformMatrix4fv(fullTransformUniLoc, 1, GL_FALSE, &fullTransform[0][0]);
	//glDrawElements(GL_TRIANGLES, indices, GL_UNSIGNED_SHORT, 0);

	mat4 fullTransformMatrix;
	mat4 viewToProjectionMatrix = glm::perspective(60.0f, ((float)width()) / height(), 0.1f, 10.0f);
	mat4 worldToViewMatrix = camera.getWorldToViewMatrix();
	mat4 worldToProjectionMatrix = viewToProjectionMatrix * worldToViewMatrix;

	// Cube
	glBindVertexArray(cubeVertexArrayObjectID);
	mat4 cube1ModelToWorldMatrix =
		glm::translate(vec3(-2.0f, 0.0f, -3.0f)) *
		glm::rotate(36.0f, vec3(1.0f, 0.0f, 0.0f));
	fullTransformMatrix = worldToProjectionMatrix * cube1ModelToWorldMatrix;
	glUniformMatrix4fv(fullTransformUniLoc, 1, GL_FALSE, &fullTransformMatrix[0][0]);
	glDrawElements(GL_TRIANGLES, cubeIndices, GL_UNSIGNED_SHORT, (void*)cubeIndexByteOffset);

	mat4 cube2ModelToWorldMatrix =
		glm::translate(vec3(2.0f, 0.0f, -3.75f)) *
		glm::rotate(126.0f, vec3(0.0f, 1.0f, 0.0f));
	fullTransformMatrix = worldToProjectionMatrix * cube2ModelToWorldMatrix;
	glUniformMatrix4fv(fullTransformUniLoc, 1, GL_FALSE, &fullTransformMatrix[0][0]);
	glDrawElements(GL_TRIANGLES, cubeIndices, GL_UNSIGNED_SHORT, (void*)cubeIndexByteOffset);

	// Arrow
	glBindVertexArray(arrowVertexArrayObjectID);
	mat4 arrowModelToWorldMatrix = glm::translate(0.0f, 0.0f, -3.0f);
	fullTransformMatrix = worldToProjectionMatrix * arrowModelToWorldMatrix;
	glUniformMatrix4fv(fullTransformUniLoc, 1, GL_FALSE, &fullTransformMatrix[0][0]);
	glDrawElements(GL_TRIANGLES, arrowIndices, GL_UNSIGNED_SHORT, (void*)arrowIndexByteOffset);
}

void MeGlWindow::mouseMoveEvent(QMouseEvent* e)
{
	camera.mouseUpdate(glm::vec2(e->x(), e->y()));
	repaint();
}


//void MeGlWindow::keyPressEvent(QKeyEvent* e)
//{
//	switch (e->key())
//	{
//	case Qt::Key::Key_Escape:
//		exit(0);
//		break;
//	case Qt::Key::Key_S:
//		//position.y += 0.05f;
//		if (dist <= 100.0f)
//		{
//			dist += 2.0f;
//		}
//		break;
//	case Qt::Key::Key_W:
//		//position.y -= 0.05f;
//		if (dist >= 40.0f)
//		{
//			dist += -2.0f;
//		}
//		break;
//	case Qt::Key::Key_A:
//		position.x += -0.05f;
//		break;
//	case Qt::Key::Key_D:
//		position.x += 0.05f;
//		break;
//	case Qt::Key::Key_Up:
//		angle_x += -3.6f;
//		break;
//	case Qt::Key::Key_Down:
//		angle_x += 3.6f;
//		break;
//	case Qt::Key::Key_Left:
//		angle_y += -3.6f;
//		break;
//	case Qt::Key::Key_Right:
//		angle_y += 3.6f;
//		break;
//	case Qt::Key::Key_F:
//		if (isBig)
//		{
//			scale -= 0.0625f;
//			if (scale <= 0.0f)
//			{
//				isBig = false;
//			}
//		}
//		else
//		{
//			scale += 0.0625f;
//			if (scale >= 2.0f)
//			{
//				isBig = true;
//			}
//		}
//		break;
//	case Qt::Key::Key_R:
//		dist = 70.0f;
//		position.x = position.y = angle_x = angle_z = 0.0f;
//		angle_y = -30.0f;
//		scale = 1.0f;
//	}
//	repaint();
//}

void MeGlWindow::keyPressEvent(QKeyEvent* e)
{
	switch (e->key())
	{
	case Qt::Key::Key_Escape:
		exit(0);
		break;
	case Qt::Key::Key_W:
		camera.moveForward();
		break;
	case Qt::Key::Key_S:
		camera.moveBackward();
		break;
	case Qt::Key::Key_A:
		camera.strafeLeft();
		break;
	case Qt::Key::Key_D:
		camera.strafeRight();
		break;
	case Qt::Key::Key_R:
		camera.moveUp();
		break;
	case Qt::Key::Key_F:
		camera.moveDown();
		break;
	}
	repaint();
}

string MeGlWindow::readShader(const char* fileName)
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

void MeGlWindow::installShaders()
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

void MeGlWindow::initializeGL()
{
	setMouseTracking(false);
	glewInit();
	glEnable(GL_DEPTH_TEST);
	installShaders();
	sendContent();
}

MeGlWindow::~MeGlWindow()
{
	glDeleteBuffers(1, &bufferID);
	glDeleteVertexArrays(1, &cubeVertexArrayObjectID);
	glDeleteVertexArrays(1, &arrowVertexArrayObjectID);
	glUseProgram(0);
	glDeleteProgram(programID);
}
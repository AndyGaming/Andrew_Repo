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
using glm::vec4;
using glm::mat4;

const uint NUM_VERTICES_PER_TRI = 3;
const uint NUM_FLOATS_PER_VERTICE = 14;
const uint VERTEX_BYTE_SIZE = NUM_FLOATS_PER_VERTICE * sizeof(float);

Camera camera;
GLuint programID;
GLuint bufferID;
GLuint textureID;
GLuint fullTransformUniLoc;

GLuint cubeIndices;
GLuint cubeVertexArrayObjectID;
GLuint cubeIndexByteOffset;

//GLuint arrowIndices;
//GLuint arrowVertexArrayObjectID;
//GLuint arrowIndexByteOffset;

//GLuint sphereIndices;
//GLuint sphereVertexArrayObjectID;
//GLuint sphereIndexByteOffset;

GLuint planeIndices;
GLuint planeVertexArrayObjectID;
GLuint planeIndexByteOffset;

float angle_x = 0.0f;
float angle_y = -30.0f;
float angle_z = 0.0f;
float lightPos_x = -2.0f;
float lightPos_y = 4.0f;

void MeGlWindow::sendContent()
{
	ShapeData cube = ShapeGenerator::makeCube();
	//ShapeData arrow = ShapeGenerator::makeArrow();
	//ShapeData sphere = ShapeGenerator::makeSphere();
	ShapeData plane = ShapeGenerator::makePlane();

	glGenBuffers(1, &bufferID);
	glBindBuffer(GL_ARRAY_BUFFER, bufferID);
	glBufferData(GL_ARRAY_BUFFER, 
		cube.vertexBufferSize() + cube.indexBufferSize()
		//+ arrow.vertexBufferSize() + arrow.indexBufferSize()
		//+ sphere.vertexBufferSize() + sphere.indexBufferSize()
		+ plane.vertexBufferSize() + plane.indexBufferSize(),
		0, GL_STATIC_DRAW);
	GLsizeiptr currentOffset = 0;
	glBufferSubData(GL_ARRAY_BUFFER, currentOffset, cube.vertexBufferSize(), cube.vertices);
	currentOffset += cube.vertexBufferSize();
	glBufferSubData(GL_ARRAY_BUFFER, currentOffset, cube.indexBufferSize(), cube.indices);
	currentOffset += cube.indexBufferSize();
	//glBufferSubData(GL_ARRAY_BUFFER, currentOffset, arrow.vertexBufferSize(), arrow.vertices);
	//currentOffset += arrow.vertexBufferSize();
	//glBufferSubData(GL_ARRAY_BUFFER, currentOffset, arrow.indexBufferSize(), arrow.indices);
	//currentOffset += arrow.indexBufferSize();
	//glBufferSubData(GL_ARRAY_BUFFER, currentOffset, sphere.vertexBufferSize(), sphere.vertices);
	//currentOffset += sphere.vertexBufferSize();
	//glBufferSubData(GL_ARRAY_BUFFER, currentOffset, sphere.indexBufferSize(), sphere.indices);
	//currentOffset += sphere.indexBufferSize();
	glBufferSubData(GL_ARRAY_BUFFER, currentOffset, plane.vertexBufferSize(), plane.vertices);
	currentOffset += plane.vertexBufferSize();
	glBufferSubData(GL_ARRAY_BUFFER, currentOffset, plane.indexBufferSize(), plane.indices);

	cubeIndices = cube.numIndices;
	//arrowIndices = arrow.numIndices;
	//sphereIndices = sphere.numIndices;
	planeIndices = plane.numIndices;

	glGenVertexArrays(1, &cubeVertexArrayObjectID);
	//glGenVertexArrays(1, &arrowVertexArrayObjectID);
	//glGenVertexArrays(1, &sphereVertexArrayObjectID);
	glGenVertexArrays(1, &planeVertexArrayObjectID);

	glBindVertexArray(cubeVertexArrayObjectID);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glEnableVertexAttribArray(3);
	glEnableVertexAttribArray(4);
	glBindBuffer(GL_ARRAY_BUFFER, bufferID);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, VERTEX_BYTE_SIZE, 0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, VERTEX_BYTE_SIZE, (void*)(sizeof(float) * 3));
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, VERTEX_BYTE_SIZE, (void*)(sizeof(float) * 6));
	glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, VERTEX_BYTE_SIZE, (void*)(sizeof(float) * 9));
	glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, VERTEX_BYTE_SIZE, (void*)(sizeof(float) * 11));
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufferID);

	glBindVertexArray(planeVertexArrayObjectID);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, bufferID);
	GLuint planeByteOffset = cube.vertexBufferSize() + cube.indexBufferSize();
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, VERTEX_BYTE_SIZE, (void*)planeByteOffset);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, VERTEX_BYTE_SIZE, (void*)(planeByteOffset + sizeof(float) * 3));
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, VERTEX_BYTE_SIZE, (void*)(planeByteOffset + sizeof(float) * 6));
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufferID);

	cubeIndexByteOffset = cube.vertexBufferSize();
	//arrowIndexByteOffset = arrowByteOffset + arrow.vertexBufferSize();
	//sphereIndexByteOffset = sphereByteOffset + sphere.vertexBufferSize();
	planeIndexByteOffset = planeByteOffset + plane.vertexBufferSize();

	cube.cleanup();
	//arrow.cleanup();
	//sphere.cleanup();
	plane.cleanup();
}

void MeGlWindow::paintGL()
{
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	glViewport(0, 0, width(), height());
	
	mat4 modelToProjectionMat;
	mat4 viewToProjectionMatrix = glm::perspective(60.0f, ((float)width()) / height(), 0.1f, 20.0f);
	mat4 worldToViewMatrix = camera.getWorldToViewMatrix();
	mat4 worldToProjectionMatrix = viewToProjectionMatrix * worldToViewMatrix;

	GLuint modelToWorldMatUniLoc = glGetUniformLocation(programID, "modelToWorldMat");

	// Ambient light
	GLuint ambientLightUniLoc = glGetUniformLocation(programID, "ambientLight");
	vec4 ambientLight(0.1f, 0.1f, 0.1f, 1.0f);
	glUniform4fv(ambientLightUniLoc, 1, &ambientLight[0]);

	// Diffuse light
	GLuint lightPositionWorldUniLoc = glGetUniformLocation(programID, "lightPositionWorld");
	vec3 lightPositionWorld(lightPos_x, lightPos_y, -3.5f);
	glUniform3fv(lightPositionWorldUniLoc, 1, &lightPositionWorld[0]);

	// Specular light
	GLuint cameraPositionWorldUniLoc = glGetUniformLocation(programID, "cameraPositionWorld");
	vec3 cameraPosition = camera.getPosition();
	glUniform3fv(cameraPositionWorldUniLoc, 1, &cameraPosition[0]);

	// Cube
	glBindVertexArray(cubeVertexArrayObjectID);
	mat4 cubeTranslateMatrix = glm::translate(vec3(0.0f, 1.0f, -3.0f));
	mat4 cubeRotaionMatrix_x = glm::rotate(mat4(), angle_x, vec3(1.0f, 0.0f, 0.0f));
	mat4 cubeRotaionMatrix_y = glm::rotate(mat4(), angle_y, vec3(0.0f, 1.0f, 0.0f));
	mat4 cubeModelToWorldMatrix = cubeTranslateMatrix * cubeRotaionMatrix_x * cubeRotaionMatrix_y;
	//	glm::translate(vec3(-2.0f, 1.0f, 1.0f))
	//	* glm::rotate(60.0f, vec3(0.0f, 1.0f, 0.0f));
	modelToProjectionMat = worldToProjectionMatrix * cubeModelToWorldMatrix;
	glUniformMatrix4fv(fullTransformUniLoc, 1, GL_FALSE, &modelToProjectionMat[0][0]);
	glUniformMatrix4fv(modelToWorldMatUniLoc, 1, GL_FALSE, &cubeModelToWorldMatrix[0][0]);
	glDrawElements(GL_TRIANGLES, cubeIndices, GL_UNSIGNED_SHORT, (void*)cubeIndexByteOffset);

	// Arrow
	//glBindVertexArray(arrowVertexArrayObjectID);
	//mat4 arrowModelToWorldMatrix = glm::translate(0.0f, 0.5f, -3.0f);
	//modelToProjectionMat = worldToProjectionMatrix * arrowModelToWorldMatrix;
	//glUniformMatrix4fv(fullTransformUniLoc, 1, GL_FALSE, &modelToProjectionMat[0][0]);
	//glUniformMatrix4fv(modelToWorldMatUniLoc, 1, GL_FALSE, &arrowModelToWorldMatrix[0][0]);
	//glDrawElements(GL_TRIANGLES, arrowIndices, GL_UNSIGNED_SHORT, (void*)arrowIndexByteOffset);

	// Sphere
	//glBindVertexArray(sphereVertexArrayObjectID);
	//mat4 sphereModelToWorldMatrix = glm::translate(2.0f, 1.0f, -1.0f);
	//modelToProjectionMat = worldToProjectionMatrix * sphereModelToWorldMatrix;
	//glUniformMatrix4fv(fullTransformUniLoc, 1, GL_FALSE, &modelToProjectionMat[0][0]);
	//glUniformMatrix4fv(modelToWorldMatUniLoc, 1, GL_FALSE, &sphereModelToWorldMatrix[0][0]);
	//glDrawElements(GL_TRIANGLES, sphereIndices, GL_UNSIGNED_SHORT, (void*)sphereIndexByteOffset);

	// Plane
	glBindVertexArray(planeVertexArrayObjectID);
	mat4 planeModelToWorldMatrix = glm::mat4();
	modelToProjectionMat = worldToProjectionMatrix * planeModelToWorldMatrix;
	glUniformMatrix4fv(fullTransformUniLoc, 1, GL_FALSE, &modelToProjectionMat[0][0]);
	glUniformMatrix4fv(modelToWorldMatUniLoc, 1, GL_FALSE, &planeModelToWorldMatrix[0][0]);
	//glDrawElements(GL_TRIANGLES, planeIndices, GL_UNSIGNED_SHORT, (void*)planeIndexByteOffset);
}

void MeGlWindow::mouseMoveEvent(QMouseEvent* e)
{
	setFocus();
	camera.mouseUpdate(glm::vec2(e->x(), e->y()));
	repaint();
}

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
	case Qt::Key::Key_Q:
		camera.moveUp();
		break;
	case Qt::Key::Key_E:
		camera.moveDown();
		break;
	case Qt::Key::Key_J:
		lightPos_x += 0.2f;
		break;
	case Qt::Key::Key_L:
		lightPos_x += -0.2f;
		break;
	case Qt::Key::Key_I:
		lightPos_y += 0.2f;
		break;
	case Qt::Key::Key_K:
		lightPos_y += -0.2f;
		break;
	case Qt::Key::Key_Up:
		angle_x += 3.6f;
		break;
	case Qt::Key::Key_Down:
		angle_x += -3.6f;
		break;
	case Qt::Key::Key_Left:
		angle_y += -3.6f;
		break;
	case Qt::Key::Key_Right:
		angle_y += 3.6f;
		break;
	case Qt::Key::Key_R:
		camera.resetCamera();
		angle_x = 0.0f;
		angle_y = -30.0f;
		lightPos_x = -2.0f;
		lightPos_y = 4.0f;
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

void MeGlWindow::initTextures()
{
	glEnable(GL_TEXTURE_2D);

	//int width = 64, height = 64;
	QImage tex_1 = QGLWidget::convertToGLFormat(QImage("Shapes.png", "png"));

	glActiveTexture(GL_TEXTURE0);
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); // GL_NEAREST
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(
		GL_TEXTURE_2D, 0, GL_RGBA, tex_1.width(), tex_1.height(),
		0, GL_RGBA, GL_UNSIGNED_BYTE, tex_1.bits());
	glUniform1i(glGetUniformLocation(programID, "normalMap_1"), GL_TEXTURE0);

	glDisable(GL_TEXTURE_2D);
}

void MeGlWindow::initializeGL()
{
	setMouseTracking(false);
	glewInit();
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	sendContent();
	initTextures();
	installShaders();
	fullTransformUniLoc = glGetUniformLocation(programID, "modelToProjectionMat");
}

MeGlWindow::~MeGlWindow()
{
	glDeleteBuffers(1, &bufferID);
	glDeleteVertexArrays(1, &cubeVertexArrayObjectID);
	//glDeleteVertexArrays(1, &arrowVertexArrayObjectID);
	//glDeleteVertexArrays(1, &sphereVertexArrayObjectID);
	glDeleteVertexArrays(1, &planeVertexArrayObjectID);
	glUseProgram(0);
	glDeleteProgram(programID);
}
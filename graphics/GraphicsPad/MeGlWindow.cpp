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
GLuint cubeMapProgramID;
GLuint bufferID;
GLuint fullTransformUniLoc;
GLuint lightbulbTransformUniLoc;

GLuint cubeIndices;
GLuint cubeVertexArrayObjectID;
GLuint cubeIndexByteOffset;

GLuint lightbulbIndices;
GLuint lightbulbVertexArrayObjectID;
GLuint lightbulbIndexByteOffset;

//GLuint arrowIndices;
//GLuint arrowVertexArrayObjectID;
//GLuint arrowIndexByteOffset;

GLuint planeIndices;
GLuint planeVertexArrayObjectID;
GLuint planeIndexByteOffset;

vec3 cubeAngle(0.0f, -30.0f, 0.0f);
vec3 lightPos(-2.0f, 4.0f, -3.5f);

void MeGlWindow::sendContent()
{
	ShapeData cube = ShapeGenerator::makeCube();
	ShapeData lightbulb = ShapeGenerator::makeCube();
	//ShapeData arrow = ShapeGenerator::makeArrow();
	ShapeData plane = ShapeGenerator::makePlane();

	glGenBuffers(1, &bufferID);
	glBindBuffer(GL_ARRAY_BUFFER, bufferID);
	glBufferData(GL_ARRAY_BUFFER, 
		cube.vertexBufferSize() + cube.indexBufferSize()
		+ lightbulb.vertexBufferSize() + lightbulb.indexBufferSize()
		//+ arrow.vertexBufferSize() + arrow.indexBufferSize()
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
	//currentOffset += lightbulb.indexBufferSize();
	glBufferSubData(GL_ARRAY_BUFFER, currentOffset, plane.vertexBufferSize(), plane.vertices);
	currentOffset += plane.vertexBufferSize();
	glBufferSubData(GL_ARRAY_BUFFER, currentOffset, plane.indexBufferSize(), plane.indices);

	cubeIndices = cube.numIndices;
	lightbulbIndices = lightbulb.numIndices;
	//arrowIndices = arrow.numIndices;
	planeIndices = plane.numIndices;

	glGenVertexArrays(1, &cubeVertexArrayObjectID);
	glGenVertexArrays(1, &lightbulbVertexArrayObjectID);
	//glGenVertexArrays(1, &arrowVertexArrayObjectID);
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
	glEnableVertexAttribArray(3);
	glEnableVertexAttribArray(4);
	glBindBuffer(GL_ARRAY_BUFFER, bufferID);
	GLuint planeByteOffset = cube.vertexBufferSize() + cube.indexBufferSize();
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, VERTEX_BYTE_SIZE, (void*)planeByteOffset);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, VERTEX_BYTE_SIZE, (void*)(planeByteOffset + sizeof(float) * 3));
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, VERTEX_BYTE_SIZE, (void*)(planeByteOffset + sizeof(float) * 6));
	glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, VERTEX_BYTE_SIZE, (void*)(planeByteOffset + sizeof(float) * 9));
	glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, VERTEX_BYTE_SIZE, (void*)(planeByteOffset + sizeof(float) * 11));
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufferID);

	cubeIndexByteOffset = cube.vertexBufferSize();
	//arrowIndexByteOffset = arrowByteOffset + arrow.vertexBufferSize();
	planeIndexByteOffset = planeByteOffset + plane.vertexBufferSize();

	cube.cleanup();
	lightbulb.cleanup();
	//arrow.cleanup();
	plane.cleanup();
}

void MeGlWindow::paintGL()
{
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	glViewport(0, 0, width(), height());
	
	mat4 MVP;
	// Projection matrix
	mat4 viewToProjectionMatrix = glm::perspective(60.0f, ((float)width()) / height(), 0.1f, 20.0f);
	// View matrix
	mat4 worldToViewMatrix = camera.getWorldToViewMatrix();
	// Projection * View
	mat4 worldToProjectionMatrix = viewToProjectionMatrix * worldToViewMatrix;
	
	//glUseProgram(programID);

	GLuint modelToWorldMatUniLoc = glGetUniformLocation(programID, "modelToWorldMatrix");

	// Ambient light
	GLuint ambientLightUniLoc = glGetUniformLocation(programID, "ambientLight");
	vec4 ambientLight(0.1f, 0.1f, 0.1f, 1.0f);
	glUniform4fv(ambientLightUniLoc, 1, &ambientLight[0]);

	// Diffuse light
	GLuint lightPositionWorldUniLoc = glGetUniformLocation(programID, "lightPositionWorld");
	vec3 lightPositionWorld(lightPos);
	glUniform3fv(lightPositionWorldUniLoc, 1, &lightPositionWorld[0]);

	// Specular light
	GLuint cameraPositionWorldUniLoc = glGetUniformLocation(programID, "cameraPositionWorld");
	vec3 cameraPosition = camera.getPosition();
	glUniform3fv(cameraPositionWorldUniLoc, 1, &cameraPosition[0]);

	// Cube
	//glUseProgram(cubeMapProgramID);
	glBindVertexArray(cubeVertexArrayObjectID);
	mat4 cubeTranslateMatrix = glm::translate(0.0f, 1.0f, -3.0f);
	mat4 cubeRotaionMatrix_x = glm::rotate(cubeAngle.x, vec3(1.0f, 0.0f, 0.0f));
	mat4 cubeRotaionMatrix_y = glm::rotate(cubeAngle.y, vec3(0.0f, 1.0f, 0.0f));
	mat4 cubeModelToWorldMatrix = cubeTranslateMatrix * cubeRotaionMatrix_x * cubeRotaionMatrix_y;
	MVP = worldToProjectionMatrix * cubeModelToWorldMatrix;
	glUniformMatrix4fv(fullTransformUniLoc, 1, GL_FALSE, &MVP[0][0]);
	glUniformMatrix4fv(modelToWorldMatUniLoc, 1, GL_FALSE, &cubeModelToWorldMatrix[0][0]);
	glDrawElements(GL_TRIANGLES, cubeIndices, GL_UNSIGNED_SHORT, (void*)cubeIndexByteOffset);

	// Lightbulb
	//glUseProgram(programID);
	//glBindVertexArray(lightbulbVertexArrayObjectID);
	glBindVertexArray(cubeVertexArrayObjectID);
	cubeTranslateMatrix = glm::translate(lightPos);
	mat4 cubeScaleMatrix = glm::scale(vec3(0.1f, 0.1f, 0.1f));
	cubeModelToWorldMatrix = cubeTranslateMatrix * cubeScaleMatrix;
	MVP = worldToProjectionMatrix * cubeModelToWorldMatrix;
	glUniformMatrix4fv(fullTransformUniLoc, 1, GL_FALSE, &MVP[0][0]);
	glDrawElements(GL_TRIANGLES, cubeIndices, GL_UNSIGNED_SHORT, (void*)cubeIndexByteOffset);

	// Plane
	glBindVertexArray(planeVertexArrayObjectID);
	mat4 planeModelToWorldMatrix = glm::mat4();
	MVP = worldToProjectionMatrix * planeModelToWorldMatrix;
	glUniformMatrix4fv(fullTransformUniLoc, 1, GL_FALSE, &MVP[0][0]);
	glUniformMatrix4fv(modelToWorldMatUniLoc, 1, GL_FALSE, &planeModelToWorldMatrix[0][0]);
	glDrawElements(GL_TRIANGLES, planeIndices, GL_UNSIGNED_SHORT, (void*)planeIndexByteOffset);
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
		lightPos.x += 0.2f;
		break;
	case Qt::Key::Key_L:
		lightPos.x += -0.2f;
		break;
	case Qt::Key::Key_P:
		lightPos.y += 0.2f;
		break;
	case Qt::Key::Key_O:
		lightPos.y += -0.2f;
		break;
	case Qt::Key::Key_I:
		lightPos.z += 0.2f;
		break;
	case Qt::Key::Key_K:
		lightPos.z += -0.2f;
		break;
	case Qt::Key::Key_Up:
		cubeAngle.x += 3.6f;
		break;
	case Qt::Key::Key_Down:
		cubeAngle.x += -3.6f;
		break;
	case Qt::Key::Key_Left:
		cubeAngle.y += -3.6f;
		break;
	case Qt::Key::Key_Right:
		cubeAngle.y += 3.6f;
		break;
	case Qt::Key::Key_R:
		camera.resetCamera();
		cubeAngle.x = 0.0f;
		cubeAngle.y = -30.0f;
		lightPos.x = -2.0f;
		lightPos.y = 4.0f;
		lightPos.z = -3.5f;
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

	glDeleteShader(vertexShaderID);
	glDeleteShader(fragmentShaderID);

	vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

	temp = readShader("CubemapVertexShader.glsl");
	adapter[0] = temp.c_str();
	glShaderSource(vertexShaderID, 1, adapter, 0);
	temp = readShader("CubemapFragmentShader.glsl");
	adapter[0] = temp.c_str();
	glShaderSource(fragmentShaderID, 1, adapter, 0);

	glCompileShader(vertexShaderID);
	glCompileShader(fragmentShaderID);

	cubeMapProgramID = glCreateProgram();
	glAttachShader(cubeMapProgramID, vertexShaderID);
	glAttachShader(cubeMapProgramID, fragmentShaderID);
	glLinkProgram(cubeMapProgramID);

	glUseProgram(programID);
	//glUseProgram(cubeMapProgramID);
}

void MeGlWindow::initTextures()
{
	glEnable(GL_TEXTURE_2D);
	GLuint textureID;
	string baseTexPath = "../Textures/";

	// Normal map - Shapes
	QImage tex_N_1 = QGLWidget::convertToGLFormat(QImage("../Textures/Shapes.png", "png"));

	glActiveTexture(GL_TEXTURE0);
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); // GL_NEAREST
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(
		GL_TEXTURE_2D, 0, GL_RGBA, tex_N_1.width(), tex_N_1.height(),
		0, GL_RGBA, GL_UNSIGNED_BYTE, tex_N_1.bits());
	glUniform1i(glGetUniformLocation(programID, "normalMap_1"), GL_TEXTURE0);

	// Cube map
	glActiveTexture(GL_TEXTURE1);
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

	const char* suffix[] = {
		"posx", "negx", "posy", "negy", "posz", "negz"
	};
	GLuint targets[] = {
		GL_TEXTURE_CUBE_MAP_POSITIVE_X,
		GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
		GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
		GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
		GL_TEXTURE_CUBE_MAP_POSITIVE_Z,
		GL_TEXTURE_CUBE_MAP_NEGATIVE_Z
	};

	for (int i = 0; i < 6; i++) {
		string texName = baseTexPath + suffix[i] + ".png";
		QImage cubeMap = QGLWidget::convertToGLFormat(QImage(texName.c_str(), "png"));
		glTexImage2D(
			targets[i], 0, GL_RGBA, cubeMap.width(), cubeMap.height(),
			0, GL_RGBA, GL_UNSIGNED_BYTE, cubeMap.bits());
	}

	glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glUniform1i(glGetUniformLocation(programID, "cubeMapTex"), GL_TEXTURE1);

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
	fullTransformUniLoc = glGetUniformLocation(programID, "MVP");
	//lightbulbTransformUniLoc = glGetUniformLocation(cubeMapProgramID, "MVP");
}

MeGlWindow::~MeGlWindow()
{
	glDeleteBuffers(1, &bufferID);
	glDeleteVertexArrays(1, &cubeVertexArrayObjectID);
	glDeleteVertexArrays(1, &lightbulbVertexArrayObjectID);
	//glDeleteVertexArrays(1, &arrowVertexArrayObjectID);
	glDeleteVertexArrays(1, &planeVertexArrayObjectID);
	glUseProgram(0);
	glDeleteProgram(programID);
}
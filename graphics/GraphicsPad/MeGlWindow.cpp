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
using glm::mat3;
using glm::mat4;

const uint NUM_VERTICES_PER_TRI = 3;
const uint NUM_FLOATS_PER_VERTICE = 14;
const uint VERTEX_BYTE_SIZE = NUM_FLOATS_PER_VERTICE * sizeof(float);

Camera camera;
GLuint programID;
GLuint cubeMapProgramID;
GLuint bufferID;

GLuint cubeIndices;
GLuint cubeVertexArrayObjectID;
GLuint cubeIndexByteOffset;

GLuint skyboxIndexByteOffset;

GLuint planeIndices;
GLuint planeVertexArrayObjectID;
GLuint planeIndexByteOffset;

vec3 cubeAngle(0.0f, -30.0f, 0.0f);
vec3 lightPos(-2.0f, 4.0f, -3.5f);

void MeGlWindow::sendContent()
{
	ShapeData cube = ShapeGenerator::makeCube();
	ShapeData plane = ShapeGenerator::makePlane(20);

	glGenBuffers(1, &bufferID);
	glBindBuffer(GL_ARRAY_BUFFER, bufferID);
	glBufferData(GL_ARRAY_BUFFER, 
		cube.vertexBufferSize() + cube.indexBufferSize()
		+ plane.vertexBufferSize() + plane.indexBufferSize(),
		0, GL_STATIC_DRAW);

	GLsizeiptr currentOffset = 0;
	glBufferSubData(GL_ARRAY_BUFFER, currentOffset, cube.vertexBufferSize(), cube.vertices);
	currentOffset += cube.vertexBufferSize();
	glBufferSubData(GL_ARRAY_BUFFER, currentOffset, cube.indexBufferSize(), cube.indices);
	currentOffset += cube.indexBufferSize();
	glBufferSubData(GL_ARRAY_BUFFER, currentOffset, plane.vertexBufferSize(), plane.vertices);
	currentOffset += plane.vertexBufferSize();
	glBufferSubData(GL_ARRAY_BUFFER, currentOffset, plane.indexBufferSize(), plane.indices);

	cubeIndices = cube.numIndices;
	planeIndices = plane.numIndices;

	glGenVertexArrays(1, &cubeVertexArrayObjectID);
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
	planeIndexByteOffset = planeByteOffset + plane.vertexBufferSize();

	cube.cleanup();
	plane.cleanup();
}

void MeGlWindow::paintGL()
{
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	glViewport(0, 0, width(), height());

	glUseProgram(programID);
	
	// Projection matrix
	mat4 viewToProjectionMatrix = glm::perspective(60.0f, ((float)width()) / height(), 0.1f, 20.0f);
	// View matrix
	mat4 worldToViewMatrix = camera.getWorldToViewMatrix();
	// Projection * View
	mat4 worldToProjectionMatrix = viewToProjectionMatrix * worldToViewMatrix;
	// Model View Projection
	mat4 MVP;
	
	GLuint fullTransformUniLoc = glGetUniformLocation(programID, "MVP");
	GLuint modelToWorldMatUniLoc = glGetUniformLocation(programID, "modelToWorldMatrix");
	GLuint normalMatrixUniLoc = glGetUniformLocation(programID, "normalMatrix");
	//GLuint cm_modelMatrixUniLoc = glGetUniformLocation(cubeMapProgramID, "modelToWorldMatrix");

	// Ambient light
	GLuint ambientLightUniLoc = glGetUniformLocation(programID, "ambientLight");
	vec4 ambientLight(0.1f, 0.1f, 0.1f, 1.0f);
	glUniform4fv(ambientLightUniLoc, 1, &ambientLight[0]);

	// Diffuse light
	GLuint lightPositionWorldUniLoc = glGetUniformLocation(programID, "lightPositionWorld");
	glUniform3fv(lightPositionWorldUniLoc, 1, &lightPos[0]);

	// Specular light
	GLuint cameraPositionWorldUniLoc = glGetUniformLocation(programID, "cameraPositionWorld");
	vec3 cameraPosition = camera.getPosition();
	glUniform3fv(cameraPositionWorldUniLoc, 1, &cameraPosition[0]);
	//GLuint cm_cameraPositionWorldUniLoc= glGetUniformLocation(cubeMapProgramID, "cameraPositionWorld");
	//glUniform3fv(cm_cameraPositionWorldUniLoc, 1, &lightPositionWorld[0]);

	// Cube
	glBindVertexArray(cubeVertexArrayObjectID);

	mat4 cubeTranslateMatrix = glm::translate(0.0f, 1.0f, -3.0f);
	mat4 cubeRotaionMatrix_x = glm::rotate(cubeAngle.x, vec3(1.0f, 0.0f, 0.0f));
	mat4 cubeRotaionMatrix_y = glm::rotate(cubeAngle.y, vec3(0.0f, 1.0f, 0.0f));

	mat4 cubeModelToWorldMatrix = cubeTranslateMatrix * cubeRotaionMatrix_x * cubeRotaionMatrix_y;
	glUniformMatrix4fv(modelToWorldMatUniLoc, 1, GL_FALSE, &cubeModelToWorldMatrix[0][0]);

	MVP = worldToProjectionMatrix * cubeModelToWorldMatrix;
	glUniformMatrix4fv(fullTransformUniLoc, 1, GL_FALSE, &MVP[0][0]);

	//glUniformMatrix4fv(cm_modelMatrixUniLoc, 1, GL_FALSE, &cubeModelToWorldMatrix[0][0]);
	mat3 normalMatrix = glm::transpose(glm::inverse(mat3(worldToViewMatrix * cubeModelToWorldMatrix)));
	glUniformMatrix3fv(normalMatrixUniLoc, 1, GL_FALSE, &normalMatrix[0][0]);

	glDrawElements(GL_TRIANGLES, cubeIndices, GL_UNSIGNED_SHORT, (void*)cubeIndexByteOffset);

	// Plane
	glBindVertexArray(planeVertexArrayObjectID);

	mat4 planeModelToWorldMatrix = glm::mat4();
	glUniformMatrix4fv(modelToWorldMatUniLoc, 1, GL_FALSE, &planeModelToWorldMatrix[0][0]);

	MVP = worldToProjectionMatrix * planeModelToWorldMatrix;
	glUniformMatrix4fv(fullTransformUniLoc, 1, GL_FALSE, &MVP[0][0]);

	//glUniformMatrix4fv(cm_modelMatrixUniLoc, 1, GL_FALSE, &planeModelToWorldMatrix[0][0]);
	glDrawElements(GL_TRIANGLES, planeIndices, GL_UNSIGNED_SHORT, (void*)planeIndexByteOffset);

	// Lightbulb
	//glUseProgram(programID);
	glBindVertexArray(cubeVertexArrayObjectID);

	cubeTranslateMatrix = glm::translate(lightPos);
	mat4 cubeScaleMatrix = glm::scale(vec3(0.1f, 0.1f, 0.1f));

	cubeModelToWorldMatrix = cubeTranslateMatrix * cubeScaleMatrix;
	MVP = worldToProjectionMatrix * cubeModelToWorldMatrix;
	glUniformMatrix4fv(fullTransformUniLoc, 1, GL_FALSE, &MVP[0][0]);

	glDrawElements(GL_TRIANGLES, cubeIndices, GL_UNSIGNED_SHORT, (void*)cubeIndexByteOffset);

	// Skybox
	glUseProgram(cubeMapProgramID);
	glBindVertexArray(cubeVertexArrayObjectID);

	cubeTranslateMatrix = glm::mat4();
	cubeScaleMatrix = glm::scale(vec3(20.0f));

	cubeModelToWorldMatrix = cubeTranslateMatrix * cubeScaleMatrix;
	worldToViewMatrix[3][0] = 0.0;
	worldToViewMatrix[3][1] = 0.0;
	worldToViewMatrix[3][2] = 0.0;
	MVP = viewToProjectionMatrix * worldToViewMatrix * cubeModelToWorldMatrix;
	GLuint skyboxFullTransformMatUniLoc = glGetUniformLocation(cubeMapProgramID, "MVP");
	glUniformMatrix4fv(skyboxFullTransformMatUniLoc, 1, GL_FALSE, &MVP[0][0]);

	glDrawElements(GL_TRIANGLES, cubeIndices, GL_UNSIGNED_SHORT, (void*)cubeIndexByteOffset);
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

bool checkStatus(
	GLuint objectID,
	PFNGLGETSHADERIVPROC objectPropertyGetterFunc,
	PFNGLGETSHADERINFOLOGPROC getInfoLogFunc,
	GLenum statusType)
{
	GLint status;
	objectPropertyGetterFunc(objectID, statusType, &status);
	if (status != GL_TRUE)
	{
		GLint infoLogLength;
		objectPropertyGetterFunc(objectID, GL_INFO_LOG_LENGTH, &infoLogLength);
		GLchar* buffer = new GLchar[infoLogLength];

		GLsizei bufferSize;
		getInfoLogFunc(objectID, infoLogLength, &bufferSize, buffer);
		std::cout << buffer << std::endl;
		delete[] buffer;
		return false;
	}
	return true;
}

bool MeGlWindow::checkShaderStatus(GLuint shaderID)
{
	return checkStatus(shaderID, glGetShaderiv, glGetShaderInfoLog, GL_COMPILE_STATUS);
}

bool MeGlWindow::checkProgramStatus(GLuint programID)
{
	return checkStatus(programID, glGetProgramiv, glGetProgramInfoLog, GL_LINK_STATUS);
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

	if (!checkShaderStatus(vertexShaderID) || !checkShaderStatus(fragmentShaderID))
		return;

	programID = glCreateProgram();
	glAttachShader(programID, vertexShaderID);
	glAttachShader(programID, fragmentShaderID);
	glLinkProgram(programID);

	if (!checkProgramStatus(programID))
		return;

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

	if (!checkShaderStatus(vertexShaderID) || !checkShaderStatus(fragmentShaderID))
		return;

	cubeMapProgramID = glCreateProgram();
	glAttachShader(cubeMapProgramID, vertexShaderID);
	glAttachShader(cubeMapProgramID, fragmentShaderID);
	glLinkProgram(cubeMapProgramID);

	if (!checkProgramStatus(cubeMapProgramID))
		return;

	glDeleteShader(vertexShaderID);
	glDeleteShader(fragmentShaderID);
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
	glUniform1i(glGetUniformLocation(programID, "normalMap_1"), 0);

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

	glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glUniform1i(glGetUniformLocation(cubeMapProgramID, "cubeMapTex"), 1);

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
}

MeGlWindow::~MeGlWindow()
{
	glDeleteBuffers(1, &bufferID);
	glDeleteVertexArrays(1, &cubeVertexArrayObjectID);
	glDeleteVertexArrays(1, &planeVertexArrayObjectID);
	glUseProgram(0);
	glDeleteProgram(programID);
}
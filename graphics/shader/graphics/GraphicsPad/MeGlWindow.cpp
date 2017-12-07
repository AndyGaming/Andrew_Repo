#include <gl\glew.h>
#include <MeGlWindow.h>
#include <Vertex.h>
#include <ShapeFactory.h>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\glm.hpp>
#include <QtGui\qkeyevent>
#include <Camera.h>
#include <iostream>
#include <fstream>

using namespace std;
using namespace glm;

static GLuint cubeIndexBufferOffset;
static GLuint planeIndexBufferOffset;
static GLuint cubeIndices;
static GLuint planeIndices;
static GLuint cubeVertexArrayObjectID;
static GLuint planeVertexArrayObjectID;

GLuint programID;
GLuint lightProgramID;
GLuint cubeMapProgramID;
GLuint shadowProgramID;
GLuint testProgramID;

GLuint frameBufferID;
GLuint frameTextureID;
GLuint frameDepthID;

GLfloat attenuationAmount = 0.08;

Camera MainCamera;
Camera LightCamera;

vec3 LightPosition(0.0f, 2.5f, -2.0f);
float RotationAngle = 0.0f;
vec3 colorVariation = vec3(0.01f, 0.02f, 0.03f);
bool plusVariation = true;

const char* MeGlWindow::TexFile[] = { "posx.png","negx.png","negy.png","posy.png","posz.png","negz.png" };

void MeGlWindow::sendContent()
{
	glClearColor(0, 0, 0, 1);

	Shapedata Cube = ShapeFactory::MakeCube();
	Shapedata Plane = ShapeFactory::MakePlane();

	GLuint bufferID;
	GLuint currentbufferoffset = 0;

	glGenBuffers(1, &bufferID);
	glBindBuffer(GL_ARRAY_BUFFER, bufferID);
	glBufferData(GL_ARRAY_BUFFER, Cube.VertexBufferSize()+Cube.IndicesBufferSize()+Plane.VertexBufferSize()+Plane.IndicesBufferSize(), 0, GL_STATIC_DRAW);

	glBufferSubData(GL_ARRAY_BUFFER, 0, Cube.VertexBufferSize(), Cube.vertices);
	currentbufferoffset = Cube.VertexBufferSize();
	cubeIndexBufferOffset = currentbufferoffset;
	glBufferSubData(GL_ARRAY_BUFFER, currentbufferoffset, Cube.IndicesBufferSize(), Cube.Indices);
	currentbufferoffset += Cube.IndicesBufferSize();
	glBufferSubData(GL_ARRAY_BUFFER, currentbufferoffset, Plane.VertexBufferSize(),Plane.vertices);
	currentbufferoffset += Plane.VertexBufferSize();
	planeIndexBufferOffset = currentbufferoffset;
	glBufferSubData(GL_ARRAY_BUFFER, currentbufferoffset, Plane.IndicesBufferSize(), Plane.Indices);
	cubeIndices = Cube.numIndices;
	planeIndices = Plane.numIndices;

	glGenVertexArrays(1, &cubeVertexArrayObjectID);
	glGenVertexArrays(1, &planeVertexArrayObjectID);

	glBindVertexArray(cubeVertexArrayObjectID);
	glBindBuffer(GL_ARRAY_BUFFER, bufferID);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 15 * sizeof(float), 0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 15 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 15 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, 15 * sizeof(float), (void*)(9 * sizeof(float)));
	glEnableVertexAttribArray(4);
	glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, 15 * sizeof(float), (void*)(11 * sizeof(float)));
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufferID);

	glBindVertexArray(planeVertexArrayObjectID);
	glBindBuffer(GL_ARRAY_BUFFER, bufferID);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 15 * sizeof(float), (void*)(Cube.VertexBufferSize()+Cube.IndicesBufferSize()));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 15 * sizeof(float), (void*)(3 * sizeof(float) + Cube.VertexBufferSize() + Cube.IndicesBufferSize()));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 15 * sizeof(float), (void*)(6 * sizeof(float) + Cube.VertexBufferSize() + Cube.IndicesBufferSize()));
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, 15 * sizeof(float), (void*)(9 * sizeof(float) + Cube.VertexBufferSize() + Cube.IndicesBufferSize()));
	glEnableVertexAttribArray(4);
	glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, 15 * sizeof(float), (void*)(11 * sizeof(float)+ Cube.VertexBufferSize() + Cube.IndicesBufferSize()));
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufferID);
	
	Cube.cleanup();
	Plane.cleanup();
}

void MeGlWindow::initTextures() 
{
	const string baseTexPath = "../Textures/";

	const char* texName = "Splatter.png";
	string texFileName = baseTexPath + texName;
	QImage texture = QGLWidget::convertToGLFormat(QImage(texFileName.c_str(), "PNG"));

	glActiveTexture(GL_TEXTURE0);

	GLuint TextureBufferID;
	glGenTextures(1, &TextureBufferID);
	glBindTexture(GL_TEXTURE_2D, TextureBufferID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texture.width(), texture.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, texture.bits());
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	const char* normalMapName = "Shapes.png";
	texFileName = baseTexPath + normalMapName;
	QImage Normalmap = QGLWidget::convertToGLFormat(QImage(texFileName.c_str(), "PNG"));

	glActiveTexture(GL_TEXTURE1);

	GLuint normalBufferID;
	glGenTextures(1, &normalBufferID);
	glBindTexture(GL_TEXTURE_2D, normalBufferID);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, Normalmap.width(), Normalmap.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, Normalmap.bits());
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glActiveTexture(GL_TEXTURE2);
	GLuint CubeBufferID;
	glGenTextures(1, &CubeBufferID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, CubeBufferID);

	for (int i = 0; i < 6; ++i) {
		texFileName = baseTexPath + TexFile[i];
		QImage Texdata = QGLWidget::convertToGLFormat(QImage(texFileName.c_str(), "PNG"));
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA, Texdata.width(), Texdata.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, Texdata.bits());
	}
	glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glActiveTexture(GL_TEXTURE3);

	glGenFramebuffers(1, &frameBufferID);
	glBindBuffer(GL_FRAMEBUFFER, frameBufferID);
	glDrawBuffer(GL_COLOR_ATTACHMENT0);
	glDrawBuffer(GL_DEPTH_ATTACHMENT);

	glGenTextures(1, &frameTextureID);
	glBindTexture(GL_TEXTURE_2D, frameTextureID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width(), height(), 0, GL_RGB, GL_FLOAT, NULL);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glActiveTexture(GL_TEXTURE4);
	glGenTextures(1, &frameDepthID);
	glBindTexture(GL_TEXTURE_2D, frameDepthID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, width(), height(), 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
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

std::string MeGlWindow::ReadShaderCode(const char* fileName)
{
	std::ifstream meInput(fileName);
	if (!meInput.good())
	{
		std::cout << "File failed to load..." << fileName;
		exit(1);
	}
	return std::string(
		std::istreambuf_iterator<char>(meInput),
		std::istreambuf_iterator<char>());
}

void MeGlWindow::installshaders()
{
	GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

	const GLchar* adapter[1];
	std::string temp = ReadShaderCode("VertexShader.glsl");
	adapter[0] = temp.c_str();
	glShaderSource(VertexShaderID, 1, adapter, 0);
	temp = ReadShaderCode("FragmentShader.glsl");
	adapter[0] = temp.c_str();
	glShaderSource(FragmentShaderID, 1, adapter, 0);

	glCompileShader(VertexShaderID);
	glCompileShader(FragmentShaderID);

	if (!checkShaderStatus(VertexShaderID) || !checkShaderStatus(FragmentShaderID))
		return;

	programID = glCreateProgram();
	glAttachShader(programID, VertexShaderID);
	glAttachShader(programID, FragmentShaderID);
	glLinkProgram(programID);

	if (!checkProgramStatus(programID))
		return;

	glDeleteShader(VertexShaderID);
	glDeleteShader(FragmentShaderID);

	VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

	temp = ReadShaderCode("CubeLightVertexShader.glsl");
	adapter[0] = temp.c_str();
	glShaderSource(VertexShaderID, 1, adapter, 0);
	temp = ReadShaderCode("CubeLightFragmentShader.glsl");
	adapter[0] = temp.c_str();
	glShaderSource(FragmentShaderID, 1, adapter, 0);

	glCompileShader(VertexShaderID);
	glCompileShader(FragmentShaderID);

	if (!checkShaderStatus(VertexShaderID) || !checkShaderStatus(FragmentShaderID))
		return;

	lightProgramID = glCreateProgram();
	glAttachShader(lightProgramID, VertexShaderID);
	glAttachShader(lightProgramID, FragmentShaderID);
	glLinkProgram(lightProgramID);
	if (!checkProgramStatus(lightProgramID))
		return;

	glDeleteShader(VertexShaderID);
	glDeleteShader(FragmentShaderID);

	VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

	temp = ReadShaderCode("CubeMapVertexShader.glsl");
	adapter[0] = temp.c_str();
	glShaderSource(VertexShaderID, 1, adapter, 0);
	temp = ReadShaderCode("CubeMapFragmentShader.glsl");
	adapter[0] = temp.c_str();
	glShaderSource(FragmentShaderID, 1, adapter, 0);

	glCompileShader(VertexShaderID);
	glCompileShader(FragmentShaderID);

	if (!checkShaderStatus(VertexShaderID) || !checkShaderStatus(FragmentShaderID))
		return;

	cubeMapProgramID = glCreateProgram();
	glAttachShader(cubeMapProgramID, VertexShaderID);
	glAttachShader(cubeMapProgramID, FragmentShaderID);
	glLinkProgram(cubeMapProgramID);
	if (!checkProgramStatus(cubeMapProgramID))
		return;

	glDeleteShader(VertexShaderID);
	glDeleteShader(FragmentShaderID);

	VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

	temp = ReadShaderCode("ShadowMapVertexShader.glsl");
	adapter[0] = temp.c_str();
	glShaderSource(VertexShaderID, 1, adapter, 0);
	temp = ReadShaderCode("ShadowMapFragmentShader.glsl");
	adapter[0] = temp.c_str();
	glShaderSource(FragmentShaderID, 1, adapter, 0);

	glCompileShader(VertexShaderID);
	glCompileShader(FragmentShaderID);

	if (!checkShaderStatus(VertexShaderID) || !checkShaderStatus(FragmentShaderID))
		return;

	testProgramID = glCreateProgram();
	glAttachShader(testProgramID, VertexShaderID);
	glAttachShader(testProgramID, FragmentShaderID);
	glLinkProgram(testProgramID);
	if (!checkProgramStatus(testProgramID))
		return;

	glDeleteShader(VertexShaderID);
	glDeleteShader(FragmentShaderID);

	VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

	temp = ReadShaderCode("PlaneShadowVertexShader.glsl");
	adapter[0] = temp.c_str();
	glShaderSource(VertexShaderID, 1, adapter, 0);
	temp = ReadShaderCode("PlaneShadowFragmentShader.glsl");
	adapter[0] = temp.c_str();
	glShaderSource(FragmentShaderID, 1, adapter, 0);

	glCompileShader(VertexShaderID);
	glCompileShader(FragmentShaderID);

	if (!checkShaderStatus(VertexShaderID) || !checkShaderStatus(FragmentShaderID))
		return;

	shadowProgramID = glCreateProgram();
	glAttachShader(shadowProgramID, VertexShaderID);
	glAttachShader(shadowProgramID, FragmentShaderID);
	glLinkProgram(shadowProgramID);
	if (!checkProgramStatus(shadowProgramID))
		return;

	glDeleteShader(VertexShaderID);
	glDeleteShader(FragmentShaderID);
}

void MeGlWindow::initializeGL()
{
	glewInit();
	glEnable(GL_DEPTH_TEST);
	installshaders();
	sendContent();
	initTextures();

	Mytimer = new QTimer(this);
	connect(Mytimer, SIGNAL(timeout()), this, SLOT(update()));
	Mytimer->setInterval(0);
	Mytimer->start();
}

void MeGlWindow::paintGL()
{
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	glViewport(0, 0, width(), height());

	//render to shadow map
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, frameBufferID);
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, frameTextureID, 0);
	glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, frameDepthID, 0);
	GLuint status = glCheckFramebufferStatus(GL_DRAW_FRAMEBUFFER);
	assert(status == GL_FRAMEBUFFER_COMPLETE);

	LightCamera.setPosition(LightPosition);
	DrawObjects(LightCamera);

	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	glViewport(0, 0, width(), height());
	mat4 CameraMatrix = MainCamera.getWorldToViewMatrix();
	mat4 projectionMatrix = perspective(60.0f, ((float)width() / height()), 0.3f, 100.0f);

	mat4 WorldToProjectionMatrix = projectionMatrix * CameraMatrix;

	mat4 FullTransformMatrix;

	glUseProgram(programID);

	GLint TextureUniformLocation = glGetUniformLocation(programID, "tex_temp");
	glUniform1i(TextureUniformLocation, 0);
	GLint NormalmapUniformLocation = glGetUniformLocation(programID, "NormalMap");
	glUniform1i(NormalmapUniformLocation, 1);

	GLuint FullTransformMatrixUniformLocaiton;
	FullTransformMatrixUniformLocaiton = glGetUniformLocation(programID, "FullTransformMatrix");
	GLuint ModelToWorldMatrixUniformLocation;
	ModelToWorldMatrixUniformLocation = glGetUniformLocation(programID, "ModelToWorldMatrix");
	mat4 TransformMatrix;
	mat4 RotationMatrix;
	mat4 ScaleMatrix;
	//Light Begins Here
	vec3 AmbientLight(0.2f, 0.2f, 0.2f);

	GLuint AmbientLightUniformLocation = glGetUniformLocation(programID, "AmbientLight");
	glUniform3fv(AmbientLightUniformLocation, 1, &AmbientLight[0]);

	GLuint LightPositionUniformLocation = glGetUniformLocation(programID, "LightPosition");
	glUniform3fv(LightPositionUniformLocation, 1, &LightPosition[0]);

	GLuint ViewPositionUniformLocation = glGetUniformLocation(programID, "ViewPosition");
	glUniform3fv(ViewPositionUniformLocation, 1, &MainCamera.getPosition()[0]);

	GLuint AttenuationUniformLocation = glGetUniformLocation(programID, "attenuationAmount");
	glUniform1f(AttenuationUniformLocation, attenuationAmount);

	//Cube
	glBindVertexArray(cubeVertexArrayObjectID);
	TransformMatrix = translate(mat4(), vec3(+3.0f, 0.0f, -5.0f));
	RotationMatrix = rotate(mat4(), 60.0f, vec3(-1.0f, 1.0f, 0.0f));

	FullTransformMatrix = WorldToProjectionMatrix * TransformMatrix * RotationMatrix;
	mat4 CubeModelToWorldMatrix = TransformMatrix * RotationMatrix;

	glUniformMatrix4fv(FullTransformMatrixUniformLocaiton, 1, GL_FALSE, &FullTransformMatrix[0][0]);
	glUniformMatrix4fv(ModelToWorldMatrixUniformLocation, 1, GL_FALSE, &CubeModelToWorldMatrix[0][0]);

	glDrawElements(GL_TRIANGLES, cubeIndices, GL_UNSIGNED_SHORT, (void*)(cubeIndexBufferOffset));

	// Cube
	glBindVertexArray(cubeVertexArrayObjectID);
	TransformMatrix = translate(mat4(), vec3(+4.0f, 2.0f, -1.0f));
	RotationMatrix = rotate(mat4(), 45.0f, vec3(0.0f, 0.0f, 1.0f));

	FullTransformMatrix = WorldToProjectionMatrix * TransformMatrix * RotationMatrix;
	CubeModelToWorldMatrix = TransformMatrix * RotationMatrix;

	glUniformMatrix4fv(FullTransformMatrixUniformLocaiton, 1, GL_FALSE, &FullTransformMatrix[0][0]);
	glUniformMatrix4fv(ModelToWorldMatrixUniformLocation, 1, GL_FALSE, &CubeModelToWorldMatrix[0][0]);

	glDrawElements(GL_TRIANGLES, cubeIndices, GL_UNSIGNED_SHORT, (void*)(cubeIndexBufferOffset));

	//plane
	glUseProgram(shadowProgramID);
	glBindVertexArray(planeVertexArrayObjectID);

	TextureUniformLocation = glGetUniformLocation(shadowProgramID, "tex_temp");
	glUniform1i(TextureUniformLocation, 0);
	//NormalmapUniformLocation = glGetUniformLocation(shadowProgramID, "NormalMap");
	//glUniform1i(NormalmapUniformLocation, 1);

	GLint ShadowMapUniformLocation = glGetUniformLocation(shadowProgramID, "ShadowMap");
	glUniform1i(ShadowMapUniformLocation, 4);

	mat4 LightWorldToProjectionMatrix = projectionMatrix * LightCamera.getWorldToViewMatrix();
	GLint LightFullTransformMatrixUniformLocaiton = glGetUniformLocation(shadowProgramID, "LightFullTransformMatrix");
	glUniformMatrix4fv(LightFullTransformMatrixUniformLocaiton, 1, GL_FALSE, &LightWorldToProjectionMatrix[0][0]);

	TransformMatrix = translate(mat4(), vec3(-0.0f, -2.0f, -5.0f));
	RotationMatrix = rotate(mat4(), 0.0f, vec3(1.0f, 0.0f, 0.0f));

	FullTransformMatrix = WorldToProjectionMatrix * TransformMatrix * RotationMatrix;
	mat4 PlaneModelToWorldMatrix = TransformMatrix * RotationMatrix;

	AmbientLightUniformLocation = glGetUniformLocation(shadowProgramID, "AmbientLight");
	glUniform3fv(AmbientLightUniformLocation, 1, &AmbientLight[0]);

	LightPositionUniformLocation = glGetUniformLocation(shadowProgramID, "LightPosition");
	glUniform3fv(LightPositionUniformLocation, 1, &LightPosition[0]);

	ViewPositionUniformLocation = glGetUniformLocation(shadowProgramID, "ViewPosition");
	glUniform3fv(ViewPositionUniformLocation, 1, &MainCamera.getPosition()[0]);

	AttenuationUniformLocation = glGetUniformLocation(shadowProgramID, "attenuationAmount");
	glUniform1f(AttenuationUniformLocation, attenuationAmount);
	FullTransformMatrixUniformLocaiton = glGetUniformLocation(shadowProgramID, "FullTransformMatrix");
	ModelToWorldMatrixUniformLocation = glGetUniformLocation(shadowProgramID, "ModelToWorldMatrix");
	glUniformMatrix4fv(FullTransformMatrixUniformLocaiton, 1, GL_FALSE, &FullTransformMatrix[0][0]);
	glUniformMatrix4fv(ModelToWorldMatrixUniformLocation, 1, GL_FALSE, &PlaneModelToWorldMatrix[0][0]);

	glDrawElements(GL_TRIANGLES, planeIndices, GL_UNSIGNED_SHORT, (void*)(planeIndexBufferOffset));

	//CubeLight
	glUseProgram(lightProgramID);
	glBindVertexArray(cubeVertexArrayObjectID);
	TransformMatrix = translate(mat4(), LightPosition);
	RotationMatrix = rotate(mat4(), 0.0f, vec3(1.0f, 0.0f, 0.0f));
	ScaleMatrix = scale(mat4(), vec3(0.08f, 0.08f, 0.08f));

	GLuint LightTransformMatrixUniformLocation = glGetUniformLocation(lightProgramID, "LightTransformMatrix");
	FullTransformMatrix = WorldToProjectionMatrix  *  TransformMatrix * ScaleMatrix * RotationMatrix;
	glUniformMatrix4fv(LightTransformMatrixUniformLocation, 1, GL_FALSE, &FullTransformMatrix[0][0]);

	glDrawElements(GL_TRIANGLES, cubeIndices, GL_UNSIGNED_SHORT, (void*)cubeIndexBufferOffset);

	//Cube
	glBindVertexArray(cubeVertexArrayObjectID);
	TransformMatrix = translate(mat4(), vec3(-3.0f, 0.0f, -5.0f));
	RotationMatrix = rotate(mat4(), 45.0f, vec3(0.0f, 1.0f, 0.0f));

	FullTransformMatrix = WorldToProjectionMatrix * TransformMatrix * RotationMatrix;
	CubeModelToWorldMatrix = TransformMatrix * RotationMatrix;

	glUniformMatrix4fv(FullTransformMatrixUniformLocaiton, 1, GL_FALSE, &FullTransformMatrix[0][0]);
	glUniformMatrix4fv(ModelToWorldMatrixUniformLocation, 1, GL_FALSE, &CubeModelToWorldMatrix[0][0]);

	glDrawElements(GL_TRIANGLES, cubeIndices, GL_UNSIGNED_SHORT, (void*)(cubeIndexBufferOffset));

	// Cube
	glBindVertexArray(cubeVertexArrayObjectID);
	TransformMatrix = translate(mat4(), vec3(-6.0f, 0.0f, -2.0f));
	RotationMatrix = rotate(mat4(), 15.0f, vec3(0.0f, 1.0f, -1.0f));
	ScaleMatrix = scale(mat4(), vec3(0.5f));

	FullTransformMatrix = WorldToProjectionMatrix * TransformMatrix * RotationMatrix * ScaleMatrix;
	CubeModelToWorldMatrix = TransformMatrix * RotationMatrix;

	glUniformMatrix4fv(FullTransformMatrixUniformLocaiton, 1, GL_FALSE, &FullTransformMatrix[0][0]);
	glUniformMatrix4fv(ModelToWorldMatrixUniformLocation, 1, GL_FALSE, &CubeModelToWorldMatrix[0][0]);

	glDrawElements(GL_TRIANGLES, cubeIndices, GL_UNSIGNED_SHORT, (void*)(cubeIndexBufferOffset));

	// Cube
	glBindVertexArray(cubeVertexArrayObjectID);
	TransformMatrix = translate(mat4(), vec3(-1.0f, 4.0f, -2.0f));
	RotationMatrix = rotate(mat4(), 70.0f, vec3(1.0f, 1.0f, 0.0f));
	ScaleMatrix = scale(mat4(), vec3(0.8f));

	FullTransformMatrix = WorldToProjectionMatrix * TransformMatrix * RotationMatrix * ScaleMatrix;
	CubeModelToWorldMatrix = TransformMatrix * RotationMatrix;

	glUniformMatrix4fv(FullTransformMatrixUniformLocaiton, 1, GL_FALSE, &FullTransformMatrix[0][0]);
	glUniformMatrix4fv(ModelToWorldMatrixUniformLocation, 1, GL_FALSE, &CubeModelToWorldMatrix[0][0]);

	glDrawElements(GL_TRIANGLES, cubeIndices, GL_UNSIGNED_SHORT, (void*)(cubeIndexBufferOffset));

	// Color changing
	if (plusVariation) {
		colorVariation += vec3(0.0001f, 0.0003f, 0.0005f);
	}
	else {
		colorVariation -= vec3(0.0001f, 0.0003f, 0.0005f);
	}
	if (colorVariation.x >= 1 || colorVariation.y >= 1 || colorVariation.z >= 1) {
		plusVariation = false;
	}
	else if (colorVariation.x <= 0 || colorVariation.y <= 0 || colorVariation.z <= 0) {
		plusVariation = true;
	}
	GLuint colorVariationUniLoc = glGetUniformLocation(lightProgramID, "colorVariation");
	glUniform3fv(colorVariationUniLoc, 1, &colorVariation[0]);

	//CubeMap
	glUseProgram(cubeMapProgramID);

	GLint CubeMapUniformLocation = glGetUniformLocation(cubeMapProgramID, "CubeMap");
	glUniform1i(CubeMapUniformLocation, 2);

	glBindVertexArray(cubeVertexArrayObjectID);
	ScaleMatrix = scale(mat4(), vec3(50.0f, 50.0f, 50.0f));

	GLuint SkyboxTransformMatrixUniformLocation = glGetUniformLocation(cubeMapProgramID, "SkyboxTransformMatrix");
	CameraMatrix[3][0] = 0.0;
	CameraMatrix[3][1] = 0.0;
	CameraMatrix[3][2] = 0.0;
	FullTransformMatrix = projectionMatrix * CameraMatrix * ScaleMatrix;
	glUniformMatrix4fv(SkyboxTransformMatrixUniformLocation, 1, GL_FALSE, &FullTransformMatrix[0][0]);

	glDrawElements(GL_TRIANGLES, cubeIndices, GL_UNSIGNED_SHORT, (void*)cubeIndexBufferOffset);
	
	glUseProgram(testProgramID);

	TextureUniformLocation = glGetUniformLocation(testProgramID, "tex_temp");
	glUniform1i(TextureUniformLocation, 4);
	glBindVertexArray(cubeVertexArrayObjectID);
	CameraMatrix = MainCamera.getWorldToViewMatrix();
	projectionMatrix = perspective(60.0f, ((float)width() / height()), 0.1f, 100.0f);

	WorldToProjectionMatrix = projectionMatrix * CameraMatrix;

	TransformMatrix = translate(mat4(), vec3(0.0f,0.0f,-5.0f));
	RotationMatrix = rotate(mat4(), 0.0f, vec3(1.0f, 0.0f, 0.0f));
	ScaleMatrix = scale(mat4(), vec3(1.0f, 1.0f, 1.0f));

	GLuint FullTransformMatrixUniformLocation = glGetUniformLocation(testProgramID, "FullTransformMatrix");
	FullTransformMatrix = WorldToProjectionMatrix  *  TransformMatrix * ScaleMatrix * RotationMatrix;
	glUniformMatrix4fv(FullTransformMatrixUniformLocation, 1, GL_FALSE, &FullTransformMatrix[0][0]);

	glDrawElements(GL_TRIANGLES, cubeIndices, GL_UNSIGNED_SHORT, (void*)cubeIndexBufferOffset);
}

void MeGlWindow::DrawObjects(Camera & camera){

	mat4 CameraMatrix = camera.getWorldToViewMatrix();
	mat4 projectionMatrix = perspective(60.0f, ((float)width() / height()), 0.3f, 100.0f);

	mat4 WorldToProjectionMatrix = projectionMatrix * CameraMatrix;

	mat4 FullTransformMatrix;

	glUseProgram(programID);

	GLint TextureUniformLocation = glGetUniformLocation(programID, "tex_temp");
	glUniform1i(TextureUniformLocation, 0);
	GLint NormalmapUniformLocation = glGetUniformLocation(programID, "NormalMap");
	glUniform1i(NormalmapUniformLocation, 1);
	
	GLuint FullTransformMatrixUniformLocaiton;
	FullTransformMatrixUniformLocaiton = glGetUniformLocation(programID, "FullTransformMatrix");
	GLuint ModelToWorldMatrixUniformLocation;
	ModelToWorldMatrixUniformLocation = glGetUniformLocation(programID, "ModelToWorldMatrix");
	mat4 TransformMatrix;
	mat4 RotationMatrix;

	//Light Begins Here
	vec3 AmbientLight(0.2f, 0.2f, 0.2f);

	GLuint AmbientLightUniformLocation = glGetUniformLocation(programID, "AmbientLight");
	glUniform3fv(AmbientLightUniformLocation, 1, &AmbientLight[0]);

	GLuint LightPositionUniformLocation = glGetUniformLocation(programID, "LightPosition");
	glUniform3fv(LightPositionUniformLocation, 1, &LightPosition[0]);

	GLuint ViewPositionUniformLocation = glGetUniformLocation(programID, "ViewPosition");
	glUniform3fv(ViewPositionUniformLocation, 1, &camera.getPosition()[0]);

	GLuint AttenuationUniformLocation = glGetUniformLocation(programID, "attenuationAmount");
	glUniform1f(AttenuationUniformLocation,attenuationAmount);

	//Cube
	glBindVertexArray(cubeVertexArrayObjectID);
	TransformMatrix = translate(mat4(), vec3(-3.0f, 0.0f, -5.0f));
	RotationMatrix = rotate(mat4(), 66.0f, vec3(1.0f, -1.0f, 0.0f));

	FullTransformMatrix = WorldToProjectionMatrix * TransformMatrix * RotationMatrix;
	mat4 CubeModelToWorldMatrix = TransformMatrix * RotationMatrix;

	glUniformMatrix4fv(FullTransformMatrixUniformLocaiton, 1, GL_FALSE, &FullTransformMatrix[0][0]);
	glUniformMatrix4fv(ModelToWorldMatrixUniformLocation, 1, GL_FALSE, &CubeModelToWorldMatrix[0][0]);

	glDrawElements(GL_TRIANGLES, cubeIndices, GL_UNSIGNED_SHORT, (void*)(cubeIndexBufferOffset));

	// Cube
	glBindVertexArray(cubeVertexArrayObjectID);
	TransformMatrix = translate(mat4(), vec3(+4.0f, 2.0f, -1.0f));
	RotationMatrix = rotate(mat4(), 45.0f, vec3(0.0f, 0.0f, 1.0f));

	FullTransformMatrix = WorldToProjectionMatrix * TransformMatrix * RotationMatrix;
	CubeModelToWorldMatrix = TransformMatrix * RotationMatrix;

	glUniformMatrix4fv(FullTransformMatrixUniformLocaiton, 1, GL_FALSE, &FullTransformMatrix[0][0]);
	glUniformMatrix4fv(ModelToWorldMatrixUniformLocation, 1, GL_FALSE, &CubeModelToWorldMatrix[0][0]);

	glDrawElements(GL_TRIANGLES, cubeIndices, GL_UNSIGNED_SHORT, (void*)(cubeIndexBufferOffset));

	//plane
	glBindVertexArray(planeVertexArrayObjectID);
	TransformMatrix = translate(mat4(), vec3(-0.0f, -2.0f, -5.0f));
	RotationMatrix = rotate(mat4(), 0.0f, vec3(1.0f, 0.0f, 0.0f));

	FullTransformMatrix = WorldToProjectionMatrix * TransformMatrix * RotationMatrix;
	mat4 PlaneModelToWorldMatrix = TransformMatrix * RotationMatrix;

	glUniformMatrix4fv(FullTransformMatrixUniformLocaiton, 1, GL_FALSE, &FullTransformMatrix[0][0]);
	glUniformMatrix4fv(ModelToWorldMatrixUniformLocation, 1, GL_FALSE, &PlaneModelToWorldMatrix[0][0]);

	glDrawElements(GL_TRIANGLES, planeIndices, GL_UNSIGNED_SHORT, (void*)(planeIndexBufferOffset));

	//CubeLight
	glUseProgram(lightProgramID);
	glBindVertexArray(cubeVertexArrayObjectID);
	TransformMatrix = translate(mat4(),LightPosition);
	RotationMatrix = rotate(mat4(), 0.0f, vec3(1.0f, 0.0f, 0.0f));
	mat4 ScaleMatrix = scale(mat4(), vec3(0.08f, 0.08f, 0.08f));

	GLuint LightTransformMatrixUniformLocation = glGetUniformLocation(lightProgramID, "LightTransformMatrix");
	FullTransformMatrix = WorldToProjectionMatrix  *  TransformMatrix * ScaleMatrix * RotationMatrix;
	glUniformMatrix4fv(LightTransformMatrixUniformLocation, 1, GL_FALSE, &FullTransformMatrix[0][0]);

	glDrawElements(GL_TRIANGLES, cubeIndices, GL_UNSIGNED_SHORT, (void*)cubeIndexBufferOffset);

	//Cube
	glBindVertexArray(cubeVertexArrayObjectID);
	TransformMatrix = translate(mat4(), vec3(+3.0f, 0.0f, -5.0f));
	RotationMatrix = rotate(mat4(), RotationAngle, vec3(0.0f, 1.0f, 0.0f));

	FullTransformMatrix = WorldToProjectionMatrix * TransformMatrix * RotationMatrix;
	mat4 Cube1ModelToWorldMatrix = TransformMatrix * RotationMatrix;

	glUniformMatrix4fv(FullTransformMatrixUniformLocaiton, 1, GL_FALSE, &FullTransformMatrix[0][0]);
	glUniformMatrix4fv(ModelToWorldMatrixUniformLocation, 1, GL_FALSE, &Cube1ModelToWorldMatrix[0][0]);

	glDrawElements(GL_TRIANGLES, cubeIndices, GL_UNSIGNED_SHORT, (void*)(cubeIndexBufferOffset));

	// Cube
	glBindVertexArray(cubeVertexArrayObjectID);
	TransformMatrix = translate(mat4(), vec3(-6.0f, 0.0f, -2.0f));
	RotationMatrix = rotate(mat4(), 15.0f, vec3(0.0f, 1.0f, -1.0f));
	ScaleMatrix = scale(mat4(), vec3(0.5f));

	FullTransformMatrix = WorldToProjectionMatrix * TransformMatrix * RotationMatrix * ScaleMatrix;
	CubeModelToWorldMatrix = TransformMatrix * RotationMatrix;

	glUniformMatrix4fv(FullTransformMatrixUniformLocaiton, 1, GL_FALSE, &FullTransformMatrix[0][0]);
	glUniformMatrix4fv(ModelToWorldMatrixUniformLocation, 1, GL_FALSE, &CubeModelToWorldMatrix[0][0]);

	glDrawElements(GL_TRIANGLES, cubeIndices, GL_UNSIGNED_SHORT, (void*)(cubeIndexBufferOffset));

	// Cube
	glBindVertexArray(cubeVertexArrayObjectID);
	TransformMatrix = translate(mat4(), vec3(-1.0f, 4.0f, -2.0f));
	RotationMatrix = rotate(mat4(), 70.0f, vec3(1.0f, 1.0f, 0.0f));
	ScaleMatrix = scale(mat4(), vec3(0.8f));

	FullTransformMatrix = WorldToProjectionMatrix * TransformMatrix * RotationMatrix * ScaleMatrix;
	CubeModelToWorldMatrix = TransformMatrix * RotationMatrix;

	glUniformMatrix4fv(FullTransformMatrixUniformLocaiton, 1, GL_FALSE, &FullTransformMatrix[0][0]);
	glUniformMatrix4fv(ModelToWorldMatrixUniformLocation, 1, GL_FALSE, &CubeModelToWorldMatrix[0][0]);

	glDrawElements(GL_TRIANGLES, cubeIndices, GL_UNSIGNED_SHORT, (void*)(cubeIndexBufferOffset));

	//CubeMap
	glUseProgram(cubeMapProgramID);

	GLint CubeMapUniformLocation = glGetUniformLocation(cubeMapProgramID, "CubeMap");
	glUniform1i(CubeMapUniformLocation, 2);

	glBindVertexArray(cubeVertexArrayObjectID);
	ScaleMatrix = scale(mat4(), vec3(50.0f, 50.0f, 50.0f));

	GLuint SkyboxTransformMatrixUniformLocation = glGetUniformLocation(cubeMapProgramID, "SkyboxTransformMatrix");
	CameraMatrix[3][0] = 0.0;
	CameraMatrix[3][1] = 0.0;
	CameraMatrix[3][2] = 0.0;
	FullTransformMatrix = projectionMatrix * CameraMatrix * ScaleMatrix;
	glUniformMatrix4fv(SkyboxTransformMatrixUniformLocation, 1, GL_FALSE, &FullTransformMatrix[0][0]);

	glDrawElements(GL_TRIANGLES, cubeIndices, GL_UNSIGNED_SHORT, (void*)cubeIndexBufferOffset);
}

void MeGlWindow::mouseMoveEvent(QMouseEvent* e)
{
	setFocus();
	MainCamera.mouseUpdate(vec2(e->x(), e->y()));
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
		MainCamera.move_forward();
		break;
	case Qt::Key::Key_S:
		MainCamera.move_backward();
		break;
	case Qt::Key::Key_A:
		MainCamera.move_leftward();
		break;
	case Qt::Key::Key_D:
		MainCamera.move_rightward();
		break;
	case Qt::Key::Key_R:
		MainCamera.move_upward();
		break;
	case Qt::Key::Key_F:
		MainCamera.move_downward();
		break;
	case Qt::Key::Key_Q:
		MainCamera.rotate_left();
		break;
	case Qt::Key::Key_E:
		MainCamera.rotate_right();
		break;
	case Qt::Key::Key_Z:
		MainCamera.rotate_up();
		break;
	case Qt::Key::Key_C:
		MainCamera.rotate_down();
		break;
	case Qt::Key::Key_T:
		LightCamera.rotate_left();
		break;
	case Qt::Key::Key_Y:
		LightCamera.rotate_right();
		break;
	case Qt::Key::Key_G:
		LightCamera.rotate_up();
		break;
	case Qt::Key::Key_H:
		LightCamera.rotate_down();
		break;
	case Qt::Key::Key_I:
		LightPosition += vec3 (0,0,-0.2);
		break;
	case Qt::Key::Key_K:
		LightPosition += vec3(0, 0, 0.2);
		break;
	case Qt::Key::Key_J:
		LightPosition += vec3(-0.2, 0, -0.0);
		break;
	case Qt::Key::Key_L:
		LightPosition += vec3(0.2, 0, -0.0);
		break;
	case Qt::Key::Key_U:
		LightPosition += vec3(0, 0.2, -0.0);
		break;
	case Qt::Key::Key_O:
		LightPosition += vec3(0, -0.2, -0.0);
	}
	repaint();
}
#include <gl\glew.h>
#include <glm\glm.hpp>
#include <glm\gtx\transform.hpp>
#include <MeGlWindow.h>
#include <Vertex.h>
#include <iostream>
#include <fstream>

using namespace std;

extern const char* vertexShaderCode;
extern const char* fragmentShaderCode;

const uint NUM_VERTICES_PER_TRI = 3;
const uint NUM_FLOATS_PER_VERTICE = 6;
const uint VERTEX_BYTE_SIZE = NUM_FLOATS_PER_VERTICE * sizeof(float);

void sendContent()
{
	/*static GLfloat vertexPos[] = {
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
	};*/

	// Vertex positions on the screen
	Vertex triPos[] = {
		glm::vec3(+0.0f, +0.0f, +0.0f),
		glm::vec3(+1.0f, +0.0f, +0.0f),

		glm::vec3(-1.0f, +1.0f, +0.0f),
		glm::vec3(+0.0f, +1.0f, +0.0f),
		
		glm::vec3(-1.0f, -1.0f, +0.0f),
		glm::vec3(+0.0f, +0.0f, +1.0f),

		glm::vec3(+0.75f, -0.75f, +0.0f),
		glm::vec3(+1.0f, +0.0f, +1.0f),

		glm::vec3(+0.75f, +0.75f, +0.0f),
		glm::vec3(+0.0f, +1.0f, +1.0f),
	};

	// Order of drawing
	static GLushort indices[] = {
		0, 1, 2,
	//	0, 3, 4
	};

	glm::mat3 transform = glm::mat3(glm::translate(0.5f, 0.0f, 0.0f));

	GLuint bufferID;
	glGenBuffers(1, &bufferID);
	glBindBuffer(GL_ARRAY_BUFFER, bufferID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufferID);

	glBufferData(GL_ARRAY_BUFFER, sizeof(triPos) + sizeof(indices), 0, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(triPos), triPos);
	glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, sizeof(triPos), sizeof(indices), indices);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 6, 0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, (char*)(sizeof(float) * 3));
}

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
	//adapter[0] = vertexShaderCode;
	glShaderSource(vertexShaderID, 1, adapter, 0);
	temp = readShader("FragmentShader.glsl");
	adapter[0] = temp.c_str();
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
	sendContent();
	installShaders();
}

void MeGlWindow::paintGL()
{
	glClear(GL_COLOR_BUFFER_BIT);
	glViewport(0, 0, width(), height());
	// (void*) number of vertex position floats * 4
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, (void*)120);
}


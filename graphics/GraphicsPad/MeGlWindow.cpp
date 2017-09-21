#include <gl\glew.h>
#include <glm\glm.hpp>
#include <glm\gtx\transform.hpp>
#include <MeGlWindow.h>
#include <Vertex.h>
#include <iostream>
#include <fstream>

using namespace std;

const uint NUM_VERTICES_PER_TRI = 3;
const uint NUM_FLOATS_PER_VERTICE = 6;
const uint VERTEX_BYTE_SIZE = NUM_FLOATS_PER_VERTICE * sizeof(float);
GLuint programID;

glm::vec3 position;
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
	// Vertex positions on the screen
	Vertex triPos[] = {
		glm::vec3(+0.0f, +1.0f, +1.0f),
		glm::vec3(+1.0f, +0.0f, +0.0f),

		glm::vec3(-1.0f, -1.0f, +1.0f),
		glm::vec3(+0.0f, +1.0f, +0.0f),
		
		glm::vec3(+1.0f, -1.0, +1.0f),
		glm::vec3(+0.0f, +0.0f, +1.0f),
	};

	// Order of drawing
	static GLushort indices[] = {
		0, 1, 2,
	};
	
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

void transformShape()
{	
	glm::mat4 rotate = glm::rotate(angle, 0.0f, 0.0f, 1.0f);
	glm::mat4 translate = glm::translate(position);
	glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(scaling));

	glm::mat4 transform = rotate * translate * scale;
	GLint uniformLoc = glGetUniformLocation(programID, "transform");
	glUniformMatrix4fv(uniformLoc, 1, GL_FALSE, &transform[0][0]);
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

void MeGlWindow::initializeGL()
{
	glewInit();
	glClearColor(0, 0, 0, 1);
	installShaders();
	sendContent();
}

void MeGlWindow::paintGL()
{
	glClear(GL_COLOR_BUFFER_BIT);
	glViewport(0, 0, width(), height());
	transformShape();
	// (void*) number of vertex position floats * 4
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, (void*)72);
}


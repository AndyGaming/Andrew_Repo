#ifndef ME_GL_WINDOW
#define ME_GL_WINDOW
#include <QtOpenGL\qglwidget>
#include <QtGui\qmouseevent>
#include <QtGui\qkeyevent>

class MeGlWindow : public QGLWidget
{
	void sendContent();
	std::string readShader(const char* fileName);
	void installShaders();
	void initTextures();
protected:
	void initializeGL();
	void paintGL();
	void keyPressEvent(QKeyEvent* e);
	void mouseMoveEvent(QMouseEvent*);

public:
	~MeGlWindow();

};

#endif

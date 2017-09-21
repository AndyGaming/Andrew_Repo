#ifndef ME_GL_WINDOW
#define ME_GL_WINDOW
#include <QtOpenGL\qglwidget>
#include <QtGui\qmouseevent>
#include <QtGui\qkeyevent>

class MeGlWindow : public QGLWidget
{
protected:
	void initializeGL();
	void paintGL();
	void keyPressEvent(QKeyEvent* e);

public:
};

#endif

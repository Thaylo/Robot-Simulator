#ifndef WINDOW_H_
#define WINDOW_H_

#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <GL/glx.h>
#include <GL/gl.h>
#include <GL/glu.h>

using namespace std;

class window
{
private:
	Display* g_pDisplay;
	Window g_window;
	int g_bDoubleBuffered;
    int width, height;
	XSetWindowAttributes windowAttributes;
	XVisualInfo *visualInfo = NULL;
	Colormap colorMap;
	GLXContext glxContext;
	int errorBase;
	int eventBase;

	bool checkForGLXSupport();
	bool openXServerConnection();
	bool tryDoubleBufferedVisual();
	bool trySingleBufferedVisual();

	void createOpenGLRenderingContext();
	void createXColorMap();

public:
	window(int, int);
	void initialize();
	void open();
	void update();
	~window(){}
	bool process(void (*mouseFunc)(int type, int button, int x, int y),
	                  void (*keyPress)(int code), void (*keyRelease)(int code));
};

window* initWindow();
void showWindow(window* w);

#endif

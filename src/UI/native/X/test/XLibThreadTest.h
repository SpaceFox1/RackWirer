#ifndef XLIBTHREADTEST_H
#define XLIBTHREADTEST_H

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

class XLibThreadTest {
  private:
    Display * display;
    GC gc;
    XFontStruct *font_info;
    int screen;

  public:
    bool running;
    Window window;
    void drawSpecialMenuClickEventCallback();

    XLibThreadTest();
    int start();
    int stop();
    void loop();
};

#endif

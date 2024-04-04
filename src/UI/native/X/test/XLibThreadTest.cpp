#include "./XLibThreadTest.h"

XLibThreadTest::XLibThreadTest() {
  this->running = true;
}

int XLibThreadTest::start() {
  this->display = XOpenDisplay(NULL);
  if (NULL == this->display) {
    fprintf(stderr, "Failed to initialize display\n");
    return EXIT_FAILURE;
  }

  Window root = DefaultRootWindow(this->display);
  if (None == root) {
    fprintf(stderr, "No root window found\n");
    XCloseDisplay(this->display);
    return EXIT_FAILURE;
  }

  this->window = XCreateSimpleWindow(this->display, root, 0, 0, 800, 600, 0, 0, 0xffffffff);
  if (None == this->window) {
    fprintf(stderr, "Failed to create window\n");
    XCloseDisplay(this->display);
    return EXIT_FAILURE;
  }

  // set application root name
  XClassHint * hint = XAllocClassHint();
  hint->res_name = (char*)"rackwirer";
  hint->res_class = (char*)"rackwirer";
  XSetClassHint(this->display, this->window, hint);
  XFree(hint);

  XStoreName(this->display, this->window, "Minha janela");

  XMapWindow(this->display, this->window);

  fprintf(stderr, "Window id: %lu\n", this->window);

  this->screen = DefaultScreen(this->display);

  fprintf(stderr, "Screen: %d\n", this->screen);

  this->gc = XCreateGC(this->display, this->window, 0, 0);
  this->font_info = XLoadQueryFont(this->display, "*");
  if (this->font_info == NULL) {
    fprintf(stderr, "XLoadQueryFont: failed loading font 'FreeSans'\n");
    exit(1);
  }
  XSetFont(this->display, this->gc, this->font_info->fid);

  this->loop();
  return 0;
}

int XLibThreadTest::stop() {
  this->running = false;
  return 0;
}

void XLibThreadTest::drawSpecialMenuClickEventCallback() {
  fprintf(stderr, "DrawSpecialMenuClickEventCallback\n");

  XSetForeground(this->display, this->gc, WhitePixel(this->display, this->screen));

  // get the size of the text
  XCharStruct text_structure;
  int direction, ascent, descent;
  XTextExtents(font_info, "Hello World", strlen("Hello World"), &direction, &ascent, &descent, &text_structure);

  // get the size of the window
  XWindowAttributes window_attributes;
  XGetWindowAttributes(this->display, this->window, &window_attributes);

  // calculate the position of the text
  int x = (window_attributes.width - text_structure.width) / 2;
  int y = (window_attributes.height - (text_structure.ascent + text_structure.descent)) / 2;

  // set text color to black
  XSetForeground(this->display, gc, BlackPixel(this->display, this->screen));
  XSetBackground(this->display, gc, WhitePixel(this->display, this->screen));

  // draw the text
  XDrawString(this->display, this->window, this->gc, x, y, "Hello World", strlen("Hello World"));

  // draw a pink cube pixel by pixel
  // set the color to pink
  XSetForeground(this->display, this->gc, 0xFF00FF);

  int windowWidth = window_attributes.width;
  int windowHeight = window_attributes.height;

  // draw the cube
  XFillRectangle(this->display, this->window, this->gc, 0, 0, windowWidth / 3, windowHeight / 3);

  XFlush(this->display);
}

void XLibThreadTest::loop() {
  XEvent event;
  Atom wm_delete_window = XInternAtom(this->display, "WM_DELETE_WINDOW", False);
  XSetWMProtocols(this->display, this->window, & wm_delete_window, 1);
  while (running) {
    XNextEvent(this->display, &event);
    switch(event.type) {
      case ClientMessage:
        if(event.xclient.data.l[0] == (long) wm_delete_window) {
          XDestroyWindow(event.xclient.display, event.xclient.window);
          XFreeGC(this->display, this->gc);
          XFreeFont(this->display, this->font_info);
          XCloseDisplay(this->display);
          fprintf(stderr, "Exiting...\n");
          this->running = false;
          // exit(0);
        }
      break;
    }
  }
}

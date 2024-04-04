#ifndef PIPEWIRETHREADTEST_H
#define PIPEWIRETHREADTEST_H

#include <pipewire/pipewire.h>

class PipeWireThreadTest {
  private:
    struct pw_main_loop *loop;

  public:
    PipeWireThreadTest();
    int start(int *argc, char ***argv);
    int stop();
};

#endif // PIPEWIRETHREADTEST_H

#ifndef ACTIONEMITTER_H
#define ACTIONEMITTER_H

#include <functional>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

class ActionEmitter {
  private:
    struct EventList {
      int* id;
      const char* event;
      std::function<void (void *)> *callback;
      struct EventList* next;
    };

    struct EventList* eventList;

  public:
    ActionEmitter();

    int addEventListener(const char* event, std::function<void (void *)> callback);
    void removeEventListener(int id);

    void emit(const char* event, void* data);
};

#endif // ACTIONEMITTER_H

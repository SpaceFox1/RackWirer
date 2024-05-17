#ifndef ACTIONEMITTER_H
#define ACTIONEMITTER_H

#include <functional>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <memory>

typedef std::shared_ptr<std::function<void (void *)>> ActionCallback;
typedef std::function<void (void *)> ActionCallbackFunc;

class ActionEmitter {
  private:
    struct EventList {
      int* id;
      const char* event;
      ActionCallback callback;
      struct EventList* next;
    };

    struct EventList* eventList;

  public:
    ActionEmitter();

    int addEventListener(const char* event, ActionCallback callback);
    void removeEventListener(int id);

    void emit(const char* event, void* data);
};

#endif // ACTIONEMITTER_H

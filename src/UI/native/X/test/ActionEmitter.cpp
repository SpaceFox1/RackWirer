#include "./ActionEmitter.h"

ActionEmitter::ActionEmitter() {
  this->eventList = nullptr;
}

int ActionEmitter::addEventListener(const char* event, std::function<void (void *)> callback) {
  struct EventList* newCallback = (struct EventList*) malloc(sizeof(struct EventList));
  newCallback->event = event;
  newCallback->callback = &callback;
  newCallback->next = nullptr;
  int id = 0;
  if (this->eventList == nullptr) this->eventList = newCallback;
  else {
    struct EventList* current = this->eventList;
    while (current->next != nullptr) {
      current = current->next;
    }
    id = (*current->id) + 1;
    current->next = newCallback;
  }
  newCallback->id = &id;

  return id;
}

void ActionEmitter::removeEventListener(int id) {
  struct EventList* current = this->eventList;
  struct EventList* previous = nullptr;
  while (current != nullptr) {
    if ((*current->id) == id) {
      if (previous == nullptr) this->eventList = current->next;
      else previous->next = current->next;
      free(current);
      break;
    }
    previous = current;
    current = current->next;
  }
}

void ActionEmitter::emit(const char* event, void* data) {
  struct EventList* current = this->eventList;
  while (current != nullptr) {
    if (strcmp(current->event, event) == 0) {
      (*current->callback)(data);
    }
    current = current->next;
  }
}

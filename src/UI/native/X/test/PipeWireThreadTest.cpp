#include "./PipeWireThreadTest.h"

static void registry_event_global(void *, uint32_t id, uint32_t, const char *type, uint32_t version, const struct spa_dict *) {
  printf("object: id:%u type:%s/%d\n", id, type, version);
}

static const struct pw_registry_events registry_events = {
  PW_VERSION_REGISTRY_EVENTS,
  .global = registry_event_global,
};

PipeWireThreadTest::PipeWireThreadTest() {
}

int PipeWireThreadTest::start(int *argc, char ***argv) {
  struct pw_context *context;
  struct pw_core *core;
  struct pw_registry *registry;
  struct spa_hook registry_listener;

  pw_init(argc, argv);

  this->loop = pw_main_loop_new(NULL);
  context = pw_context_new(pw_main_loop_get_loop(this->loop), NULL, 0);

  core = pw_context_connect(context, NULL, 0);

  registry = pw_core_get_registry(core, PW_VERSION_REGISTRY, 0);

  spa_zero(registry_listener);
  pw_registry_add_listener(registry, &registry_listener, &registry_events, NULL);

  pw_main_loop_run(this->loop);

  pw_proxy_destroy((struct pw_proxy*)registry);
  pw_core_disconnect(core);
  pw_context_destroy(context);
  pw_main_loop_destroy(this->loop);

  return 0;
}

int PipeWireThreadTest::stop() {
  pw_main_loop_quit(this->loop);

  return 0;
}

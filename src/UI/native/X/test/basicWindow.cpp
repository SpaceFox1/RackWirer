#include <thread>
#include <functional>
#include "./ActionEmitter.h"
#include "./dbus/DbusThreadTest.h"
#include "./PipeWireThreadTest.h"
#include "./XLibThreadTest.h"

int main(int argc, char **argv) {
  ActionEmitter ae;
  XLibThreadTest xlt;
  PipeWireThreadTest pwt;
  DbusThreadTest dbt(&ae);

  std::thread xlibThread(&XLibThreadTest::start, &xlt);
  std::thread pipewireThread(&PipeWireThreadTest::start, &pwt, &argc, &argv);
  std::thread dbusThread(&DbusThreadTest::start, &dbt, &xlt);

  ae.addEventListener("buttonClick", std::make_shared<ActionCallbackFunc>([&](void*) {
    xlt.drawSpecialMenuClickEventCallback();
  }));

  while (xlt.running) std::this_thread::sleep_for(std::chrono::milliseconds(1000));

  dbt.stop();
  pwt.stop();
  xlt.stop();

  xlibThread.join();
  pipewireThread.join();
  dbusThread.join();

  return 0;
}


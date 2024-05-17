#ifndef __DBUSTHREADTEST_H__
#define __DBUSTHREADTEST_H__

#include <dbus/dbus.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <functional>
#include <thread>
#include "./DbusHandler/DbusHandler.h"
#include "../ActionEmitter.h"
#include "../XLibThreadTest.h"

class DbusThreadTest {
  private:
    DBusHandlerResult handleIncommingMessage(DBusMessage* message);
    void handleMenuMessage(DBusMessage* message, DBusMessage* reply);
    void handleFreeDesktopMessage(DBusMessage* message, DBusMessage* reply);
    void handleOwnMethodsMessage(DBusMessage* message, DBusMessage* reply);

    DBusHandlerResult handleResponseCleanUp(DBusMessage* message, DBusMessage *reply, DBusError err);

    void DbusLoop();

    ActionEmitter* ae;
    DbusHandler handler;

  public:
    bool isRunning;
    DBusConnection* myDBusConnection;
    DBusError myDBusError;

    const char * uniqName;

    DbusThreadTest(ActionEmitter* ae);
    int start(XLibThreadTest* xlt);

    int stop();
};

#endif // __DBUSTHREADTEST_H__

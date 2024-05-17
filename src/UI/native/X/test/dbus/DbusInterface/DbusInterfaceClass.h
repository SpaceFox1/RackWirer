#ifndef DBUS_INTERFACE_CLASS_H
#define DBUS_INTERFACE_CLASS_H

#include <string>
#include <vector>
#include <functional>
#include "../DbusMethod/DbusMethodClass.h"
#include "../DbusProperty/DbusPropertyClass.h"

class DBUSInterface {
  private:
    std::string name;
    std::vector<DBUSMethod> methods;
    std::vector<DBUSProperty> properties;
  public:
    DBUSInterface(std::string name);
    void appendMethod(DBUSMethod method);
    void appendProperty(DBUSProperty property);

    std::string getName();
    DBUSMethod* getMethod(std::string methodName);

    std::vector<DBUSProperty> getProperties();
    std::vector<DBUSMethod> getMethods();
};

#endif // DBUS_INTERFACE_CLASS_H

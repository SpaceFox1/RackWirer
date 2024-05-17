#ifndef DBUS_NODE_CLASS_H
#define DBUS_NODE_CLASS_H

#include <string>
#include <vector>
#include "../DbusInterface/DbusInterfaceClass.h"

class DbusNode {
  private:
    bool isRoot;
    std::string name;
    std::vector<DBUSInterface> interfaces;
    std::vector<DbusNode> children;
    int indentationStep = 2;
  public:
    DbusNode(std::string name, bool isRoot);
    void appendInterface(DBUSInterface interface);
    void appendChild(DbusNode child);

    std::string getName();
    DBUSInterface* getInterface(std::string interfaceName);
    std::vector<DBUSInterface> getInterfaces();
    DbusNode* getChild(std::string nodeName);
    std::vector<DbusNode> getChildren();
    std::string introspectNode(int indentationLevel);
};

#endif // DBUS_NODE_CLASS_H

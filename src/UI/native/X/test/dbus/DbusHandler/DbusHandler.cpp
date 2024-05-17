#include "./DbusHandler.h"

// Constructor
DbusHandler::DbusHandler(std::string rootNodeName) : root(DbusNode(rootNodeName, true)) {
  // implement default freeDesktop interfaces

  DBUSInterface introspectable = DBUSInterface("org.freedesktop.DBus.Introspectable");
  DBUSMethod introspect = DBUSMethod("Introspect", DBUSMethodArgument("data", DbusTypeSignature(DBUSPrimitiveTypes::DString)), std::make_shared<DbusMethodCallbackFunc>([&](DBusMessage* message) {
    DBusMessage* reply = NULL;
    if (!(reply = dbus_message_new_method_return(message))) return reply;

    std::string introspectionXml = this->root.introspectNode(0);

    dbus_message_append_args(reply, DBUS_TYPE_STRING, introspectionXml, DBUS_TYPE_INVALID);

    return reply;
  }));
  introspectable.appendMethod(introspect);

  std::vector<DBUSMethodArgument> args = std::vector<DBUSMethodArgument>();
  // DBUSStruct testArgs;

  DBUSPString stringe = DBUSPString("");
  DBUSPInt32 inte = DBUSPInt32(0);
  args.push_back(DBUSMethodArgument("TestString", stringe));
  args.push_back(DBUSMethodArgument("TestNumber", inte));

  // args.push_back(DBUSMethodArgument("testInput", DbusTypeSignature(testArgs)));

  DBUSMethod test = DBUSMethod("Test", args, DBUSMethodArgument("testOutput", DbusTypeSignature(DBUSPrimitiveTypes::DBoolean)), std::make_shared<DbusMethodCallbackFunc>([&](DBusMessage* message) {
    DBusMessage* reply = NULL;
    if (!(reply = dbus_message_new_method_return(message))) return reply;

    //print message
    DBusMessageIter iter;
    dbus_message_iter_init(message, &iter);
    dbus_message_iter_get_basic(&iter, &iter);

    //print arguments
    dbus_message_iter_next(&iter);
    dbus_message_iter_get_basic(&iter, &iter);

  


    dbus_message_append_args(reply, DBUS_TYPE_BOOLEAN, true, DBUS_TYPE_INVALID);

    return reply;
  }));
  introspectable.appendMethod(test);

  this->root.appendInterface(introspectable);
}

DbusNode* DbusHandler::getNode(std::string nodeName) {
  // e.g. nodeName = "/menu"
  // / is root, the first node, so if nodeName is just / return root
  // menu is a child of root, so if nodeName is /menu return root->getChild("menu")
  // /menu/app is a child of menu, so if nodeName is /menu/app return root->getChild("menu")->getChild("app")

  // split nodeName by /
  if (nodeName == "/") return &this->root;

  std::vector<std::string> nodeNames;
  std::string nodeNameCopy = nodeName;
  size_t pos = 0;
  std::string token;
  while ((pos = nodeNameCopy.find("/")) != std::string::npos) {
    token = nodeNameCopy.substr(0, pos);
    nodeNames.push_back(token);
    nodeNameCopy.erase(0, pos + 1);
  }
  nodeNames.push_back(nodeNameCopy);
  
  DbusNode* currentNode = &this->root;
  for (std::string name : nodeNames) {
    currentNode = currentNode->getChild(name);
    if (currentNode == nullptr) break;
  }
  return currentNode;
}


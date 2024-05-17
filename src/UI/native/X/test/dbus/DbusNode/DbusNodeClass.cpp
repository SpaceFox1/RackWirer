#include "./DbusNodeClass.h"

DbusNode::DbusNode(std::string name, bool isRoot) {
  this->name = name;
  this->isRoot = isRoot;
}

std::string DbusNode::getName() {
  return this->name;
}

void DbusNode::appendInterface(DBUSInterface interface) {
  this->interfaces.push_back(interface);
}

void DbusNode::appendChild(DbusNode child) {
  this->children.push_back(child);
}

DbusNode* DbusNode::getChild(std::string nodeName) {
  for (DbusNode& child : this->children) {
    if (child.name == nodeName) return &child;
  }
  return nullptr;
}

DBUSInterface* DbusNode::getInterface(std::string interfaceName) {
  for (DBUSInterface& interface : this->interfaces) {
    if (interface.getName() == interfaceName) return &interface;
  }
  return nullptr;
}

std::vector<DBUSInterface> DbusNode::getInterfaces() {
  return this->interfaces;
}

std::vector<DbusNode> DbusNode::getChildren() {
  return this->children;
}

// return an string containing a xml of all the dbus hierarchy
std::string DbusNode::introspectNode(int indentationLevel) {
  int identLevel = indentationLevel;
  std::string introspection;
  introspection.append(std::string(identLevel*this->indentationStep, ' '));
  if (this->isRoot) {
    introspection.append("<node>\n");
  } else {
    introspection.append("<node name=\"");
    introspection.append(this->name);
    introspection.append("\">\n");
  }
  identLevel++;

 // iterate over all interfaces
  std::vector<DBUSInterface> interfaces = this->interfaces;

  for (DBUSInterface& i : interfaces) {
    introspection.append(std::string(identLevel*this->indentationStep, ' '));
    introspection.append("<interface name=\"");
    introspection.append(i.getName());
    introspection.append("\">\n");
    identLevel++;

   // add all properties of interface
    std::vector<DBUSProperty> props = i.getProperties();
    for (DBUSProperty& p : props) {
      introspection.append(std::string(identLevel*this->indentationStep, ' '));
      introspection.append("<property name=\"");
      introspection.append(p.getName());
      introspection.append("\" type=\"");
      introspection.append(p.getType().signature);
      introspection.append("\" access=\"");
      DBUSPropertyAccessTypes accessType = p.getAccessType();
      if (accessType == DBUSPropertyAccessTypes::read) {
        introspection.append("read");
      }
      introspection.append("\"/>\n");
    }

   // add all methods
    std::vector<DBUSMethod> methods = i.getMethods();
    for (DBUSMethod& m : methods) {
      introspection.append(std::string(indentationLevel*this->indentationStep, ' '));
      introspection.append("<method name=\"");
      introspection.append(m.getName());
      introspection.append("\">\n");
      identLevel++;

      std::vector<DBUSMethodArgument> args = m.getArguments();
      for (DBUSMethodArgument& a : args) {
        introspection.append(std::string(indentationLevel*this->indentationStep, ' '));
        introspection.append("<arg name=\"");
        introspection.append(a.name);
        introspection.append("\" type=\"");
        introspection.append(a.type.signature);
        introspection.append("\" direction=\"in\"/>\n");
      }

      DBUSMethodArgument returnValue = m.getReturnArgument();
      introspection.append(std::string(indentationLevel*this->indentationStep, ' '));
      introspection.append("<arg name=\"");
      introspection.append(returnValue.name);
      introspection.append("\" type=\"");
      introspection.append(returnValue.type.signature);
      introspection.append("\" direction=\"out\"/>\n");

      identLevel--;
      introspection.append(std::string(indentationLevel*this->indentationStep, ' '));
      introspection.append("</method>\n");
    }
    identLevel--;
    introspection.append(std::string(indentationLevel*this->indentationStep, ' '));
    introspection.append("</interface>\n");
  }

 // add all sub nodes
  std::vector<DbusNode> subnodes = this->children;
  for (DbusNode& node : subnodes) {
    introspection.append(std::string(indentationLevel*this->indentationStep, ' '));
    introspection.append("<node name=\"");
    introspection.append(node.getName());
    introspection.append("\"></node>\n");
  }

  identLevel--;
  introspection.append(std::string(indentationLevel*this->indentationStep, ' '));
  introspection.append("</node>\n");
  return introspection;
}

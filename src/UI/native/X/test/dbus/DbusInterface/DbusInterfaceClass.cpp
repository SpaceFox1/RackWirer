#include "./DbusInterfaceClass.h"

DBUSInterface::DBUSInterface(std::string name) {
  this->name = name;
}

void DBUSInterface::appendMethod(DBUSMethod method) {
  this->methods.push_back(method);
}

void DBUSInterface::appendProperty(DBUSProperty property) {
  this->properties.push_back(property);
}

std::string DBUSInterface::getName() {
  return this->name;
}

DBUSMethod* DBUSInterface::getMethod(std::string methodName) {
  for (DBUSMethod& method : this->methods) {
    if (method.getName() == methodName) return &method;
  }
  return nullptr;
}

std::vector<DBUSProperty> DBUSInterface::getProperties() {
  return this->properties;
}

std::vector<DBUSMethod> DBUSInterface::getMethods() {
  return this->methods;
}

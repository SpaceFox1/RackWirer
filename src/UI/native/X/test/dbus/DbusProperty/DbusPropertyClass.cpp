#include "./DbusPropertyClass.h"

DBUSProperty::DBUSProperty(std::string name, DbusTypeSignature type, DBUSPropertyAccessTypes accessType, DbusPropertyCallback callback) : type(type) {
  this->name = name;
  this->accessType = accessType;
  this->callback = callback;
}

std::string DBUSProperty::getName() {
  return this->name;
}

DbusTypeSignature DBUSProperty::getType() {
  return this->type;
}

DBUSPropertyAccessTypes DBUSProperty::getAccessType() {
  return this->accessType;
}

DbusPropertyCallback DBUSProperty::getCallback() {
  return this->callback;
}

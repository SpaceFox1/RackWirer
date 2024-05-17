#include "./DbusStructs.h"

DBUSStruct::DBUSStruct(std::vector<DbusGenericType*> types) : types(types) {}
DBUSStruct::DBUSStruct() {}

void DBUSStruct::addType(DbusGenericType &type) {
  this->types.push_back(&type);
}

bool DBUSStruct::addValue(DbusGenericType &type, void* value) {
  int currentListIndex = this->values.size();
  if (currentListIndex >= static_cast<int>(this->types.size())) return false;
  if (this->types.at(currentListIndex)->isEqual(type)) return false;
  this->values.push_back(value);
  return true;
}

std::vector<DbusGenericType*> DBUSStruct::getTypes() {
  return this->types;
}

std::vector<void*> DBUSStruct::getValues() {
  return this->values;
}

std::string DBUSStruct::getSignature() {
  std::string signature = "(";
  for (DbusGenericType *i : (this->types)) {
    signature += i->getSignature();
  }
  signature.append(")");
  return signature;
}

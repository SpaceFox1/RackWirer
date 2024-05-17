#include "./DbusPrimitives.h"

DBUSPString::DBUSPString(std::string value) : value(value) {}
std::string DBUSPString::getValue() { return value; }

DBUSPInt32::DBUSPInt32(int value) : value(value) {}
int DBUSPInt32::getValue() { return value; }

DBUSPUInt32::DBUSPUInt32(unsigned int value) : value(value) {}
unsigned int DBUSPUInt32::getValue() { return value; }

DBUSPInt64::DBUSPInt64(long value) : value(value) {}
long DBUSPInt64::getValue() { return value; }

DBUSPUInt64::DBUSPUInt64(unsigned long value) : value(value) {}
unsigned long DBUSPUInt64::getValue() { return value; }

DBUSPDouble::DBUSPDouble(double value) : value(value) {}
double DBUSPDouble::getValue() { return value; }

DBUSPBoolean::DBUSPBoolean(bool value) : value(value) {}
bool DBUSPBoolean::getValue() { return value; }

DBUSPByte::DBUSPByte(unsigned char value) : value(value) {}
unsigned char DBUSPByte::getValue() { return value; }

DBUSPObjectPath::DBUSPObjectPath(std::string value) : value(value) {}
std::string DBUSPObjectPath::getValue() { return value; }

DBUSPSignature::DBUSPSignature(std::string value) : value(value) {}
std::string DBUSPSignature::getValue() { return value; }

DBUSPString* DBUSPrimitiveFactory::createString(std::string value) { return new DBUSPString(value); }
DBUSPInt32* DBUSPrimitiveFactory::createInt32(int value) { return new DBUSPInt32(value); }
DBUSPUInt32* DBUSPrimitiveFactory::createUInt32(unsigned int value) { return new DBUSPUInt32(value); }
DBUSPInt64* DBUSPrimitiveFactory::createInt64(long value) { return new DBUSPInt64(value); }
DBUSPUInt64* DBUSPrimitiveFactory::createUInt64(unsigned long value) { return new DBUSPUInt64(value); }
DBUSPDouble* DBUSPrimitiveFactory::createDouble(double value) { return new DBUSPDouble(value); }
DBUSPBoolean* DBUSPrimitiveFactory::createBoolean(bool value) { return new DBUSPBoolean(value); }
DBUSPByte* DBUSPrimitiveFactory::createByte(unsigned char value) { return new DBUSPByte(value); }
DBUSPObjectPath* DBUSPrimitiveFactory::createObjectPath(std::string value) { return new DBUSPObjectPath(value); }
DBUSPSignature* DBUSPrimitiveFactory::createSignature(std::string value) { return new DBUSPSignature(value); }
void* DBUSPrimitiveFactory::createPrimitive(DBUSPrimitiveTypes type, void* value) {
  switch (type) {
    case DBUSPrimitiveTypes::DString:
      return createString(*(std::string*)value);
    case DBUSPrimitiveTypes::DInt32:
      return createInt32(*(int*)value);
    case DBUSPrimitiveTypes::DUInt32:
      return createUInt32(*(unsigned int*)value);
    case DBUSPrimitiveTypes::DInt64:
      return createInt64(*(long*)value);
    case DBUSPrimitiveTypes::DUInt64:
      return createUInt64(*(unsigned long*)value);
    case DBUSPrimitiveTypes::DDouble:
      return createDouble(*(double*)value);
    case DBUSPrimitiveTypes::DBoolean:
      return createBoolean(*(bool*)value);
    case DBUSPrimitiveTypes::DByte:
      return createByte(*(unsigned char*)value);
    case DBUSPrimitiveTypes::DObjectPath:
      return createObjectPath(*(std::string*)value);
    case DBUSPrimitiveTypes::DSignature:
      return createSignature(*(std::string*)value);
    default:
      return nullptr;
  }
}

DBUSPrimitiveTypes DBUSPrimitiveFactory::getType(DBUSGenericPrimitive* primitive) {
  if (dynamic_cast<DBUSPString*>(primitive) != nullptr) {
    return DBUSPrimitiveTypes::DString;
  } else if (dynamic_cast<DBUSPInt32*>(primitive) != nullptr) {
    return DBUSPrimitiveTypes::DInt32;
  } else if (dynamic_cast<DBUSPUInt32*>(primitive) != nullptr) {
    return DBUSPrimitiveTypes::DUInt32;
  } else if (dynamic_cast<DBUSPInt64*>(primitive) != nullptr) {
    return DBUSPrimitiveTypes::DInt64;
  } else if (dynamic_cast<DBUSPUInt64*>(primitive) != nullptr) {
    return DBUSPrimitiveTypes::DUInt64;
  } else if (dynamic_cast<DBUSPDouble*>(primitive) != nullptr) {
    return DBUSPrimitiveTypes::DDouble;
  } else if (dynamic_cast<DBUSPBoolean*>(primitive) != nullptr) {
    return DBUSPrimitiveTypes::DBoolean;
  } else if (dynamic_cast<DBUSPByte*>(primitive) != nullptr) {
    return DBUSPrimitiveTypes::DByte;
  } else if (dynamic_cast<DBUSPObjectPath*>(primitive) != nullptr) {
    return DBUSPrimitiveTypes::DObjectPath;
  } else if (dynamic_cast<DBUSPSignature*>(primitive) != nullptr) {
    return DBUSPrimitiveTypes::DSignature;
  } else {
    throw new std::runtime_error("Invalid primitive type to get signature");
  }
}

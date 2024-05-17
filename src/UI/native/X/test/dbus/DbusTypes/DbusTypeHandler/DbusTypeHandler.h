#ifndef __DBUSTYPEHANDLER__
#define __DBUSTYPEHANDLER__

#include <string>
#include <stdexcept>
#include <map>
#include <string>
#include <memory>
#include <functional>
#include <dbus/dbus.h>

typedef std::shared_ptr<std::function<DBusMessage* (DBusMessage*)>> DBUSMethodCallback;
typedef std::function<DBusMessage* (DBusMessage*)> DbusMethodCallbackFunc;

typedef DBUSMethodCallback DbusPropertyCallback;
typedef DbusMethodCallbackFunc DbusPropertyCallbackFunc;

enum class DBUSPrimitiveTypes {
  DString = 0,
  DInt32,
  DUInt32,
  DInt64,
  DUInt64,
  DDouble,
  DBoolean,
  DByte,
  DObjectPath,
  DSignature,
};

enum class DBUSPropertyAccessTypes {
  read = 0,
};

class DbusGenericType {
  public:
    DbusGenericType();
    virtual std::string getSignature()=0;
    bool isEqual(DbusGenericType& other);
};

struct DbusTypeSignature {
  std::string signature;
  DbusTypeSignature(std::string signature);
  DbusTypeSignature(DBUSPrimitiveTypes signature);
  DbusTypeSignature(DbusGenericType& signature);
};

class DbusTypeHandler {
  public:
    DbusTypeHandler(std::string signature, std::string value);
    DbusTypeHandler();
    DbusGenericType fromType(std::string signature, std::string value);
    DbusGenericType fromType();
};

#endif //__DBUSTYPEHANDLER__

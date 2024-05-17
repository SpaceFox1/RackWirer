#ifndef __DBUS_PRIMITIVES__
#define __DBUS_PRIMITIVES__

#include <string>
#include <map>
#include <exception>
#include "../DbusTypeHandler/DbusTypeHandler.h"

class DBUSGenericPrimitive : public DbusGenericType {
  public:
    virtual ~DBUSGenericPrimitive() {};
    virtual std::string getSignature()=0;
    virtual DBUSPrimitiveTypes getType()=0;
};

class DBUSPString : public DBUSGenericPrimitive {
  private:
    std::string value;

  public:
    DBUSPString(std::string value);
    std::string getValue();
    std::string getSignature() override { return "s"; }
    DBUSPrimitiveTypes getType() override { return DBUSPrimitiveTypes::DString; }
};

class DBUSPInt32 : public DBUSGenericPrimitive {
  private:
    int value;

  public:
    DBUSPInt32(int value);
    int getValue();
    std::string getSignature() override { return "i"; }
    DBUSPrimitiveTypes getType() override { return DBUSPrimitiveTypes::DInt32; }
};

class DBUSPUInt32 : public DBUSGenericPrimitive {
  private:
    unsigned int value;

  public:
    DBUSPUInt32(unsigned int value);
    unsigned int getValue();
    std::string getSignature() override { return "u"; }
    DBUSPrimitiveTypes getType() override { return DBUSPrimitiveTypes::DUInt32; }
};

class DBUSPInt64 : public DBUSGenericPrimitive {
  private:
    long value;

  public:
    DBUSPInt64(long value);
    long getValue();
    std::string getSignature() override { return "x"; }
    DBUSPrimitiveTypes getType() override { return DBUSPrimitiveTypes::DInt64; }
};

class DBUSPUInt64 : public DBUSGenericPrimitive {
  private:
    unsigned long value;

  public:
    DBUSPUInt64(unsigned long value);
    unsigned long getValue();
    std::string getSignature() override { return "t"; }
    DBUSPrimitiveTypes getType() override { return DBUSPrimitiveTypes::DUInt64; }
};

class DBUSPDouble : public DBUSGenericPrimitive {
  private:
    double value;

  public:
    DBUSPDouble(double value);
    double getValue();
    std::string getSignature() override { return "d"; }
    DBUSPrimitiveTypes getType() override { return DBUSPrimitiveTypes::DDouble; }
};

class DBUSPBoolean : public DBUSGenericPrimitive {
  private:
    bool value;

  public:
    DBUSPBoolean(bool value);
    bool getValue();
    std::string getSignature() override { return "b"; }
    DBUSPrimitiveTypes getType() override { return DBUSPrimitiveTypes::DBoolean; }
};

class DBUSPByte : public DBUSGenericPrimitive {
  private:
    unsigned char value;

  public:
    DBUSPByte(unsigned char value);
    unsigned char getValue();
    std::string getSignature() override { return "y"; }
    DBUSPrimitiveTypes getType() override { return DBUSPrimitiveTypes::DByte; }
};

class DBUSPObjectPath : public DBUSGenericPrimitive {
  private:
    std::string value;

  public:
    DBUSPObjectPath(std::string value);
    std::string getValue();
    std::string getSignature() override { return "o"; }
    DBUSPrimitiveTypes getType() override { return DBUSPrimitiveTypes::DObjectPath; }
};

class DBUSPSignature : public DBUSGenericPrimitive {
  private:
    std::string value;

  public:
    DBUSPSignature(std::string value);
    std::string getValue();
    std::string getSignature() override { return "g"; }
    DBUSPrimitiveTypes getType() override { return DBUSPrimitiveTypes::DSignature; }
};

class DBUSPrimitiveFactory {
  public:
    DBUSPString* createString(std::string value);
    DBUSPInt32* createInt32(int value);
    DBUSPUInt32* createUInt32(unsigned int value);
    DBUSPInt64* createInt64(long value);
    DBUSPUInt64* createUInt64(unsigned long value);
    DBUSPDouble* createDouble(double value);
    DBUSPBoolean* createBoolean(bool value);
    DBUSPByte* createByte(unsigned char value);
    DBUSPObjectPath* createObjectPath(std::string value);
    DBUSPSignature* createSignature(std::string value);
    void* createPrimitive(DBUSPrimitiveTypes type, void* value);
    DBUSPrimitiveTypes getType(DBUSGenericPrimitive* primitive);
};

#endif // __DBUS_PRIMITIVES__

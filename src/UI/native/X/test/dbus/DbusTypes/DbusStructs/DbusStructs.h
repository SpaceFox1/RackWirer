#ifndef __DBUS_STRUCTS__
#define __DBUS_STRUCTS__

#include <vector>
#include "../DbusTypeHandler/DbusTypeHandler.h"
#include "../DbusPrimitives/DbusPrimitives.h"

class DBUSStruct : public DbusGenericType {
  private:
    std::vector<DbusGenericType*> types;
    std::vector<void*> values;
  public:
    DBUSStruct(std::vector<DbusGenericType*> types);
    DBUSStruct();
    void addType(DbusGenericType &type);
    bool addValue(DbusGenericType &type, void* value);
    std::vector<DbusGenericType*> getTypes();
    std::vector<void*> getValues();
    std::string getSignature() override;
};

#endif // __DBUS_STRUCTS__

#ifndef DBUSPROPERTYCLASS_H
#define DBUSPROPERTYCLASS_H

#include <string>
#include <functional>
#include "../DbusTypes/DbusTypeHandler/DbusTypeHandler.h"

class DBUSProperty {
  private:
    std::string name;
    DbusTypeSignature type;
    DBUSPropertyAccessTypes accessType;
    DbusPropertyCallback callback;

  public:
    DBUSProperty(std::string name, DbusTypeSignature type, DBUSPropertyAccessTypes accessType, DbusPropertyCallback callback);

    std::string getName();
    DbusTypeSignature getType();
    DBUSPropertyAccessTypes getAccessType();
    DbusPropertyCallback getCallback();

};

#endif // DBUSPROPERTYCLASS_H

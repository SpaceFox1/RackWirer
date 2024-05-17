#ifndef DBUSMETHODCLASS_H
#define DBUSMETHODCLASS_H

#include <string>
#include <vector>
#include <functional>
#include <memory>
#include "../DbusTypes/DbusTypeHandler/DbusTypeHandler.h"

struct DBUSMethodArgument {
  std::string name;
  DbusTypeSignature type;
  DBUSMethodArgument(std::string name, DbusTypeSignature type) : type(type) {
    this->name = name;
  }
};

class DBUSMethod {
  private:
    std::string name;
    std::vector<DBUSMethodArgument> args;
    DBUSMethodArgument returnArg;
    DBUSMethodCallback callback;

  public:
    DBUSMethod(std::string name, std::vector<DBUSMethodArgument> args, DBUSMethodArgument returnArg, DBUSMethodCallback callback);
    DBUSMethod(std::string name, DBUSMethodArgument returnArg, DBUSMethodCallback callback);

    std::string getName();
    std::vector<DBUSMethodArgument> getArguments();
    DBUSMethodArgument getReturnArgument();
    DBUSMethodCallback getCallback();
};

#endif // DBUSMETHODCLASS_H

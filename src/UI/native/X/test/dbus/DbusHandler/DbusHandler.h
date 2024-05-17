#ifndef DBUSHANDLER_H
#define DBUSHANDLER_H

#include <string>
#include <vector>
#include "../DbusTypes/DbusStructs/DbusStructs.h"
#include "../DbusNode/DbusNodeClass.h"

class DbusHandler {
  private:
    DbusNode root;

  public:
    DbusHandler(std::string rootNodeName);
    DbusNode* getNode(std::string nodeName);

};

#endif // DBUSHANDLER_H

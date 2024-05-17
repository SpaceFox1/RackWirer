#include "./DbusMethodClass.h"

DBUSMethod::DBUSMethod(std::string name, std::vector<DBUSMethodArgument> args, DBUSMethodArgument returnArg, DBUSMethodCallback callback) : returnArg(returnArg) {
  this->name = name;
  this->args = args;
  this->callback = callback;
}

DBUSMethod::DBUSMethod(std::string name, DBUSMethodArgument returnArg, DBUSMethodCallback callback) : returnArg(returnArg) {
  this->name = name,
  this->args = std::vector<DBUSMethodArgument>();
  this->callback = callback;
}

std::string DBUSMethod::getName() {
  return this->name;
}

std::vector<DBUSMethodArgument> DBUSMethod::getArguments() {
  return this->args;
}

DBUSMethodArgument DBUSMethod::getReturnArgument() {
  return this->returnArg;
}

DBUSMethodCallback DBUSMethod::getCallback() {
  return this->callback;
}

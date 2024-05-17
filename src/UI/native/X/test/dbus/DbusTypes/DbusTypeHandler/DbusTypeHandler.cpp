#include "./DbusTypeHandler.h"

DbusTypeSignature::DbusTypeSignature(std::string signature) {
  this->signature = signature;
}

DbusTypeSignature::DbusTypeSignature(DBUSPrimitiveTypes signature) {
  if (signature == DBUSPrimitiveTypes::DString) {
    this->signature = "s";
  } else if (signature == DBUSPrimitiveTypes::DInt32) {
    this->signature = "i";
  } else if (signature == DBUSPrimitiveTypes::DUInt32) {
    this->signature = "u";
  } else if (signature == DBUSPrimitiveTypes::DInt64) {
    this->signature = "x";
  } else if (signature == DBUSPrimitiveTypes::DUInt64) {
    this->signature = "t";
  } else if (signature == DBUSPrimitiveTypes::DDouble) {
    this->signature = "d";
  } else if (signature == DBUSPrimitiveTypes::DByte) {
    this->signature = "y";
  } else if (signature == DBUSPrimitiveTypes::DBoolean) {
    this->signature = "b";
  } else if (signature == DBUSPrimitiveTypes::DObjectPath) {
    this->signature = "o";
  } else if (signature == DBUSPrimitiveTypes::DSignature) {
    this->signature = "g";
  } else {
    throw "Invalid signature type";
  }
}

DbusTypeSignature::DbusTypeSignature(DbusGenericType& signature) {
  this->signature = signature.getSignature();
}

DbusGenericType::DbusGenericType() {}

bool DbusGenericType::isEqual(DbusGenericType& other) {
  return this->getSignature() == other.getSignature();
}

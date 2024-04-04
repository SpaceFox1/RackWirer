#ifndef DBUSPROPERTIESHANDLER_H
#define DBUSPROPERTIESHANDLER_H

class DbusNodeRegistrer {
  private:
    struct Parameter {
      const char* name;
      char type;
    };

    // method arguments structure
    struct MethodArg {
      const char* name;
      char type;
    };

    // method structure
    struct Method {
      const char* name;
      // callback function
      void (*callback)(void* value);
    };

    // interface structure
    struct Interface {
      const char* name;
      struct Interface* next;
    };

  public:
  DbusNodeRegistrer();
  ~DbusNodeRegistrer();

  void appendNode(DbusNodeRegistrer node);
  
};

#endif // DBUSPROPERTIESHANDLER_H

#include "./DbusThreadTest.h"

const char *menubar_introspection_xml =
  DBUS_INTROSPECT_1_0_XML_DOCTYPE_DECL_NODE
  "<node>\n"

  "  <interface name='org.freedesktop.DBus.Introspectable'>\n"
  "    <method name='Introspect'>\n"
  "      <arg name='data' type='s' direction='out' />\n"
  "    </method>\n"
  "  </interface>\n"

  "  <interface name='org.freedesktop.DBus.Properties'>\n"
  "    <method name='Get'>\n"
  "      <arg name='interface' type='s' direction='in' />\n"
  "      <arg name='property'  type='s' direction='in' />\n"
  "      <arg name='value'     type='s' direction='out' />\n"
  "    </method>\n"
  "    <method name='GetAll'>\n"
  "      <arg name='interface'  type='s'     direction='in'/>\n"
  "      <arg name='properties' type='a{sv}' direction='out'/>\n"
  "    </method>\n"
  "  </interface>\n"

  "  <interface name='com.canonical.dbusmenu'>\n"
  "    <method name='GetLayout'>\n"
  "      <arg type='i' direction='in' />\n"
  "      <arg type='a(iiia{sv})' direction='out' />\n"
  "    </method>\n"
  "    <method name='Event'>\n"
  "      <arg type='i' direction='in' />\n"
  "      <arg type='s' direction='in' />\n"
  "    </method>\n"
  "    <method name='AboutToShow'>\n"
  "      <arg type='i' direction='in' />\n"
  "    </method>\n"
  "  </interface>\n"

  "</node>\n";

const char *server_introspection_xml =
	DBUS_INTROSPECT_1_0_XML_DOCTYPE_DECL_NODE
	"<node>\n"

	"  <interface name='org.freedesktop.DBus.Introspectable'>\n"
	"    <method name='Introspect'>\n"
	"      <arg name='data' type='s' direction='out' />\n"
	"    </method>\n"
	"  </interface>\n"

	"  <interface name='org.freedesktop.DBus.Properties'>\n"
	"    <method name='Get'>\n"
	"      <arg name='interface' type='s' direction='in' />\n"
	"      <arg name='property'  type='s' direction='in' />\n"
	"      <arg name='value'     type='s' direction='out' />\n"
	"    </method>\n"
	"    <method name='GetAll'>\n"
	"      <arg name='interface'  type='s'     direction='in'/>\n"
	"      <arg name='properties' type='a{sv}' direction='out'/>\n"
	"    </method>\n"
	"  </interface>\n"

	"  <interface name='br.com.talesgardem.rackwirer'>\n"
	"    <property name='Version' type='s' access='read' />\n"
	"    <method name='Ping' >\n"
	"      <arg type='s' direction='out' />\n"
	"    </method>\n"
	"  </interface>\n"

  "  <node name='MenuBar'></node>\n"

	"</node>\n";

DBusHandlerResult server_get_properties_handler(const char *property, DBusConnection *conn, DBusMessage *reply) {
  if (!strcmp(property, "Version")) {
    dbus_message_append_args(reply, DBUS_TYPE_STRING, &"0.1", DBUS_TYPE_INVALID);
  } else {
    return DBUS_HANDLER_RESULT_NOT_YET_HANDLED;
  }

  if (!dbus_connection_send(conn, reply, NULL)) {
    return DBUS_HANDLER_RESULT_NEED_MEMORY;
  }

  return DBUS_HANDLER_RESULT_HANDLED;
}

DBusHandlerResult server_get_all_properties_handler(DBusConnection *conn, DBusMessage *reply) {
  DBusHandlerResult result;
  DBusMessageIter array, dict, iter, variant;
  const char *property = "Version";

  result = DBUS_HANDLER_RESULT_NEED_MEMORY;

  dbus_message_iter_init_append(reply, &iter);
  dbus_message_iter_open_container(&iter, DBUS_TYPE_ARRAY, "{sv}", &array);

  property = "Version";
  dbus_message_iter_open_container(&array, DBUS_TYPE_DICT_ENTRY, NULL, &dict);
  dbus_message_iter_append_basic(&dict, DBUS_TYPE_STRING, &property);
  dbus_message_iter_open_container(&dict, DBUS_TYPE_VARIANT, "s", &variant);
  dbus_message_iter_append_basic(&variant, DBUS_TYPE_STRING, &"0.1");
  dbus_message_iter_close_container(&dict, &variant);
  dbus_message_iter_close_container(&array, &dict);

  dbus_message_iter_close_container(&iter, &array);

  if (dbus_connection_send(conn, reply, NULL)) {
    result = DBUS_HANDLER_RESULT_HANDLED;
  }
  return result;
}

DbusThreadTest::DbusThreadTest(ActionEmitter* ae) {
  this->isRunning = false;
  this->myDBusConnection = nullptr;
  this->myDBusError = DBUS_ERROR_INIT;
  this->ae = ae;
}

void DbusThreadTest::DbusLoop() {
  while (this->isRunning) {
    dbus_connection_read_write_dispatch(this->myDBusConnection, 100);
    DBusMessage* msg =  dbus_connection_pop_message(this->myDBusConnection);
    if (msg != NULL) this->handleIncommingMessage(msg);
  }
}

int DbusThreadTest::start(XLibThreadTest* xlt) {
  if (this->isRunning) return -1;
  this->isRunning = true;
  this->myDBusError = DBUS_ERROR_INIT;

  dbus_threads_init_default();

  this->myDBusConnection = dbus_bus_get(DBUS_BUS_SESSION, &this->myDBusError);
  if (dbus_error_is_set(&this->myDBusError)) {
    fprintf(stderr, "Connection Error (%s)\n", this->myDBusError.message);
    dbus_error_free(&this->myDBusError);
  }
  
  if (NULL == this->myDBusConnection) {
    fprintf(stderr, "Connection Null\n");
    return 1;
  }

  dbus_connection_set_exit_on_disconnect(this->myDBusConnection, false);

  dbus_bus_request_name(this->myDBusConnection, "br.com.talesgardem.rackwirer", DBUS_NAME_FLAG_REPLACE_EXISTING, &this->myDBusError);
  
  if (dbus_error_is_set(&this->myDBusError)) {
    fprintf(stderr, "Name Error (%s)\n", this->myDBusError.message);
    dbus_error_free(&this->myDBusError);
    return 1;
  }

  DbusThreadTest::uniqName = dbus_bus_get_unique_name(this->myDBusConnection);

  static const char* DBUS_SERVICE = "com.canonical.AppMenu.Registrar";
  static const char* DBUS_OBJECT_PATH = "/com/canonical/AppMenu/Registrar";
  static const char* DBUS_INTERFACE = "com.canonical.AppMenu.Registrar";

  // RegisterWindow
  DBusMessage* registerWinMsg = dbus_message_new_method_call(DBUS_SERVICE, DBUS_OBJECT_PATH, DBUS_INTERFACE, "RegisterWindow");
  DBusMessageIter registerWinArgs;
  if (nullptr == registerWinMsg) {
    fprintf(stderr, "Message Null\n");
    return 1;
  }

  dbus_message_iter_init_append(registerWinMsg, &registerWinArgs);
  const void *x11Window = &xlt->window;
  if (!dbus_message_iter_append_basic(&registerWinArgs, DBUS_TYPE_UINT32, x11Window)) {
    fprintf(stderr, "Out Of Memory!\n");
    return 1;
  }

  // add second argument
  const char *menuObjectPath = "/MenuBar";

  if (!dbus_message_iter_append_basic(&registerWinArgs, DBUS_TYPE_OBJECT_PATH, &menuObjectPath)) {
    fprintf(stderr, "Out Of Memory!\n");
    return 1;
  }

  // send message
  dbus_connection_send(this->myDBusConnection, registerWinMsg, NULL);

  // free message
  dbus_message_unref(registerWinMsg);

  this->DbusLoop();

  return 0;
}

int DbusThreadTest::stop() {
  this->isRunning = false;

  return 0;
}

DBusHandlerResult DbusThreadTest::handleIncommingMessage(DBusMessage* message) {
  dbus_connection_pop_message(this->myDBusConnection);
  dbus_error_init(&this->myDBusError);
  DBusMessage *reply = NULL;
  DBusHandlerResult result = DBUS_HANDLER_RESULT_NOT_YET_HANDLED;
  
  const char* interface = dbus_message_get_interface(message);
  if (interface == nullptr) return result;

  if (strcmp(interface, "com.canonical.dbusmenu") == 0) {
    DbusThreadTest::handleMenuMessage(message, reply);
  }

  if (
    dbus_message_is_method_call(message, DBUS_INTERFACE_INTROSPECTABLE, "Introspect") || 
    dbus_message_is_method_call(message, DBUS_INTERFACE_PROPERTIES, "Get") || 
    dbus_message_is_method_call(message, DBUS_INTERFACE_PROPERTIES, "GetAll") ||
    dbus_message_is_signal(message, "org.freedesktop.DBus", "NameAcquired")
  ) DbusThreadTest::handleFreeDesktopMessage(message, reply);

  if (dbus_message_is_method_call(message, "br.com.talesgardem.rackwirer", "Ping")) DbusThreadTest::handleOwnMethodsMessage(message, reply);

  result = handleResponseCleanUp(message, reply, this->myDBusError);
  dbus_message_unref(message);
  return result;
}

void DbusThreadTest::handleMenuMessage(DBusMessage* message, DBusMessage* reply) {
  const char* dbusmenu = "com.canonical.dbusmenu";

  DBusMessageIter args;
  dbus_message_iter_init(message, &args);

  if (dbus_message_is_method_call(message, dbusmenu, "GetLayout")) {
    if (!(reply = dbus_message_new_method_return(message))) return;

    int32_t id;
    dbus_message_iter_get_basic(&args, &id);

    if (id == 0) {
      DBusMessageIter iter;
      dbus_message_iter_init_append(reply, &iter);

      // append int32 17
      int32_t value = 17; // magic number
      dbus_message_iter_append_basic(&iter, DBUS_TYPE_UINT32, &value);

      // append struct
      DBusMessageIter structMain;
      dbus_message_iter_open_container(&iter, DBUS_TYPE_STRUCT, NULL, &structMain); // Opens Struct

      // append int32 0
      int32_t elementId = 0; // Root menu id
      dbus_message_iter_append_basic(&structMain, DBUS_TYPE_INT32, &elementId);

      // append array
      DBusMessageIter arrayRootMenuItem;
      dbus_message_iter_open_container(&structMain, DBUS_TYPE_ARRAY, "{sv}", &arrayRootMenuItem); // Opens Array

      // append dict entry
      DBusMessageIter dictRootMenuItemKind;
      dbus_message_iter_open_container(&arrayRootMenuItem, DBUS_TYPE_DICT_ENTRY, NULL, &dictRootMenuItemKind); // Opens Dict

      // append string
      const char *stringRootMenuItemKind = "children-display";
      dbus_message_iter_append_basic(&dictRootMenuItemKind, DBUS_TYPE_STRING, &stringRootMenuItemKind);

      // append variant
      const char *variantStringRootMenuItemKindName = "submenu";
      DBusMessageIter variantRootMenuItemKindName;
      dbus_message_iter_open_container(&dictRootMenuItemKind, DBUS_TYPE_VARIANT, "s", &variantRootMenuItemKindName); // Opens Variant
      dbus_message_iter_append_basic(&variantRootMenuItemKindName, DBUS_TYPE_STRING, &variantStringRootMenuItemKindName);
      dbus_message_iter_close_container(&dictRootMenuItemKind, &variantRootMenuItemKindName); // Closes Variant

      dbus_message_iter_close_container(&arrayRootMenuItem, &dictRootMenuItemKind); // Closes Dict

      // append dict entry
      DBusMessageIter dictRootMenuItemLabel;
      dbus_message_iter_open_container(&arrayRootMenuItem, DBUS_TYPE_DICT_ENTRY, NULL, &dictRootMenuItemLabel); // Opens Dict

      // append string
      const char *stringRootMenuItemLabel = "label";
      dbus_message_iter_append_basic(&dictRootMenuItemLabel, DBUS_TYPE_STRING, &stringRootMenuItemLabel);

      // append variant
      const char *variantStringRootMenuItemLabelName = "Root";
      DBusMessageIter variantRootMenuItemLabelName;
      dbus_message_iter_open_container(&dictRootMenuItemLabel, DBUS_TYPE_VARIANT, "s", &variantRootMenuItemLabelName); // Opens Variant
      dbus_message_iter_append_basic(&variantRootMenuItemLabelName, DBUS_TYPE_STRING, &variantStringRootMenuItemLabelName);
      dbus_message_iter_close_container(&dictRootMenuItemLabel, &variantRootMenuItemLabelName); // Closes Variant

      dbus_message_iter_close_container(&arrayRootMenuItem, &dictRootMenuItemLabel); // Closes Dict

      dbus_message_iter_close_container(&structMain, &arrayRootMenuItem); // Closes Array

      // append array
      DBusMessageIter arrayRootMenuSubmenuItems;
      dbus_message_iter_open_container(&structMain, DBUS_TYPE_ARRAY, "v", &arrayRootMenuSubmenuItems); // Opens Array

      // append variant
      DBusMessageIter variantRootMenuSubmenuItem1;
      dbus_message_iter_open_container(&arrayRootMenuSubmenuItems, DBUS_TYPE_VARIANT, DBUS_STRUCT_BEGIN_CHAR_AS_STRING "ia{sv}a{sv}" DBUS_STRUCT_END_CHAR_AS_STRING, &variantRootMenuSubmenuItem1); // Opens Variant

      // append struct
      DBusMessageIter structRootMenuSubmenuItem1;
      dbus_message_iter_open_container(&variantRootMenuSubmenuItem1, DBUS_TYPE_STRUCT, NULL, &structRootMenuSubmenuItem1); // Opens Struct

      // append int32 10
      int32_t elementIdRootMenuSubmenuItem1 = 10; // Root menu item id
      dbus_message_iter_append_basic(&structRootMenuSubmenuItem1, DBUS_TYPE_INT32, &elementIdRootMenuSubmenuItem1);

      // append array
      DBusMessageIter arrayRootMenuSubmenuItem1Data;
      dbus_message_iter_open_container(&structRootMenuSubmenuItem1, DBUS_TYPE_ARRAY, "{sv}", &arrayRootMenuSubmenuItem1Data); // Opens Array

      // append dict entry
      DBusMessageIter dictRootMenuSubmenuItem1DataKind;
      dbus_message_iter_open_container(&arrayRootMenuSubmenuItem1Data, DBUS_TYPE_DICT_ENTRY, NULL, &dictRootMenuSubmenuItem1DataKind); // Opens Dict

      // append string
      const char *stringRootMenuSubmenuItem1DataKind = "children-display";
      dbus_message_iter_append_basic(&dictRootMenuSubmenuItem1DataKind, DBUS_TYPE_STRING, &stringRootMenuSubmenuItem1DataKind);

      // append variant
      const char *variantStringRootMenuSubmenuItem1DataKindName = "submenu";
      DBusMessageIter variantRootMenuSubmenuItem1DataKindName;
      dbus_message_iter_open_container(&dictRootMenuSubmenuItem1DataKind, DBUS_TYPE_VARIANT, "s", &variantRootMenuSubmenuItem1DataKindName); // Opens Variant
      dbus_message_iter_append_basic(&variantRootMenuSubmenuItem1DataKindName, DBUS_TYPE_STRING, &variantStringRootMenuSubmenuItem1DataKindName);
      dbus_message_iter_close_container(&dictRootMenuSubmenuItem1DataKind, &variantRootMenuSubmenuItem1DataKindName); // Closes Variant

      dbus_message_iter_close_container(&arrayRootMenuSubmenuItem1Data, &dictRootMenuSubmenuItem1DataKind); // Closes Dict

      // append dict entry
      DBusMessageIter dictRootMenuSubmenuItem1DataLabel;
      dbus_message_iter_open_container(&arrayRootMenuSubmenuItem1Data, DBUS_TYPE_DICT_ENTRY, NULL, &dictRootMenuSubmenuItem1DataLabel); // Opens Dict

      // append string
      const char *stringRootMenuSubmenuItem1DataLabel = "label";
      dbus_message_iter_append_basic(&dictRootMenuSubmenuItem1DataLabel, DBUS_TYPE_STRING, &stringRootMenuSubmenuItem1DataLabel);

      // append variant
      const char *variantStringRootMenuSubmenuItem1DataLabelName = "_File";
      DBusMessageIter variantRootMenuSubmenuItem1DataLabelName;
      dbus_message_iter_open_container(&dictRootMenuSubmenuItem1DataLabel, DBUS_TYPE_VARIANT, "s", &variantRootMenuSubmenuItem1DataLabelName); // Opens Variant
      dbus_message_iter_append_basic(&variantRootMenuSubmenuItem1DataLabelName, DBUS_TYPE_STRING, &variantStringRootMenuSubmenuItem1DataLabelName);
      dbus_message_iter_close_container(&dictRootMenuSubmenuItem1DataLabel, &variantRootMenuSubmenuItem1DataLabelName); // Closes Variant

      dbus_message_iter_close_container(&arrayRootMenuSubmenuItem1Data, &dictRootMenuSubmenuItem1DataLabel); // Closes Dict

      dbus_message_iter_close_container(&structRootMenuSubmenuItem1, &arrayRootMenuSubmenuItem1Data); // Closes Array

      // send another empty array for magic reasons
      DBusMessageIter arrayRootMenuSubmenuItem1Empty;
      dbus_message_iter_open_container(&structRootMenuSubmenuItem1, DBUS_TYPE_ARRAY, "{sv}", &arrayRootMenuSubmenuItem1Empty); // Opens Array
      dbus_message_iter_close_container(&structRootMenuSubmenuItem1, &arrayRootMenuSubmenuItem1Empty); // Closes Array

      dbus_message_iter_close_container(&variantRootMenuSubmenuItem1, &structRootMenuSubmenuItem1); // Closes Struct

      dbus_message_iter_close_container(&arrayRootMenuSubmenuItems, &variantRootMenuSubmenuItem1); // Closes Variant

      dbus_message_iter_close_container(&structMain, &arrayRootMenuSubmenuItems); // Closes Array

      dbus_message_iter_close_container(&iter, &structMain); // Closes Struct
    } else if (id == 10) {
      DBusMessageIter iter;
      dbus_message_iter_init_append(reply, &iter);

      // append int32 17
      int32_t value = 17; // magic number
      dbus_message_iter_append_basic(&iter, DBUS_TYPE_UINT32, &value);

      // append struct
      DBusMessageIter structMain;
      dbus_message_iter_open_container(&iter, DBUS_TYPE_STRUCT, NULL, &structMain); // Opens Struct

      // append int32 0
      int32_t elementId = 10; // Root menu id
      dbus_message_iter_append_basic(&structMain, DBUS_TYPE_INT32, &elementId);

      // append array
      DBusMessageIter arrayRootMenuItem;
      dbus_message_iter_open_container(&structMain, DBUS_TYPE_ARRAY, "{sv}", &arrayRootMenuItem); // Opens Array

      // append dict entry
      DBusMessageIter dictRootMenuItemKind;
      dbus_message_iter_open_container(&arrayRootMenuItem, DBUS_TYPE_DICT_ENTRY, NULL, &dictRootMenuItemKind); // Opens Dict

      // append string
      const char *stringRootMenuItemKind = "children-display";
      dbus_message_iter_append_basic(&dictRootMenuItemKind, DBUS_TYPE_STRING, &stringRootMenuItemKind);

      // append variant
      const char *variantStringRootMenuItemKindName = "submenu";
      DBusMessageIter variantRootMenuItemKindName;
      dbus_message_iter_open_container(&dictRootMenuItemKind, DBUS_TYPE_VARIANT, "s", &variantRootMenuItemKindName); // Opens Variant
      dbus_message_iter_append_basic(&variantRootMenuItemKindName, DBUS_TYPE_STRING, &variantStringRootMenuItemKindName);
      dbus_message_iter_close_container(&dictRootMenuItemKind, &variantRootMenuItemKindName); // Closes Variant

      dbus_message_iter_close_container(&arrayRootMenuItem, &dictRootMenuItemKind); // Closes Dict

      // append dict entry
      DBusMessageIter dictRootMenuItemLabel;
      dbus_message_iter_open_container(&arrayRootMenuItem, DBUS_TYPE_DICT_ENTRY, NULL, &dictRootMenuItemLabel); // Opens Dict

      // append string
      const char *stringRootMenuItemLabel = "label";
      dbus_message_iter_append_basic(&dictRootMenuItemLabel, DBUS_TYPE_STRING, &stringRootMenuItemLabel);

      // append variant
      const char *variantStringRootMenuItemLabelName = "_File"; // the _ indicates wich letter is supposed to be highlighted when the user holds alt in this case is the F
      DBusMessageIter variantRootMenuItemLabelName;
      dbus_message_iter_open_container(&dictRootMenuItemLabel, DBUS_TYPE_VARIANT, "s", &variantRootMenuItemLabelName); // Opens Variant
      dbus_message_iter_append_basic(&variantRootMenuItemLabelName, DBUS_TYPE_STRING, &variantStringRootMenuItemLabelName);
      dbus_message_iter_close_container(&dictRootMenuItemLabel, &variantRootMenuItemLabelName); // Closes Variant

      dbus_message_iter_close_container(&arrayRootMenuItem, &dictRootMenuItemLabel); // Closes Dict

      dbus_message_iter_close_container(&structMain, &arrayRootMenuItem); // Closes Array

      // append array
      DBusMessageIter arrayRootMenuSubmenuItems;
      dbus_message_iter_open_container(&structMain, DBUS_TYPE_ARRAY, "v", &arrayRootMenuSubmenuItems); // Opens Array

      // append variant
      DBusMessageIter variantRootMenuSubmenuItem1;
      dbus_message_iter_open_container(&arrayRootMenuSubmenuItems, DBUS_TYPE_VARIANT, DBUS_STRUCT_BEGIN_CHAR_AS_STRING "ia{sv}a{sv}" DBUS_STRUCT_END_CHAR_AS_STRING, &variantRootMenuSubmenuItem1); // Opens Variant

      // append struct
      DBusMessageIter structRootMenuSubmenuItem1;
      dbus_message_iter_open_container(&variantRootMenuSubmenuItem1, DBUS_TYPE_STRUCT, NULL, &structRootMenuSubmenuItem1); // Opens Struct

      // append int32 10
      int32_t elementIdRootMenuSubmenuItem1 = 11; // Fist item of submenu 10
      dbus_message_iter_append_basic(&structRootMenuSubmenuItem1, DBUS_TYPE_INT32, &elementIdRootMenuSubmenuItem1);

      // append array
      DBusMessageIter arrayRootMenuSubmenuItem1Data;
      dbus_message_iter_open_container(&structRootMenuSubmenuItem1, DBUS_TYPE_ARRAY, "{sv}", &arrayRootMenuSubmenuItem1Data); // Opens Array
      
      // append dict entry
      DBusMessageIter dictRootMenuSubmenuItem1DataKind;
      dbus_message_iter_open_container(&arrayRootMenuSubmenuItem1Data, DBUS_TYPE_DICT_ENTRY, NULL, &dictRootMenuSubmenuItem1DataKind); // Opens Dict

      // append string
      const char *stringRootMenuSubmenuItem1DataKind = "shortcut";
      dbus_message_iter_append_basic(&dictRootMenuSubmenuItem1DataKind, DBUS_TYPE_STRING, &stringRootMenuSubmenuItem1DataKind);

      // append variant array
      DBusMessageIter variantRootMenuSubmenuItem1DataKindName;
      dbus_message_iter_open_container(&dictRootMenuSubmenuItem1DataKind, DBUS_TYPE_VARIANT, "aas", &variantRootMenuSubmenuItem1DataKindName); // Opens Variant

      // append array of array of string
      DBusMessageIter arrayRootMenuSubmenuItem1DataKindName;
      dbus_message_iter_open_container(&variantRootMenuSubmenuItem1DataKindName, DBUS_TYPE_ARRAY, "aas", &arrayRootMenuSubmenuItem1DataKindName); // Opens Array

      // append array of string
      DBusMessageIter arrayRootMenuSubmenuItem1DataKindNameShortcut;
      dbus_message_iter_open_container(&arrayRootMenuSubmenuItem1DataKindName, DBUS_TYPE_ARRAY, "s", &arrayRootMenuSubmenuItem1DataKindNameShortcut); // Opens Array

      // append string
      const char *stringRootMenuSubmenuItem1ShortcutPart1 = "Control"; // i tried with both Control and Ctrl and it didn't change how it is shown on the menu but neither worked, i'm keeping Control since is what other apps are using
      dbus_message_iter_append_basic(&arrayRootMenuSubmenuItem1DataKindNameShortcut, DBUS_TYPE_STRING, &stringRootMenuSubmenuItem1ShortcutPart1);

      // append string
      const char *stringRootMenuSubmenuItem1ShortcutPart2 = "o"; // i don't know if it matters but it seens to be sent in lowercase
      dbus_message_iter_append_basic(&arrayRootMenuSubmenuItem1DataKindNameShortcut, DBUS_TYPE_STRING, &stringRootMenuSubmenuItem1ShortcutPart2);

      dbus_message_iter_close_container(&arrayRootMenuSubmenuItem1DataKindName, &arrayRootMenuSubmenuItem1DataKindNameShortcut); // Closes Array

      dbus_message_iter_close_container(&variantRootMenuSubmenuItem1DataKindName, &arrayRootMenuSubmenuItem1DataKindName); // Closes Array

      dbus_message_iter_close_container(&dictRootMenuSubmenuItem1DataKind, &variantRootMenuSubmenuItem1DataKindName); // Closes Variant

      dbus_message_iter_close_container(&arrayRootMenuSubmenuItem1Data, &dictRootMenuSubmenuItem1DataKind); // Closes Dict

      // append dict entry
      DBusMessageIter dictRootMenuSubmenuItem1DataLabel;
      dbus_message_iter_open_container(&arrayRootMenuSubmenuItem1Data, DBUS_TYPE_DICT_ENTRY, NULL, &dictRootMenuSubmenuItem1DataLabel); // Opens Dict

      // append string
      const char *stringRootMenuSubmenuItem1DataLabel = "label";
      dbus_message_iter_append_basic(&dictRootMenuSubmenuItem1DataLabel, DBUS_TYPE_STRING, &stringRootMenuSubmenuItem1DataLabel);

      // append variant
      const char *variantStringRootMenuSubmenuItem1DataLabelName = "_Open";
      DBusMessageIter variantRootMenuSubmenuItem1DataLabelName;
      dbus_message_iter_open_container(&dictRootMenuSubmenuItem1DataLabel, DBUS_TYPE_VARIANT, "s", &variantRootMenuSubmenuItem1DataLabelName); // Opens Variant
      dbus_message_iter_append_basic(&variantRootMenuSubmenuItem1DataLabelName, DBUS_TYPE_STRING, &variantStringRootMenuSubmenuItem1DataLabelName);
      dbus_message_iter_close_container(&dictRootMenuSubmenuItem1DataLabel, &variantRootMenuSubmenuItem1DataLabelName); // Closes Variant

      dbus_message_iter_close_container(&arrayRootMenuSubmenuItem1Data, &dictRootMenuSubmenuItem1DataLabel); // Closes Dict

      dbus_message_iter_close_container(&structRootMenuSubmenuItem1, &arrayRootMenuSubmenuItem1Data); // Closes Array

      // send another empty array for magic reasons
      DBusMessageIter arrayRootMenuSubmenuItem1Empty;
      dbus_message_iter_open_container(&structRootMenuSubmenuItem1, DBUS_TYPE_ARRAY, "{sv}", &arrayRootMenuSubmenuItem1Empty); // Opens Array

      dbus_message_iter_close_container(&structRootMenuSubmenuItem1, &arrayRootMenuSubmenuItem1Empty); // Closes Array

      dbus_message_iter_close_container(&variantRootMenuSubmenuItem1, &structRootMenuSubmenuItem1); // Closes Struct

      dbus_message_iter_close_container(&arrayRootMenuSubmenuItems, &variantRootMenuSubmenuItem1); // Closes Variant

      dbus_message_iter_close_container(&structMain, &arrayRootMenuSubmenuItems); // Closes Array

      dbus_message_iter_close_container(&iter, &structMain); // Closes Struct
    }

    dbus_connection_send(this->myDBusConnection, reply, NULL);
  } else if (dbus_message_is_method_call(message, dbusmenu, "AboutToShow")) {
    // we just reply with a false boolean
    if (!(reply = dbus_message_new_method_return(message))) return;

    dbus_bool_t value = false;
    dbus_message_append_args(reply, DBUS_TYPE_BOOLEAN, &value, DBUS_TYPE_INVALID);

    dbus_connection_send(this->myDBusConnection, reply, NULL);
  } else if (dbus_message_is_method_call(message, dbusmenu, "Event")) {
    if (!(reply = dbus_message_new_method_return(message))) return;
    // read the args
    // there shold be a int32 and a string and a useless variant

    DBusMessageIter args;
    dbus_message_iter_init(message, &args);

    int32_t id;
    dbus_message_iter_get_basic(&args, &id);

    dbus_message_iter_next(&args);

    char *eventName;
    dbus_message_iter_get_basic(&args, &eventName);

    if (strcmp(eventName, "clicked") == 0) {
      // we could parse wich button were pressed but, we only have one button so we kinda just know that it was it that got pressed
      fprintf(stderr, "Open Button pressed\n");
      void * msg = &message;
      this->ae->emit("buttonClick", msg);
    }

    dbus_connection_send(this->myDBusConnection, reply, NULL);
  }
}

void DbusThreadTest::handleFreeDesktopMessage(DBusMessage* message, DBusMessage* reply) {
  if (dbus_message_is_method_call(message, DBUS_INTERFACE_INTROSPECTABLE, "Introspect")) {
    if (!(reply = dbus_message_new_method_return(message))) return;

    const char *path = dbus_message_get_path(message);
    if (!strcmp(path, "/MenuBar")) {
      dbus_message_append_args(reply, DBUS_TYPE_STRING, &menubar_introspection_xml, DBUS_TYPE_INVALID);
    } else {
      dbus_message_append_args(reply, DBUS_TYPE_STRING, &server_introspection_xml, DBUS_TYPE_INVALID);
    }
  } else if (dbus_message_is_method_call(message, DBUS_INTERFACE_PROPERTIES, "Get")) {
    const char *interface, *property;

    if (!dbus_message_get_args(message, &this->myDBusError, DBUS_TYPE_STRING, &interface, DBUS_TYPE_STRING, &property, DBUS_TYPE_INVALID)) return;

    if (!(reply = dbus_message_new_method_return(message))) return;

    server_get_properties_handler(property, this->myDBusConnection, reply);
    dbus_message_unref(reply);
  } else if (dbus_message_is_method_call(message, DBUS_INTERFACE_PROPERTIES, "GetAll")) {
    if (!(reply = dbus_message_new_method_return(message))) return;

    server_get_all_properties_handler(this->myDBusConnection, reply);
    dbus_message_unref(reply);
  } else if (dbus_message_is_signal(message, "org.freedesktop.DBus", "NameAcquired")) {
    fprintf(stderr, "Name acquired on: %s\n", dbus_message_get_path(message));
  }
}

void DbusThreadTest::handleOwnMethodsMessage(DBusMessage* message, DBusMessage* reply) {
  if (dbus_message_is_method_call(message, "br.com.talesgardem.rackwirer", "Ping")) {
    if (!(reply = dbus_message_new_method_return(message))) return;
    dbus_message_append_args(reply, DBUS_TYPE_STRING, "Pong", DBUS_TYPE_INVALID);
  }
}

DBusHandlerResult DbusThreadTest::handleResponseCleanUp(DBusMessage* message, DBusMessage *reply, DBusError err) {
  if (dbus_error_is_set(&err)) {
    if (reply) dbus_message_unref(reply);
    reply = dbus_message_new_error(message, err.name, err.message);
    dbus_error_free(&err);
  }

  if (!reply) return DBUS_HANDLER_RESULT_NEED_MEMORY;

  DBusHandlerResult result = DBUS_HANDLER_RESULT_HANDLED;
  if (!dbus_connection_send(DbusThreadTest::myDBusConnection, reply, NULL)) result = DBUS_HANDLER_RESULT_NEED_MEMORY;
  dbus_message_unref(reply);
  return result;
}

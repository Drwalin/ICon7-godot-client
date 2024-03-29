// #include <godot_cpp/godot.hpp>
// #include <godot_cpp/core/class_db.hpp>
//
// #include <gdextension_interface.h>
// #include <godot_cpp/core/defs.hpp>
// #include <godot_cpp/godot.hpp>

#include "../include/icon7-godot-client/gdextension_registration.h"

#include "../include/icon7-godot-client/Connections.hpp"
#include "../include/icon7-godot-client/ByteReader.hpp"
#include "../include/icon7-godot-client/ByteWriter.hpp"

void register_gameplay_types(godot::ModuleInitializationLevel p_level)
{
	if (p_level !=
		godot::ModuleInitializationLevel::MODULE_INITIALIZATION_LEVEL_SCENE) {
		return;
	}

	godot::ClassDB::register_class<GodotByteReader>();
	godot::ClassDB::register_class<GodotByteWriter>();
	godot::ClassDB::register_class<RpcFlags>();
	godot::ClassDB::register_class<RpcClient>();
	godot::ClassDB::register_class<RpcHost>();
	// REGISTER CLASSES HERE LATER
}

void unregister_gameplay_types(godot::ModuleInitializationLevel p_level)
{
	// DO NOTHING
}

extern "C" {

GDExtensionBool GDE_EXPORT
icon7_library_init(GDExtensionInterfaceGetProcAddress p_interface,
				   GDExtensionClassLibraryPtr p_library,
				   GDExtensionInitialization *r_initialization)
{
	godot::GDExtensionBinding::InitObject init_object(p_interface, p_library,
													  r_initialization);

	init_object.register_initializer(register_gameplay_types);
	init_object.register_terminator(unregister_gameplay_types);
	init_object.set_minimum_library_initialization_level(
		godot::ModuleInitializationLevel::MODULE_INITIALIZATION_LEVEL_SCENE);

	return init_object.init();
}
}

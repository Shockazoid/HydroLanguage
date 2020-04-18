//
//                 __  __            __           
//                / / / /__  __ ____/ /_____ ____ 
//               / /_/ // / / // __  // ___// __ \
//              / __  // /_/ // /_/ // /   / /_/ /
//             /_/ /_/ \__, / \__,_//_/    \____/ 
//                    /____/                      
//
//              The Hydro Programming Language
//
//        © 2020 Shockazoid, Inc. All Rights Reserved.
//

#include "SymbolTable.hpp"

namespace hydro
{

SymbolTable::SymbolTable() : Scope{"global"}
{
	defineBuiltinTypes();
}

SymbolTable::~SymbolTable() {}

void SymbolTable::defineBuiltinTypes()
{
    // define global package
    define(new PackageSymbol{new SimpleName{""}, new Scope{"@global", this}});
    
	// primitives
    /*
	define(new BuiltinTypeSymbol{"bool"});
	define(new BuiltinTypeSymbol{"char"});
	define(new BuiltinTypeSymbol{"byte"});
	define(new BuiltinTypeSymbol{"sbyte"});
	define(new BuiltinTypeSymbol{"ushort"});
	define(new BuiltinTypeSymbol{"short"});
	define(new BuiltinTypeSymbol{"uint"});
	define(new BuiltinTypeSymbol{"int"});
	define(new BuiltinTypeSymbol{"ulong"});
	define(new BuiltinTypeSymbol{"long"});
	define(new BuiltinTypeSymbol{"float"});
	define(new BuiltinTypeSymbol{"double"});

	// core
	define(new BuiltinTypeSymbol{"@system::Object"});
	define(new BuiltinTypeSymbol{"object"}); // alias
	define(new BuiltinTypeSymbol{"@system::Function"});
	define(new BuiltinTypeSymbol{"@system::Class"});
	define(new BuiltinTypeSymbol{"@system::String"});
	define(new BuiltinTypeSymbol{"@system::Array"});
	define(new BuiltinTypeSymbol{"@system::Dictionary"});
	define(new BuiltinTypeSymbol{"@system::Json"});
	define(new BuiltinTypeSymbol{"JSON"});
	define(new BuiltinTypeSymbol{"@system::Xml"});
	define(new BuiltinTypeSymbol{"XML"}); // alias
	define(new BuiltinTypeSymbol{"@system::Tuple"});
	define(new BuiltinTypeSymbol{"@system::Element"});
	define(new BuiltinTypeSymbol{"@system::Event"});
	define(new BuiltinTypeSymbol{"@system::Etype"});
	define(new BuiltinTypeSymbol{"@system::Action"});
	define(new BuiltinTypeSymbol{"@system::Enum"});

	// wrappers
	define(new BuiltinTypeSymbol{"@system::Boolean"});
	define(new BuiltinTypeSymbol{"@system::Uint8"});
	define(new BuiltinTypeSymbol{"@system::Int8"});
	define(new BuiltinTypeSymbol{"@system::Uint16"});
	define(new BuiltinTypeSymbol{"@system::Int16"});
	define(new BuiltinTypeSymbol{"@system::Uint32"});
	define(new BuiltinTypeSymbol{"@system::Int32"});
	define(new BuiltinTypeSymbol{"@system::Uint64"});
	define(new BuiltinTypeSymbol{"@system::Int64"});
	define(new BuiltinTypeSymbol{"@system::Single32"});
	define(new BuiltinTypeSymbol{"@system::Double64"});

	// reflection
	define(new BuiltinTypeSymbol{"@system.reflection::MemberInfo"});
	define(new BuiltinTypeSymbol{"@system.reflection::PropertyInfo"});
	define(new BuiltinTypeSymbol{"@system.reflection::GetterInfo"});
	define(new BuiltinTypeSymbol{"@system.reflection::SetterInfo"});
	define(new BuiltinTypeSymbol{"@system.reflection::MethodInfo"});
	define(new BuiltinTypeSymbol{"@system.reflection::ActionInfo"});
	define(new BuiltinTypeSymbol{"@system.reflection::EventInfo"});
	define(new BuiltinTypeSymbol{"@system.reflection::EtypeInfo"});
    */
}

} // namespace hydro

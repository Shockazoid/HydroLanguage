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

#ifndef __h3o_RuntimeContext__
#define __h3o_RuntimeContext__

#include <string>
#include <list>

#include "opcodes.hpp"
#include "hvalue.hpp"

namespace hydro
{

enum runtime_context_type
{
    hvm_runtime_function,
    hvm_runtime_action,
    hvm_runtime_class,
    hvm_runtime_interface,
    hvm_runtime_constructor,
    hvm_runtime_property,
    hvm_runtime_getter,
    hvm_runtime_setter,
    hvm_runtime_method
};


class RuntimeContext
{
protected:
	std::string _ownerPackage;
	class HClass *_ownerClass;

	virtual bool enforceAccess(class HvmEnv *env, uint16_t mod, RuntimeContext *callingContext);

	// abstract
	RuntimeContext();
	RuntimeContext(std::string ownerPackage);
	RuntimeContext(HClass *ownerClass);
	RuntimeContext(std::string ownerPackage, HClass *ownerClass);

public:
	~RuntimeContext();
	virtual std::string ownerPackage() const { return _ownerPackage; }
	virtual HClass *ownerClass() const { return _ownerClass; }
	virtual uint16_t modifier() const = 0;
	virtual std::string assemblyName() const = 0;
    virtual runtime_context_type type() const = 0;
};

} // namespace hydro

#endif /* __h3o_RuntimeContext__ */

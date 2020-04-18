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

#ifndef __h3o_Class__
#define __h3o_Class__

#include <iostream>

#include "HObject.hpp"
#include "../vm/VM_Class.hpp"
#include "../utility/classhelper.hpp"
#include "../vm/RuntimeContext.hpp"
#include "../vm/ClassAllocator.hpp"

namespace hydro
{

class HConstructor;
class HProperty;
class HMethod;

class HClass final : public HObject, public RuntimeContext
{
	friend class HvmEnv;
	friend class Runtime;
	friend class ClassNode;
    friend class VM;
    friend class HObjectContext;
    template <typename X>
    friend class ClassResource;
    friend struct CoreService;
    
private:
	const VM_Class *_vclass;
	HClass *_superClass;
	HConstructor *_constructor;
	RuntimeContext **_fields;
    HMethod **_methods;
    ClassAllocator *_allocator;
    
    std::string _name;
    std::string _qualifiedName;
    std::string _packageName;

	// reserved for the system::Class class
	HClass(class HvmEnv *env, const VM_Class *vclass, ClassAllocator *allocator = nullptr);

	// reserved for core classes that initialized before HVM run-time (i.e., system::Object, system::Class, system::String, system::List, .etc)
	HClass(HvmEnv *env, HClass *classClass, const VM_Class *vclass, ClassAllocator *allocator = nullptr);

	// used for all other runtime classes
	HClass(HvmEnv *env, HClass *classClass, const VM_Class *vclass, HClass *superClass, ClassAllocator *allocator = nullptr);
    
    void setup();
    HObject *alloc();

	uint16_t countFields();

public:
	virtual ~HClass();
    HObject *create();
	RuntimeContext *lookup(std::string name) const;
	const VM_Class *vclass() const { return _vclass; }
	HClass *superClass() const { return _superClass; }
    ClassAllocator *allocator() const { return _allocator; }
	HConstructor *constructor() const { return _constructor; }
	virtual uint16_t modifier() const override { return _vclass->modifier; }
    std::string name() { return _name; }
    std::string qualifiedName() { return _qualifiedName; }
    std::string packageName() { return _packageName; }
	virtual std::string assemblyName() const override { return _vclass->name; }
	virtual std::string toString() override;
    virtual runtime_context_type type() const override { return hvm_runtime_class; }
};

typedef object_ptr<HClass> hclass;

} // namespace hydro

#endif /* __h3o_Class__ */

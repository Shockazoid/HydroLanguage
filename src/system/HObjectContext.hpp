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

#ifndef __h3o_HObjectContext__
#define __h3o_HObjectContext__

#include <string>
#include <list>
#include <stack>
#include <vector>
#include <map>

#include "../memory/object_ptr.hpp"
#include "../vm/modifiers.hpp"
#include "../vm/hvalue.hpp"
#include "../vm/VMObject.hpp"
#include "../vm/VMContext.hpp"

namespace hydro
{

// forward references
class HvmEnv;
class RuntimeContext;

/**
 * The ObjectContext provides an interface that is separate yet dependent
 * of an object's instance for storing property values and methods
 * for interacting with the instance such as invoking methods via
 * "callProperty()".
 */
class HObjectContext final
{
	friend class HObject;
    friend class hydro_property;
    friend class HClass;
    friend class HInterface;
    friend class Context;
    friend class HEventContext;
    friend class HGetter;
    friend class HSetter;
    
private:
	struct property_space
	{
		const VMObject *data;
		std::string key;
		hvalue value;
		bool dynamic;
	};

	HObject *_instance;
	class HClass *_rclass;
    HClass *_rstatic; // for Class instances to lookup static methods
	uint16_t _size, _capacity;

	HObjectContext(HObject *instance, HClass *rclass);
    HObjectContext(HObject *instance, HClass *rclass, HClass *rstatic);

    static class HProperty *isProperty(RuntimeContext *member);
    static class HMethod *isMethod(RuntimeContext *member);
    
    void setup();

	bool checkAccess(RuntimeContext *callee, RuntimeContext *caller);
	void ensureCapacity(uint16_t capacity);
	void defineDynamicProperty(std::string name, const hvalue &value);
    
    bool isAncestorClass(HClass *classContext);
    bool isSubClass(HClass *classContext);
    RuntimeContext *lookup(std::string name, HClass *classContext);
    RuntimeContext *tryResolve(class VM *vm, class HvmContext *threadContext, std::string name, RuntimeContext *callingContext);
    
public:
    property_space **fields;
	~HObjectContext();
	bool getProperty(HvmEnv *env, VM *vm, RuntimeContext *callingContext, HvmContext *threadContext, std::string name, hvalue &value);
	bool setProperty(HvmEnv *env, VM *vm, RuntimeContext *callingContext, HvmContext *threadContext, std::string name, const hvalue &value);
	bool getIndex(HvmEnv *env, VM *vm, RuntimeContext *callingContext, HvmContext *threadContext, const hvalue &index, hvalue &value);
	bool setIndex(HvmEnv *env, VM *vm, RuntimeContext *callingContext, HvmContext *threadContext, const hvalue &index, const hvalue &value);
	bool callProperty(HvmEnv *env, VM *vm, RuntimeContext *callingContext, HvmContext *threadContext, std::string name, std::list<hvalue> &args, hvalue &result);
	bool callIndex(HvmEnv *env, VM *vm, RuntimeContext *callingContext, HvmContext *threadContext, const hvalue &index, std::list<hvalue> &args, hvalue &result);
	bool deleteProperty(HvmEnv *env, VM *vm, RuntimeContext *callingContext, HvmContext *threadContext, std::string name);
	bool deleteIndex(HvmEnv *env, VM *vm, RuntimeContext *callingContext, HvmContext *threadContext, const hvalue &index);
	HClass *classOf() const { return _rclass; }
    void copyToMap(std::map<std::string, hvalue> &map) const;
};

} // namespace hydro

#endif /* __h3o_HObjectContext__ */

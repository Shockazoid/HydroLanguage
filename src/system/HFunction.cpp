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

#include "HFunction.hpp"
#include "HvmEnv.hpp"

namespace hydro
{

HFunction::HFunction(HvmEnv *env, hclass functionClass, const FuncData *vfunc, function_glue *glue, std::string ownerPackage, hvalue thisObject) : HObject{env, functionClass}, RuntimeContext{ownerPackage}, _vfunc{vfunc}, _glue{glue}, _thisObject{thisObject}
{ 
}

HFunction::~HFunction()
{
	delete _glue;
}

hvalue HFunction::h3o_call(HvmEnv *env, VM *vm, HvmContext *cxt, hvalue thisObject, hobject args)
{
    if(hlist list = args.tryCast<HList>())
    {
        const uint32_t n = list->getLength();
        std::list<hvalue> argList{};

        // populate argument list
        for (uint32_t i = 0; i < n; i++)
            argList.push_back(list->get(i));

        // call function and return result
        return call(cxt, vm, argList, thisObject);
    }
    else
    {
        std::map<std::string, hvalue> argsMap;
        args->context()->copyToMap(argsMap);
        return call(cxt, vm, argsMap, thisObject);
    }
}

hvalue HFunction::call(hvalue thisObject, hobject args)
{
    return _env->call()->property(this, "call", thisObject, args);
}

hvalue HFunction::call(HvmContext *threadContext, VM *vm, std::list<hvalue> &args, hvalue thisObject)
{
	if (thisObject.empty() || is_null(thisObject))
		thisObject = _thisObject;
	return _glue->call(threadContext, this, vm, args, thisObject);
}

hvalue HFunction::call(HvmContext *threadContext, VM *vm, std::map<std::string, hvalue> &args, hvalue thisObject)
{
    if (thisObject.empty() || is_null(thisObject))
        thisObject = _thisObject;
    return _glue->call(threadContext, this, vm, args, thisObject);
}

} // namespace hydro

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

#include "HObject.hpp"
#include "HClass.hpp"
#include "HObjectContext.hpp"
#include "../vm/HvmEnv.hpp"
#include "../vm/VMObject.hpp"
#include "../memory/object_ptr.hpp"

namespace hydro
{

HObject::HObject(HvmEnv *env, HClass *clss) : _env{env}, _clss{clss}
{
    _cxt = new HObjectContext{this, _clss};
}

HObject::HObject(HvmEnv *env, HClass *clss, HClass *self) : _env{env}, _clss{clss}
{
    _cxt = nullptr; // will be initialized later
}

HObject::~HObject()
{
	delete _cxt;
}

void HObject::addRef()
{
    if (_nrefs == __UINT32_MAX__)
        return; // error
    _nrefs++;
}

void HObject::removeRef()
{
    if (_nrefs > 0)
    {
        _nrefs--;

        if (_nrefs == 0)
        {
            // kill
            //delete this;
        }
    }
}

void HObject::setup()
{
    if(!_cxt)
        _cxt = new HObjectContext{this, _clss};
}

void HObject::setup(HClass *rstatic)
{
    if(!_cxt)
        _cxt = new HObjectContext{this, _clss, rstatic};
}

void HObject::construct() {}

bool HObject::tryGetProperty(HvmEnv *env, VM *vm, HvmContext *threadContext, std::string name, hvalue &value)
{
	return false;
}

bool HObject::trySetProperty(HvmEnv *env, VM *vm, HvmContext *threadContext, std::string name, const hvalue &value)
{
	return false;
}

bool HObject::tryGetIndex(HvmEnv *env, VM *vm, HvmContext *threadContext, hvalue index, hvalue &value)
{
	return false;
}

bool HObject::trySetIndex(HvmEnv *env, VM *vm, HvmContext *threadContext, hvalue index, const hvalue &value)
{
	return false;
}

bool HObject::tryCallProperty(HvmEnv *env, VM *vm, HvmContext *threadContext, std::string name, std::list<hvalue> args, hvalue &result)
{

	return false;
}

bool HObject::tryCallIndex(HvmEnv *env, VM *vm, HvmContext *threadContext, hvalue index, std::list<hvalue> args, hvalue &result)
{
	return false;
}

bool HObject::tryDeleteProperty(HvmEnv *env, VM *vm, HvmContext *threadContext, std::string name)
{
	return false;
}

bool HObject::tryDeleteIndex(HvmEnv *env, VM *vm, HvmContext *threadContext, const hvalue index)
{
	return false;
}

std::string HObject::toString()
{
	std::string name = get_simple_name(_cxt->classOf()->assemblyName());
	return "[object " + name + "]";

	return "[object Object]";
}

bool HObject::equals(hvalue value)
{
    if(hobject o = value.tryCast<HObject>())
    {
        HObject *obj = o;
        return obj == this;
    }
    
    return false;
}

} // namespace hydro

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

#ifndef __h3o_Json__
#define __h3o_Json__

#include "HObject.hpp"
#include "HFunction.hpp"

namespace hydro
{

typedef object_ptr<class HJson> hjson;

class HJson final : public HObject
{
    template <typename X>
    friend class ClassResource;
    
	friend HvmEnv;

private:
	struct json_property
	{
		std::string key;
		hvalue value;
	};

	uint16_t _size;
	uint16_t _capacity;
	json_property **_props;

    void construct(hvalue dataArg);

    static std::string jsonEncodeString(std::string rawString);
    static std::string jsonEncodeValue(HvmEnv *env, hvalue jsonValue);

    void ensureCapacity(uint16_t capacity);

protected:
    
	virtual bool tryGetProperty(HvmEnv *env, VM *vm, HvmContext *threadContext, std::string name, hvalue &value) override;
	virtual bool trySetProperty(HvmEnv *env, VM *vm, HvmContext *threadContext, std::string name, const hvalue &value) override;
	virtual bool tryGetIndex(HvmEnv *env, VM *vm, HvmContext *threadContext, hvalue index, hvalue &value) override;
	virtual bool trySetIndex(HvmEnv *env, VM *vm, HvmContext *threadContext, hvalue index, const hvalue &value) override;
	virtual bool tryDeleteProperty(HvmEnv *env, VM *vm, HvmContext *threadContext, std::string name) override;
	virtual bool tryDeleteIndex(HvmEnv *env, VM *vm, HvmContext *threadContext, hvalue index) override;

public:
    HJson(HvmEnv *env, HClass *jsonClass);
	virtual ~HJson();
	static hjson parse(HvmEnv *env, std::string jsonString, hfunction reviver);
    static std::string stringify(HvmEnv *env, hjson instance);
	virtual std::string toString() override;
};

} // namespace hydro

#endif /* __h3o_Json__ */

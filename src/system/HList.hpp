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

#ifndef __h3o_List__
#define __h3o_List__

#include <list>

#include "HClass.hpp"

namespace hydro
{

typedef object_ptr<class HList> hlist;

class HList final : public HObject
{
    friend class HvmEnv;
    template <typename X>
    friend class ClassResource;
    template <typename X>
    friend struct NativeClassAllocator;
	friend void hydro_system_List_constructor(hlist instance, hlist elements);
    
private:
	hvalue *_buffer;
	uint32_t _len; // the length

protected:
	virtual bool tryGetIndex(HvmEnv *env, VM *vm, HvmContext *threadContext, hvalue index, hvalue &value) override;
	virtual bool trySetIndex(HvmEnv *env, VM *vm, HvmContext *threadContext, hvalue index, const hvalue &value) override;
	virtual bool tryCallIndex(HvmEnv *env, VM *vm, HvmContext *threadContext, hvalue index, std::list<hvalue> args, hvalue &result) override;
    
    HList(HvmEnv *env, hclass vclass);
     void construct();

public:
	virtual ~HList();
	void set(uint32_t index, hvalue element);
	hvalue get(uint32_t index) { return (index >= 0 && index < _len) ? _buffer[index] : (hvalue)undefined; }
	uint32_t push(hvalue element);
    hvalue pop();
	void setLength(uint32_t value);
	uint32_t getLength() {
        return _len; }
	virtual std::string toString() override;
	object_ptr<HList> clone() const;
};

typedef object_ptr<HList> hlist;

void hydro_system_List_constructor(hlist instance, hlist elements);

} // namespace hydro

#endif /* __h3o_List__ */

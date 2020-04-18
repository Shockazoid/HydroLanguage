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

#ifndef __h3o_MemorySpace__
#define __h3o_MemorySpace__

#include <cstdlib>

#include "hvalue.hpp"
#include "VMObject.hpp"
#include "Value.hpp"
#include "LocalVar.hpp"
#include "FuncData.hpp"
#include "VM_Class.hpp"

namespace hydro
{

class MemorySpace
{
protected:
	uint32_t _size;
	MemorySpace *_parent;

	void resize(uint32_t newSize);

public:
	Value **slots;
	MemorySpace(MemorySpace *parent = nullptr);
	MemorySpace(const FuncData *data, MemorySpace *parent = nullptr);
	~MemorySpace();
	void define(const VMObject *data, hvalue value = undefined);
	bool exists(std::string name) const;
	bool get(std::string name, hvalue &value); // dynamic load
	bool set(std::string name, hvalue value);  // dynamic storage
	bool del(std::string name);			 // delete
	MemorySpace *parent() const { return _parent; }
	uint32_t size() const { return _size; }
};

} // namespace hydro

#endif /* __h3o_MemorySpace__ */

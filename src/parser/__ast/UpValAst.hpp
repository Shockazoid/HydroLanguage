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

#ifndef __h3o_UpValAst__
#define __h3o_UpValAst__

#include "Ast.hpp"
namespace hydro
{

class UpValAst : public Ast
{
private:
	uint32_t _index;

public:
	bool local{false};
	UpValAst(uint32_t index) : Ast{}, _index{index} {}
	virtual ~UpValAst() {}
	uint32_t index() const { return _index; }
};

} // namespace hydro

#endif /* __h3o_UpValAst__ */

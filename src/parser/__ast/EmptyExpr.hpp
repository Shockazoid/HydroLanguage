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

#ifndef __h3o_EmptyExpr__
#define __h3o_EmptyExpr__

#include "Expr.hpp"

namespace hydro
{

class EmptyExpr : public Expr
{
public:
	EmptyExpr();
	virtual ~EmptyExpr();
};

typedef EmptyExpr *ast_empty;

} // namespace hydro

#endif /* __h3o_EmptyExpr__ */

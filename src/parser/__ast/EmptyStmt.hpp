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

#ifndef __h3o_EmptyStmt__
#define __h3o_EmptyStmt__

#include "Stmt.hpp"

namespace hydro
{

class EmptyStmt : public Stmt
{
public:
	EmptyStmt();
	virtual ~EmptyStmt();
};

typedef EmptyStmt *ast_estmt;

} // namespace hydro

#endif /* __h3o_EmptyStmt__ */

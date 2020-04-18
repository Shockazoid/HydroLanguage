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

#ifndef __h3o_MethodDecl__
#define __h3o_MethodDecl__

#include "FuncDecl.hpp"

namespace hydro
{

class MethodDecl : public FuncDecl
{
public:
	MethodDecl(lex_token token, ast_mod mods, ast_name name, ast_fargs args, ast_htype returnType, ast_stmt body = nullptr);
	virtual ~MethodDecl();
};

typedef MethodDecl *ast_method;

} // namespace hydro

#endif /* __h3o_MethodDecl__ */

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

#include "MethodDecl.hpp"

namespace hydro
{

MethodDecl::MethodDecl(lex_token token, ast_mod mods, ast_name name, ast_fargs args, ast_htype returnType, ast_stmt body) : FuncDecl{token, mods, name, args, returnType, body} {}

MethodDecl::~MethodDecl() {}

} // namespace hydro

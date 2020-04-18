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

#include "TypeReference.hpp"

namespace hydro
{

TypeReference::TypeReference(ast_name name) : Ast{}, _pkg{}, _name{name} {}

TypeReference::TypeReference(ast_package pkg, lex_token nqToken, ast_name name) : Ast{nqToken}, _pkg{pkg}, _name{name} {}

TypeReference::~TypeReference() {}

} // namespace hydro

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

#include "AstVisitor.hpp"

namespace hydro
{

AstVisitor::AstVisitor(Compiler *compiler) : _compiler{compiler} {}

AstVisitor::~AstVisitor() {}

void AstVisitor::appendError(std::string errorTxt, lex_token erroneousToken)
{
	_compiler->appendError(errorTxt, erroneousToken);
}

void AstVisitor::appendError(std::string errorTxt, Ast *erroneousNode)
{
	appendError(errorTxt, erroneousNode ? erroneousNode->token() : nullptr);
}

void AstVisitor::abrupt(Ast *unknown)
{
	throw unknown_ast{};
}

} // namespace hydro

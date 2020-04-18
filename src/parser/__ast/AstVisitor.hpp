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

#ifndef __h3o_AstVisitor__
#define __h3o_AstVisitor__

#include "../Compiler.hpp"
#include "Ast.hpp"

namespace hydro
{

class AstVisitor : public vpVisitor
{
protected:
	struct unknown_ast
	{
	};

	Compiler *_compiler;

	void appendError(std::string errorTxt, lex_token erroneousToken = nullptr);
	void appendError(std::string errorTxt, Ast *erroneousNode);
	void abrupt(Ast *unknown);

public:
	AstVisitor(Compiler *compiler);
	virtual ~AstVisitor();
	Compiler *compiler() const { return _compiler; }
};

} // namespace hydro

#endif /* __h3o_AstVisitor__ */

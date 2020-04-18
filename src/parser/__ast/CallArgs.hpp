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

#ifndef __h3o_CallArgs__
#define __h3o_CallArgs__

#include "Expr.hpp"
#include "SimpleName.hpp"

namespace hydro
{

class CallArgs : public Expr
{
public:
	CallArgs();
	virtual ~CallArgs();
	void append(ast_expr expr);
	unsigned count() const { return numChildren(); }
};

typedef CallArgs *ast_cargs;

class NamedArg : public Ast
{
private:
    SimpleName *_name;
    Expr *_value;
    
public:
    NamedArg(SimpleName *name, Expr *value) : _name{name}, _value{value} {}
    virtual ~NamedArg() {}
    SimpleName *name() const { return _name; }
    Expr *value() const { return _value; }
};

class NamedCallArgs : public CallArgs
{
public:
    NamedCallArgs() {}
    virtual ~NamedCallArgs() {}
    void append(NamedArg *arg) { addChild(arg); }
};

} // namespace hydro

#endif /* __h3o_CallArgs__ */

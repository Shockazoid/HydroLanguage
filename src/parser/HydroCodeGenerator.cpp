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

#include "HydroCodeGenerator.hpp"
#include "HydroCompiler.hpp"

namespace hydro
{

HydroCodeGenerator::HydroCodeGenerator(HydroCompiler *compiler, Scope *globals, Runtime *runtime) : HydroVisitor{compiler, globals}, _mode{h3o_compile_aot}, _nlabels{0}, _h3oasm{""}, _tab{0}, _runtime{runtime}, _program{nullptr}, _line{0}, _source{""} {}

HydroCodeGenerator::~HydroCodeGenerator() {}

void HydroCodeGenerator::tabUp()
{
	_tab++;
}

void HydroCodeGenerator::tabDown()
{
	if (_tab > 0)
		_tab--;
}

void HydroCodeGenerator::emit(std::string instr)
{
	// emit tabs
	uint32_t t = _tab;
	while (t-- > 0)
		_h3oasm += '\t';

	// emit instruction
	_h3oasm += instr;

	// emit line
	_h3oasm += '\n';
}

void HydroCodeGenerator::emit(std::string instr, std::string opnd1)
{
	emit(instr + " " + opnd1);
}

void HydroCodeGenerator::emit(std::string instr, std::string opnd1, std::string opnd2)
{
	emit(instr + " " + opnd1, opnd2);
}

void HydroCodeGenerator::emit(std::string instr, std::string opnd1, std::string opnd2, std::string opnd3)
{
	emit(instr + " " + opnd1, opnd2, opnd3);
}

void HydroCodeGenerator::comment(std::string content)
{
	emit(";", content);
}

std::string HydroCodeGenerator::loadClass(SimpleName *id)
{
	std::string val = id->value();

	if (val == "object")
		return "system::Object";
	else if (val == "byte")
		return "system::Uint8";
	else if (val == "sbyte")
		return "system::Int8";
	else if (val == "ushort")
		return "system::Uint16";
	else if (val == "short")
		return "system::Int16";
	else if (val == "uint")
		return "system::Uint32";
	else if (val == "int")
		return "system::Int32";
	else if (val == "ulong")
		return "system::Uint64";
	else if (val == "long")
		return "system::Int64";
	else if (val == "float")
		return "system::Single";
	else if (val == "double")
		return "system::Double";
	else if (val == "bool")
		return "system::Boolean";
	else if (val == "char")
		return "system::Char";
	else if (val == "string")
		return "system::String";
	else if (val == "number")
		return "system::Number";

	appendError("Could not resolve '" + val + "' to a type.", id->token());
	return "";
}

std::string HydroCodeGenerator::nameQualifierToAssemblyName(BinaryOp *nq)
{
	std::string val = "";
	int32_t type = nq->lhs()->tokenType();

	// name qualifier
	if (type == package_tkn)
	{
		// package
		if (SimpleName *id = nq->rhs()->tryCast<SimpleName>())
		{
			val = nq->lhs()->tokenValue().substr(1);
			std::regex pattern("\\.");
			val = regex_replace(val, pattern, "/");
			return "@" + val + "/" + id->value();
		}
		else
			appendError("Expecting a name on the right hand side of the name qualifier.");
	}
	else
		appendError("Expecting a package specifier on the right hand side of the name qualifier.");

	return "";
}

std::string HydroCodeGenerator::packageToAssemblyName(PackageSymbol *pkg)
{
	if (!pkg)
		return "";

    std::string lit = pkg->fullPackageName();
	const uint32_t n = (uint32_t)lit.length();
	std::string name = "";
    
	for (int i = 0; i < n; i++)
	{
		if (lit[i] == '.')
			name += '/';
		else
			name += lit[i];
	}
    
	return "@" + name;
}

Symbol *HydroCodeGenerator::resolveLocal(std::string name)
{
	if (_currentScope)
		return _currentScope->resolve(name);

	// fail
	return nullptr;
}

Symbol *HydroCodeGenerator::resolveUpvalue(std::string name)
{
	Scope *scope = _currentScope;
	bool local = true;

	while (scope)
	{
		if (Symbol *symbol = scope->get(name))
			return symbol; // scope with symbol
		scope = scope->parent();
		local = false;
	}

	// fail
	return nullptr;
}

void HydroCodeGenerator::tryResolveTypeBindings(ClassDecl *clss)
{
	for (Name *type : clss->types())
	{
        Symbol *symbol = nullptr;
        
        if(SimpleName *simple = dynamic_cast<SimpleName *>(type))
        {
            // try resolve simple name
            symbol = tryResolve(simple);
        }
        else if(QualifiedName *qualified = dynamic_cast<QualifiedName *>(type))
        {
            // try resolve qualified name
            symbol = tryResolve(qualified);
        }
        
        if(symbol)
            type->binding = symbol;
	}
}

ClassSymbol *HydroCodeGenerator::tryResolveSuperClass(ClassDecl *clss)
{
    ClassSymbol *superClass = nullptr;
    
    for(Name *type : clss->types())
    {
        if(ClassSymbol *symbol = dynamic_cast<ClassSymbol *>(type->binding))
        {
            if(superClass)
            {
                appendError("Multipe inheritance is prohibited.", type);
                continue;
            }
            
            // success!
            superClass = symbol;
        }
    }
    
    if(!superClass)
    {
        // fail
        return nullptr;
    }
    
	return superClass;
}

bool HydroCodeGenerator::parentDeclIsClass(Ast *child)
{
	while (child->parent())
	{
		if (dynamic_cast<Decl *>(child->parent()))
		{
			if (dynamic_cast<ClassDecl *>(child->parent()))
				return true;

			// fail
			break;
		}

		child = child->parent();
	}

	// fail
	return false;
}

void HydroCodeGenerator::emitNamedCall(std::string opcode, Call *call, NamedCallArgs *args)
{
    Ast *child = args->firstChild();
    
    // first pass
    while(child)
    {
        NamedArg *arg = child->tryCast<NamedArg>();
        Expr *val = arg->value();
        visit(val);
        child = child->next();
    }
    
    emit(opcode, std::to_string(args->count()));
    
    child = args->lastChild();
    
    tabUp();
    
    // second pass (reverse order)
    while(child)
    {
        NamedArg *arg = child->tryCast<NamedArg>();
        std::string name = arg->name()->value();
        emit(name);
        child = child->prev();
    }
    
    tabDown();
}

void HydroCodeGenerator::assign(BinaryOp *op)
{
	// (local) = (value)
	// (local) = ((some.field).value)
	// (local) = (((some.field).property).value)
	// (object.property) =
	// (object.instance[index]) = value
	Expr *subject = op->lhs();
	Expr *value = op->rhs();

	if (SimpleName *id = subject->tryCast<SimpleName>())
	{
		visit(value);
        
        if(Symbol *symbol = resolve(id->value()))
        {
            if(PropertySymbol *prop = dynamic_cast<PropertySymbol *>(symbol))
            {
                ClassSymbol *clss = prop->owner();
                
                if(clss)
                {
                    emit("this");
                    visit(value);
                    emit("setpropertyx", id->value());
                    return;
                }
            }
        }

		if (_mode == h3o_compile_aot)
		{
			Symbol *local = resolveLocal(id->value());

			if (local)
			{
                if(dynamic_cast<FuncSymbol *>(local))
                {
                    appendError("Overriding constant function.", id);
                }
                
				// set local by index
				emit("store", std::to_string(local->slot));
				return;
			}
		}

		// set local by name
		emit("storex", id->value());
	}
	else if (BinaryOp *bin = subject->tryCast<BinaryOp>())
	{
		Expr *lhs = bin->lhs();
		Expr *rhs = bin->rhs();

		// set property
		if (bin->tokenValue() == ".")
		{
			if (SimpleName *id = rhs->tryCast<SimpleName>())
			{
				// TODO find method
				if (false)
				{
					// if method located
				}
				else
				{
					visit(lhs);
					visit(value);
					emit("setpropertyx", id->value());
				}
			}
			else
			{
				// alternative subscript syntax
				visit(lhs);
				visit(rhs);
				visit(value);
				emit("setindex");
			}
		}
		else if (bin->tokenValue() == "[")
		{
			// subscript assignment
			lhs->accept(this);
			rhs->accept(this);
			value->accept(this);
			emit("setindex");
		}
		else
			appendError("Left hand side is not assignable.", bin->token());
	}
	else
		appendError("Left hand side is not assignable.", op->token());
}

void HydroCodeGenerator::chain(BinaryOp *op)
{
	Expr *lhs = op->lhs();
	Expr *rhs = op->rhs();

	if (Call *call = rhs->tryCast<Call>())
	{
		// method chain
		// foo->bar() -> barAgain()
		CallArgs *args = call->args();
		const uint32_t nargs = args->numChildren();
		visitCallArgs(args);

		if (SimpleName *id = call->lhs()->tryCast<SimpleName>())
		{
			// TODO find method
			if (false)
			{
			}
			else
			{
				//visit(call->lhs());
				emit("chainx", id->value(), std::to_string(nargs));
			}
		}
		else
		{
			call->lhs()->accept(this); // key
			lhs->accept(this);
			emit("ichain", std::to_string(nargs));
		}

		// success!
		return;
	}
	else if (BinaryOp *a = rhs->tryCast<BinaryOp>())
	{
		// is property chain (?)
		if (isAssignment(a))
		{
			// handle assignment
		}
		else
		{
		}
	}
	else
	{
		// handle expression
	}

	// fail
	appendError("Expecting invocation or assignment on the right side of chain operator.", rhs->token());
}

std::string HydroCodeGenerator::createModString(Modifier *mods)
{
    if(!mods)
        return ""; // nil
	return createModString(mods->mask());
}

std::string HydroCodeGenerator::createModString(uint16_t mods)
{
	std::vector<std::string> values{};

	if (has_modifier(mods, mod_public))
		values.push_back("public");

	if (has_modifier(mods, mod_internal))
		values.push_back("internal");

	if (has_modifier(mods, mod_protected))
		values.push_back("public");

	if (has_modifier(mods, mod_private))
		values.push_back("private");

	if (has_modifier(mods, mod_restricted))
		values.push_back("restricted");

	if (has_modifier(mods, mod_override))
		values.push_back("override");

	if (has_modifier(mods, mod_abstract))
		values.push_back("abstract");

	if (has_modifier(mods, mod_dynamic))
		values.push_back("dynamic");

	if (has_modifier(mods, mod_static))
		values.push_back("static");

	if (has_modifier(mods, mod_native))
		values.push_back("native");

	if (has_modifier(mods, mod_final))
		values.push_back("final");

	std::string s = "";

	for (std::string val : values)
		if (s != "")
			s += "|" + val;
		else
			s += val;

	return s.empty() ? s : "<" + s + ">";
}

std::string HydroCodeGenerator::toAssemblyType(TypeSpec *type)
{
	return ": *"; // untyped
}

void HydroCodeGenerator::pushLoop(std::string start, std::string alt, std::string done)
{
	_loops.push_back(new HydroCodeGenerator::loop_data(start, alt, done));
}

void HydroCodeGenerator::popLoop()
{
	if (_loops.size() > 0)
		_loops.pop_back();
}


Symbol *HydroCodeGenerator::getEnclosingSymbol()
{
    return getEnclosingSymbol(_currentScope);
}

Symbol *HydroCodeGenerator::getEnclosingSymbol(Scope *scope)
{
   while (scope)
    {
        if (scope->owner())
            return scope->owner();
        
        scope = scope->parent();
    }
    
    // fail
    return nullptr;
}

FuncSymbol *HydroCodeGenerator::getEnclosingFunc()
{
    return getEnclosingFunc(_currentScope);
}

FuncSymbol *HydroCodeGenerator::getEnclosingFunc(Scope *scope)
{
    while (scope)
    {
        if (FuncSymbol *owner = dynamic_cast<FuncSymbol *>(scope->owner()))
            return owner;
        
        // try again
        scope = scope->parent();
    }
    
    // fail
    return nullptr;
}


ClassSymbol *HydroCodeGenerator::getParentClass()
{
    return _currentScope ? getParentClass(_currentScope) : nullptr;
}

ClassSymbol *HydroCodeGenerator::getParentClass(Scope *scope)
{
    scope = scope ? scope->parent() : nullptr;
    
    while (scope)
    {
        if (ClassSymbol *owner = dynamic_cast<ClassSymbol *>(scope->owner()))
            return owner;
        
        if(scope->owner())
            break; // enclosing symbol is not a class
        
        scope = scope->parent();
    }
    
    // fail
    return nullptr;
}

ClassSymbol *HydroCodeGenerator::getEnclosingClass()
{
    return getEnclosingClass(_currentScope);
}

ClassSymbol *HydroCodeGenerator::getEnclosingClass(Scope *scope)
{
    while (scope)
    {
        if (ClassSymbol *owner = dynamic_cast<ClassSymbol *>(scope->owner()))
            return owner;
        
        // try again
        scope = scope->parent();
    }
    
    // fail
    return nullptr;
}

ConstructorSymbol *HydroCodeGenerator::getEnclosingConstructor()
{
    return getEnclosingConstructor(_currentScope);
}

ConstructorSymbol *HydroCodeGenerator::getEnclosingConstructor(Scope *scope)
{
    while (scope)
    {
        if (ConstructorSymbol *owner = dynamic_cast<ConstructorSymbol *>(scope->owner()))
            return owner;
        
        if(scope->owner())
            break; // enclosing symbol is not a constructor

        scope = scope->parent();
    }
    
    // fail
    return nullptr;
}

PropertySymbol *HydroCodeGenerator::getEnclosingProperty()
{
    return getEnclosingProperty(_currentScope);
}

PropertySymbol *HydroCodeGenerator::getEnclosingProperty(Scope *scope)
{
    while (scope)
    {
        if (PropertySymbol *owner = dynamic_cast<PropertySymbol *>(scope->owner()))
            return owner;
        
        if(scope->owner())
            break; // enclosing symbol is not a constructor

        scope = scope->parent();
    }
    
    // fail
    return nullptr;
}

MethodSymbol *HydroCodeGenerator::getEnclosingMethod()
{
    return getEnclosingMethod(_currentScope);
}

MethodSymbol *HydroCodeGenerator::getEnclosingMethod(Scope *scope)
{
    while (scope)
    {
        if (MethodSymbol *owner = dynamic_cast<MethodSymbol *>(scope->owner()))
            return owner;
        
        if(scope->owner())
            break; // enclosing symbol is not a constructor

        scope = scope->parent();
    }
    
    // fail
    return nullptr;
}

int32_t HydroCodeGenerator::addSlot()
{
    FuncSymbol *func = getEnclosingFunc();
    
    if(func)
    {
        FuncDecl *fn = dynamic_cast<FuncDecl *>(func->owner());
        assert(fn);
        return fn->slots++;
    }
    else if(_program)
    {
        return _program->slots++;
    }
    else
    {
        // error (should never be reached)
        throw std::runtime_error{"Scope does not exist."};
    }
}

int32_t HydroCodeGenerator::getLocalIndex(Symbol *symbol, Scope *scopeToUse)
{
    if(!scopeToUse)
        return -1; // invalid
    
    return scopeToUse->indexOf(symbol);
}

HydroCodeGenerator::loop_data *HydroCodeGenerator::startLoop()
{
	return startLoop(false);
}

HydroCodeGenerator::loop_data *HydroCodeGenerator::startLoop(bool alt)
{
	std::string s = newLabel();
	std::string c = alt ? newLabel() : "";
	std::string d = newLabel();
	pushLoop(s, c, d);
	emit(s + ":");
	return _loops.back();
}

HydroCodeGenerator::loop_data *HydroCodeGenerator::loopCond()
{
	loop_data *data = _loops.back();
	emit("brf", data->end);
	return data;
}

HydroCodeGenerator::loop_data *HydroCodeGenerator::loopAlt()
{
	HydroCodeGenerator::loop_data *data = _loops.back();
	emit(data->alt + ":");
	return data;
}

HydroCodeGenerator::loop_data *HydroCodeGenerator::endLoop()
{
	HydroCodeGenerator::loop_data *data = _loops.back();
	popLoop();
	emit(data->end + ":");
	return data;
}

HydroCodeGenerator::loop_data *HydroCodeGenerator::jumpLoop()
{
	HydroCodeGenerator::loop_data *data = _loops.back();
	emit("br", data->begin);
	return data;
}

HydroCodeGenerator::loop_data *HydroCodeGenerator::loopBack(int k)
{
	k = ((int)_loops.size()) - k;
	return k >= 0 && k < _loops.size() ? _loops[k] : nullptr;
}

HydroCodeGenerator::loop_data *HydroCodeGenerator::breakLoop(int k, BreakStmt *brk)
{
	HydroCodeGenerator::loop_data *data = loopBack(k);
	if (data == nullptr)
		appendError("Break depth is out of bounds." + std::to_string(_loops.size()), brk->token()->next());
	else
		emit("br", data->end);
	return data;
}

HydroCodeGenerator::loop_data *HydroCodeGenerator::continueLoop(int k, ContinueStmt *cont)
{
	HydroCodeGenerator::loop_data *data = loopBack(k);
	if (data == nullptr)
		appendError("Continue depth is out of bounds.", cont->token()->next());
	else
		emit("br", data->alt == "" ? data->begin : data->alt);
	return data;
}

Call *HydroCodeGenerator::findSuperCall(Stmt *body)
{
	if (dynamic_cast<Block *>(body))
	{
		Ast *child = body->firstChild();

		if (Stmt *stmt = dynamic_cast<Stmt *>(child))
		{
			child = stmt->firstChild();

			// super() ?
			if (Call *call = dynamic_cast<Call *>(child))
			{
				if (SimpleName *id = dynamic_cast<SimpleName *>(call->lhs()))
				{
					if (id->value() == "super")
						return call;
				}
				else if (BinaryOp *op = dynamic_cast<BinaryOp *>(call->rhs()))
				{
					// this.super() ?
					if (op->tokenValue() == ".")
					{
						if (SimpleName *id = dynamic_cast<SimpleName *>(op->rhs()))
						{
							if (id->value() == "super")
							{
								if (SimpleName *id = dynamic_cast<SimpleName *>(op->lhs()))
								{
									if (id->value() == "this")
										appendError("Informal call to super constructor using this object.", op->rhs()->token());
								}
							}
						}
					}
				}
			}
		}
	}

	// fail
	return nullptr;
}

void HydroCodeGenerator::visitProgram(Program *prog)
{
	// visit program statements
    _program = prog;
	prog->visitChildren(this);
}

void HydroCodeGenerator::visitStmt(Stmt *stmt)
{
	HydroVisitor::visitStmt(stmt);
	// TODO pop statement if eval()
}

void HydroCodeGenerator::visitIdentifier(SimpleName *id)
{
	if (id->value() == "this")
	{
		emit("this");
		return;
	}
    
    // try resolve locally or using imports
    Symbol *symbol = resolve(id);
    
    if(!symbol)
    {
        // resolve using all resources or fail with error (does not abort)
        symbol = tryResolve(id);
    }
    
    if (symbol)
    {
        // load local by index
        // TODO only emit this if local
        if(auto var = isVar(symbol))
        {
            // local variable
            if(var->slot >= 0)
            {
                // load by slot name
                emit("load", std::to_string(var->slot));
            }
            else
            {
                // error
                appendError("Accessing local variable '" + var->name()->value() + "' before the the variable is declared.", id);
            }
        }
        else if(auto prop = isProperty(symbol))
        {
            // property
            emit("this");
            emit("getpropertyx", prop->name()->value());
        }
        else if(auto meth = isMethod(symbol))
        {
            // method
            emit("this");
            emit("getpropertyx", meth->name()->value());
        }
        else if(auto func = isFunc(symbol))
        {
            if(func->package())
            {
                // load from runtime
                emit("rload", funcToAssemblyName(func->package(), func));
            }
            else
            {
                if(func->slot >= 0)
                {
                    emit("load", std::to_string(func->slot));
                }
                else
                {
                    // forward reference
                    emit("loadx", func->name()->value());
                }
            }
        }
        else if(EventSymbol *evnt = isEvent(symbol))
        {
            if(evnt->package())
            {
                // load from runtime
                emit("rload", eventToAssemblyName(evnt->package(), evnt));
            }
            else
            {
                if(evnt->slot >= 0)
                {
                    emit("load", std::to_string(evnt->slot));
                }
                else
                {
                    // forward reference
                    emit("loadx", evnt->name()->value());
                }
            }
        }
        else if(ClassSymbol *clss = isClass(symbol))
        {
            if(clss->package())
            {
                // load from runtime
                emit("rload", classToAssemblyName(clss->package(), clss));
            }
            else
            {
                // local class constant
                throw 1; // not yet supported
            }
        }
        else
        {
            // unresolved symbol
            emit("loadx", id->value());
        }
        return;
    }
	else if (evalMode())
	{
        if(ClassSymbol *clss = dynamic_cast<ClassSymbol *>(symbol))
        {
            if(PackageSymbol *pkg = dynamic_cast<PackageSymbol *>(clss->parent()))
            {
                // load definition
                emit("rload", classToAssemblyName(pkg, clss));
            }
            else
            {
                // emit by index once implemented
                emit("loadx", id->value());
            }
        }
        else if(PropertySymbol *prop = dynamic_cast<PropertySymbol *>(symbol))
        {
            ClassSymbol *owner = getEnclosingClass();
            
            if(!owner)
            {
                appendError("Illegal access to member '" + id->value() + "' outside of enclosing class.", id);
                return; // abort
            }
            
            emit("this");
            emit("getpropertyx", prop->name()->value());
        }
        else if(MethodSymbol *meth = dynamic_cast<MethodSymbol *>(symbol))
        {
            ClassSymbol *owner = getEnclosingClass();
            
            if(!owner)
            {
                appendError("Illegal access to member '" + id->value() + "' outside of enclosing class.", id);
                return; // abort
            }
            
            emit("this"); // push this
            emit("getpropertyx", meth->name()->value());
        }
        else if(FuncSymbol *func = dynamic_cast<FuncSymbol *>(symbol))
        {
            if(PackageSymbol *pkg = dynamic_cast<PackageSymbol *>(func->parent()))
            {
                // load definition
                emit("rload", funcToAssemblyName(pkg, func));
            }
            else
            {
                // emit by index once implemented
                emit("loadx", id->value());
            }
        }
        else
        {
            // load local by name
            emit("loadx", id->value());
        }
	}
    else
    {
        // unknwon symbol
        appendError("Could not resolve reference to unknown symbol '" + id->value() + "'");
    }
}

void HydroCodeGenerator::visitNull(Literal *expr)
{
	emit("nil");
}

void HydroCodeGenerator::visitUndefined(Literal *expr)
{
	emit("und");
}

void HydroCodeGenerator::visitPackage(PackageDecl *pkg)
{
    pushScope(pkg);
    HydroVisitor::visitPackage(pkg);
    popScope();
}

void HydroCodeGenerator::visitIdentity(Literal *expr)
{
	// TODO emit identity
}

void HydroCodeGenerator::visitBool(Literal *expr)
{
	emit(expr->tokenValue());
}

void HydroCodeGenerator::visitChar(Literal *expr)
{
	emit("push", expr->tokenValue());
}

void HydroCodeGenerator::visitString(Literal *expr)
{
	emit("push", expr->tokenValue());
}

void HydroCodeGenerator::visitStringTemplate(Literal *tpl)
{
	const long n = tpl->numChildren();
	emit("push", "\"\""); // push empty string

	for (uint32_t i = 0; i < n; i++)
	{
		Expr *expr = tpl->getChildAt(i)->tryCast<Expr>();

		// if string expression
		if (expr->tokenType() == string_tkn)
		{
			// todo unescape {
			emit("push", unescapeTemplateString(expr->token()));
			emit("add");
		}
		else
		{
			expr->accept(this);
			emit("add");
		}
	}
}

void HydroCodeGenerator::visitUint(Literal *expr)
{
	emit("push", expr->tokenValue());
}

void HydroCodeGenerator::visitInt(Literal *expr)
{
	emit("push", expr->tokenValue());
}

void HydroCodeGenerator::visitHexadecimal(Literal *expr)
{
	emit("push", expr->tokenValue());
}

void HydroCodeGenerator::visitBinary(Literal *expr)
{
	emit("push", expr->tokenValue());
}

void HydroCodeGenerator::visitOctal(Literal *expr)
{
	emit("push", expr->tokenValue());
}

void HydroCodeGenerator::visitUshort(Literal *expr)
{
	emit("push", expr->tokenValue());
}

void HydroCodeGenerator::visitShort(Literal *expr)
{
	emit("push", expr->tokenValue());
}

void HydroCodeGenerator::visitUlong(Literal *expr)
{
	emit("push", expr->tokenValue());
}

void HydroCodeGenerator::visitLong(Literal *expr)
{
	emit("push", expr->tokenValue());
}

void HydroCodeGenerator::visitFloat(Literal *expr)
{
	emit("push", expr->tokenValue());
}

void HydroCodeGenerator::visitDouble(Literal *expr)
{
	emit("push", expr->tokenValue());
}

void HydroCodeGenerator::visitInfinity(Literal *expr)
{
	emit("inf");
}

void HydroCodeGenerator::visitNaN(Literal *expr)
{
	emit("nan");
}

void HydroCodeGenerator::visitRegex(Literal *expr)
{
}

void HydroCodeGenerator::visitObject(ObjectInit *expr)
{
	// emit object
	emit("object");

	// emit child properties
	HydroVisitor::visitObject(expr);
}

void HydroCodeGenerator::visitObjectProp(ObjectInitProperty *prop)
{
	visit(prop->value());
	emit("objectinit", prop->name()->value());
}

void HydroCodeGenerator::visitJson(JsonInit *json)
{

	// emit json
	emit("json");

	// emit json properties
	HydroVisitor::visitJson(json);
}

void HydroCodeGenerator::visitJsonProp(JsonInitProperty *prop)
{
	visitJsonValue(prop->value());		   // get value operand
	emit("jsoninit", prop->key()->tokenValue()); // emit json proeprty
}

void HydroCodeGenerator::visitJsonValue(Expr *expr)
{
	if (expr->tokenType() == int_tkn)
	{
		emit("push", expr->tokenValue() + "D"); // cast to double
	}
	else
	{
		// visit string, double, list, json, boolean, and identifier
		visit(expr);
	}
}

void HydroCodeGenerator::visitArray(ArrayInit *arr)
{
	// emit array list
	emit("list");

	// visit child elements
	HydroVisitor::visitArray(arr);
}

void HydroCodeGenerator::visitArrayElement(Expr *elm)
{
	// array list push
	visit(elm);
	emit("listpush");
}

void HydroCodeGenerator::visitDict(DictInit *dict)
{
	// emit dictionary
	emit("dict");

	// emit child entries
	HydroVisitor::visitDict(dict);
}

void HydroCodeGenerator::visitDictEntry(DictInitEntry *entry)
{
	visit(entry->key());
	visit(entry->value());
	emit("dictinit");
}

void HydroCodeGenerator::visitXml(XmlInit *xml)
{
	Ast *child = xml->firstChild();

	while (child)
	{
		if (ast_xml xmlChild = child->tryCast<XmlInit>())
			visit(xmlChild);
		child = child->next();
	}
}

void HydroCodeGenerator::visitLambda(AFunc *lambda)
{
	visit(lambda->body());
}

void HydroCodeGenerator::visitUnaryOp(UnaryOp *op)
{
	std::string val = op->tokenValue();

	if (val == "new")
	{
		// allocation
		if (Call *call = op->expr()->tryCast<Call>())
		{
			Expr *lhs = call->lhs();
			CallArgs *args = call->args();

			if (SimpleName *id = lhs->tryCast<SimpleName>())
			{
				// resolve name from scope
				// TODO use "onew" for unresolved locals and "newx" for resolved locals

				// emit
                Symbol *symbol = tryResolve(id);
                
                if(ClassSymbol *clss = dynamic_cast<ClassSymbol *>(symbol))
                {
                    if(clss->package())
                    {
                        visitCallArgs(args);
                        emit("new", classToAssemblyName(clss->package(), clss), std::to_string(args->numChildren()));
                        return;
                    }
                }
                
                visitCallArgs(args);
                emit("loadx", id->value());
                emit("onew", std::to_string(args->numChildren()));
				return;
			}
			else if (BinaryOp *bin = lhs->tryCast<BinaryOp>())
			{
				if (bin->tokenValue() == "::")
				{
					// emit
					visitCallArgs(args);
					emit("new", nameQualifierToAssemblyName(bin), std::to_string(args->numChildren()));
					return;
				}
				else
					appendError("Expecting a name qualifier.", bin->token());
			}
		}
		else
			appendError("The 'new' operator must be followed by a call to a constructor.", op);
	}
	else if (val == "delete")
	{
		Expr *exprToDelete = op->expr();

		if (SimpleName *localName = exprToDelete->tryCast<SimpleName>())
		{
			emit("del", localName->value());
			// TODO gdelete = delete global
			return;
		}
		else if (BinaryOp *bin = exprToDelete->tryCast<BinaryOp>())
		{

			// delete = delete local
			// fdelete = delete field
			// xdelete = delete index
		}
	}
	else
	{
		op->visitChildren(this);

		if (val == "+")
		{
			emit("pos");
		}
		else if (val == "-")
		{
			emit("neg");
		}
		else if (val == "!")
		{
			emit("lnt");
		}
		else if (val == "~")
		{
			emit("bnt");
		}
        else if (val == "++")
        {
            if(SimpleName *id = op->expr()->tryCast<SimpleName>())
            {
                emit("inc", id->value());
            }
            else if(BinaryOp *bin = op->tryCast<BinaryOp>())
            {
                
            }
        }
        else if (val == "--")
        {
            if(SimpleName *id = op->expr()->tryCast<SimpleName>())
            {
                emit("dec", id->value());
            }
            else if(BinaryOp *bin = op->tryCast<BinaryOp>())
            {
                
            }
        }
		else if (val == "typeof")
		{
			emit("typeof");
		}
		else if (val == "throw")
		{
			emit("throw");
		}
		else
			appendError("'" + val + "' is not a unary operator.", op->token());
	}
}

void HydroCodeGenerator::visitBinaryOp(BinaryOp *op)
{
	std::string val = op->tokenValue();
	Expr *lhs = op->lhs();
	Expr *rhs = op->rhs();

	if (Call *call = op->tryCast<Call>())
	{
		visitCall(call);
		return;
	}
	else if (val == "[")
	{
		// indexer
		visit(lhs);
		visit(rhs);
		emit("getindex");
	}
	else if (val == ".")
	{
		// dot
		if (SimpleName *id = rhs->tryCast<SimpleName>())
		{
			// TODO get member
			if (false)
			{
			}
			else
			{
				visit(lhs);
				emit("getpropertyx", id->value());
			}
		}
		else
		{
			// alternative subscript syntax
			visit(lhs);
			visit(rhs);
			emit("getindex");
		}
	}
	else if (val == "::")
	{
		// name qualifier
		emit("rload", nameQualifierToAssemblyName(op));
	}
	else if (isAssignment(op))
	{
		// assignment
		assign(op);
	}
	else if (val == "->")
	{
		// method chaining
		chain(op);
	}
	else if (val == "&&")
	{
		visit(lhs);
		std::string label = newLabel();
		emit("lnd", label);
		visit(rhs);
		emit(label + ":");
	}
	else if (val == "||")
	{
		lhs->accept(this);
		std::string label = newLabel();
		emit("lor", label);
		rhs->accept(this);
		emit(label + ":");
	}
	else
	{
		// visit lhs and rhs
		op->visitChildren(this);

		if (val == "**")
		{
			emit("exp");
		}
		else if (val == "*")
		{
			emit("mul");
		}
		else if (val == "/")
		{
			emit("div");
		}
		else if (val == "%")
		{
			emit("mod");
		}
		else if (val == "+")
		{
			emit("add");
		}
		else if (val == "-")
		{
			emit("sub");
		}
		else if (val == ">")
		{
			emit("gt");
		}
		else if (val == ">=")
		{
			emit("gte");
		}
		else if (val == "<")
		{
			emit("lt");
		}
		else if (val == "<=")
		{
			emit("lte");
		}
		else if (val == "==")
		{
			emit("leq");
		}
		else if (val == "!=")
		{
			emit("liq");
		}
		else if (val == ">>")
		{
			emit("brs");
		}
		else if (val == "<<")
		{
			emit("bls");
		}
		else if (val == "^")
		{
			emit("bxr");
		}
		else if (val == "&")
		{
			emit("bnd");
		}
		else if (val == "|")
		{
			emit("bor");
		}
		else if (val == "is")
		{
			emit("is");
		}
		else if (val == "as")
		{
			emit("as");
		}
		else if (val == "instanceof")
		{
			emit("instanceof");
		}
		else if (isAssignment(op))
		{
			assign(op);
		}
		else
			throw std::runtime_error("Expecting a binary operator.");
	}
}

void HydroCodeGenerator::visitAssign(BinaryOp *op)
{
	visit(op->lhs());
	visit(op->rhs());
}

void HydroCodeGenerator::visitTernaryOp(TernaryOp *op)
{
	if (op->tokenValue() == "?")
	{
		// conditional
		// a ? b : c
		std::string label = newLabel();
		std::string end = newLabel();
		visit(op->expr1());
		emit("brf", label);
		visit(op->expr2());
		emit("br", end);
		emit(label + ":"); // define end point
		visit(op->expr3());
		emit(end + ":");
	}
	else
		appendError("Not a valid trenary operator.");
}

void HydroCodeGenerator::visitCall(Call *call)
{
	Expr *lhs = call->lhs();
    NamedCallArgs *named = dynamic_cast<NamedCallArgs *>(call->args());
    
	// call args
	const uint32_t nargs = call->args()->numChildren();

	// call
	if (SimpleName *id = dynamic_cast<SimpleName *>(lhs))
	{
        if(id->value() == "super")
        {
            // check if parent is expression statement
            ExprStmt *stmtParent = dynamic_cast<ExprStmt *>(call->parent());
            
            if(!stmtParent)
            {
                appendError("Call to super constructor is invalid.", call->parent());
                return; // abort
            }
            
            auto block = stmtParent->parent(); // will be a block
            assert(block);
            
            // super constructor call
            ConstructorSymbol *symbol = getEnclosingConstructor();
            
            if(symbol)
            {
                // verify
                if(block->getChildAt(0) != stmtParent)
                {
                    appendError("Call to super constructor must be the first statement in constructor block.", id);
                    return; // abort
                }
                
                visitCallArgs(call->args());
                emit("callsuper", std::to_string(nargs));
            }
            else
            {
                appendError("Illegal super constructor outside of a constructor context.", id);
            }
        }
		else if (_mode == h3o_compile_aot)
		{
			Symbol *local = resolveLocal(id->value());

			if (local)
			{
                assert(local->slot >= 0);
                
                if(named)
                {
                    emitNamedCall("namedcallx " + local->name()->value(), call, named);
                }
                else
                {
                    visitCallArgs(call->args());
                    emit("call", std::to_string(local->slot), std::to_string(nargs));
                }
				return;
			}
		}
        else
        {
            Symbol *symbol = tryResolve(id);
            
            if(ClassSymbol *cast = dynamic_cast<ClassSymbol *>(symbol))
            {
                // TODO emit explicit cast
                appendError("Explicit casts are not yet supported.");
            }
            else if(PropertySymbol *prop = dynamic_cast<PropertySymbol *>(symbol))
            {
                ClassSymbol *owner = getEnclosingClass();
                
                if(!owner)
                {
                    appendError("Illegal access to member '" + id->value() + "' outside of enclosing class.", id);
                    return; // abort
                }
                
                emit("this");
                visitCallArgs(call->args()); // emit arguments
                emit("callpropertyx", id->value(), std::to_string(nargs));
            }
            else if(MethodSymbol *meth = dynamic_cast<MethodSymbol *>(symbol))
            {
                ClassSymbol *owner = getEnclosingClass();
                
                if(!owner)
                {
                    appendError("Illegal access to member '" + id->value() + "' outside of enclosing class.", id);
                    return; // abort
                }
                
                if(named)
                {
                    emit("this");
                    emitNamedCall("namedcallproperty " + meth->name()->value(), call, named);
                }
                else
                {
                    emit("this"); // push this
                    visitCallArgs(call->args()); // emit arguments
                    emit("callpropertyx", id->value(), std::to_string(nargs));
                }
            }
            else if(FuncSymbol *func = dynamic_cast<FuncSymbol *>(symbol))
            {
                if(named)
                {
                    if(func->package())
                    {
                        emit("rload", funcToAssemblyName(func->package(), func));
                        emitNamedCall("namedocall", call, named);
                    }
                    else
                    {
                        emitNamedCall("namedcallx " + id->value(), call, named);
                    }
                }
                else
                {
                    if(func->package())
                    {
                        visitCallArgs(call->args());
                        emit("rload", funcToAssemblyName(func->package(), func));
                        emit("ocall", std::to_string(nargs));
                    }
                    else
                    {
                        visitCallArgs(call->args());
                        emit("callx", id->value(), std::to_string(nargs));
                    }
                }
            }
            else
            {
                if(named)
                {
                    emitNamedCall("namedcallx " + id->value(), call, named);
                }
                else
                {
                    visitCallArgs(call->args());
                    emit("callx", id->value(), std::to_string(nargs));
                }
            }
        }
	}
	else if (Call *c = lhs->tryCast<Call>())
	{
        if(named)
        {
            visit(c);
            emitNamedCall("namedocall", call, named);
        }
        else
        {
            // foo()()
            visitCallArgs(call->args());
            visit(c);
            emit("ocall", std::to_string(nargs));
        }
	}
	else if (BinaryOp *op = lhs->tryCast<BinaryOp>())
	{
		if (op->tokenValue() == ".")
		{
			// call property
			if (SimpleName *id = op->rhs()->tryCast<SimpleName>())
			{
				// TODO find member
				if (false)
				{
				}
				else
				{
					visit(op->lhs()); // visit subject
					visitCallArgs(call->args());
					emit("callpropertyx", id->value(), std::to_string(nargs));
				}
			}
			else
			{
				visit(op->lhs()); // visit subject
				visitCallArgs(call->args());
				emit("callindex", std::to_string(nargs));
			}
		}
		else if (op->tokenValue() == "[")
		{
			// call index
			visit(op->lhs()); // visit subject
			visit(op->rhs()); // visit key
            visitCallArgs(call->args());
			emit("callindex", std::to_string(nargs));
		}
        else if (op->tokenValue() == "::")
        {
            visit(op);
            visitCallArgs(call->args());
            emit("ocall", std::to_string(nargs));
        }
		else
		{
			// implementation error (parser should never produce this do to operator precedence)
			appendError("Unexpected operand left of invocation.", op->token());
		}
	}
	else
	{
		visit(lhs); // visit expression
        visitCallArgs(call->args());
		emit("ocall", std::to_string(nargs));
	}
}

void HydroCodeGenerator::visitCallArgs(CallArgs *args)
{
    if(dynamic_cast<NamedCallArgs *>(args))
    {
        // first pass
        Ast *child = args->firstChild();
        
        while (child)
        {
            //NamedArg *arg = child->tryCast<NamedArg>();
            // TODO named args
            //emit(""
            //emit("narg", arg->name()->value());
            child = child->next();
        }
        
        // second pass
        child = args->firstChild();
        
        while (child)
        {
            //NamedArg *arg = child->tryCast<NamedArg>();
            // TODO named args
            //emit(""
            //emit("narg", arg->name()->value());
            child = child->next();
        }
    }
    else
    {
        Ast *child = args->firstChild();

        while (child)
        {
            visitExpr(child->tryCast<Expr>());
            child = child->next();
        }
    }
}

void HydroCodeGenerator::visitEmptyStmt(EmptyStmt *stmt)
{
	// do nothing
}

void HydroCodeGenerator::visitIf(IfStmt *stmt)
{
	if (stmt->hasElse())
	{
		std::string label1 = newLabel();
		std::string label2 = newLabel();
		visit(stmt->cond());
		emit("brf", label1);
		visit(stmt->body());
		emit("br", label2);
		emit(label1 + ":");

		// emit else
		visit(stmt->elseStmt());

		// end if-then-else
		emit(label2 + ":");
	}
	else
	{
		std::string label1 = newLabel();
		visit(stmt->cond());
		emit("brf", label1);
		visit(stmt->body());
		emit(label1 + ":");
	}
}

void HydroCodeGenerator::visitThrow(ThrowStmt *stmt)
{
    visit(stmt->expr());
    emit("throw");
}

void HydroCodeGenerator::visitTry(TryStmt *stmt)
{
    emit(".try");
    emit("{");
    tabUp();
    visit(stmt->body());
    tabDown();
    emit("}");
    stmt->visitChildren(this, 1); // visit catch clauses and do not visit block again
}

void HydroCodeGenerator::visitCatch(CatchClause *stmt)
{
    int32_t slot = addSlot();
    TypeSpec *type = stmt->type();
    stmt->symbol()->slot = slot;
    emit(".catch", std::to_string(slot), toAssemblyType(type));
    emit("{");
    tabUp();
    emit(".var", stmt->name()->value(), toAssemblyType(type));
    visit(stmt->body());
    tabDown();
    emit("}");
}

void HydroCodeGenerator::visitFor(ForStmt *stmt)
{
	// initialize variable
	visit(stmt->init());

	// initialize loop
	startLoop(true);

	// condition check
	visit(stmt->cond());
	loopCond();

	// loop block
	visit(stmt->body());

	// iterator
	loopAlt();
	visit(stmt->it());
	jumpLoop();

	// end of loop (for break statements)
	endLoop();
}

void HydroCodeGenerator::visitForEach(ForEachStmt *stmt)
{
	if (stmt->keyName())
		visit(stmt->keyName());
	visit(stmt->valueName());
	visit(stmt->body());
}

void HydroCodeGenerator::visitDo(DoStmt *stmt)
{
	// initialize loop
	startLoop();

	// loop body
	visit(stmt->body());

	// loop condition
	visit(stmt->cond());
	loopCond();

	// loop end
	jumpLoop();

	// end loop (for break statements)
	endLoop();
}

void HydroCodeGenerator::visitWhile(WhileStmt *stmt)
{
	// initialize loop
	startLoop();

	// loop condition
	visit(stmt->cond());
	loopCond();

	// loop body
	visit(stmt->body());

	// loop end
	jumpLoop();

	// end loop (for break statements)
	endLoop();
}

void HydroCodeGenerator::visitContinue(ContinueStmt *stmt)
{
	if (stmt->expr())
	{
		if (stmt->expr()->tokenType() == int_tkn)
		{
			// continue at depth
			int32_t k = std::stoi(stmt->expr()->tokenValue());
			continueLoop(k, stmt);
		}
		else
		{
			// error
			appendError("Expression in continue statement is expected to be an integer.", stmt->expr()->token());
		}
	}
	else
	{
		// default
		continueLoop(1, stmt);
	}
}

void HydroCodeGenerator::visitBreak(BreakStmt *stmt)
{
	if (stmt->expr())
	{
		if (stmt->expr()->tokenType() == int_tkn)
		{
			// break at depth
			int32_t k = std::stoi(stmt->expr()->tokenValue());
			breakLoop(k, stmt);
		}
		else
		{
			// error
			appendError("Expression in break statement is expected to be an integer.", stmt->expr()->token());
		}
	}
	else
	{
		// default
		breakLoop(1, stmt);
	}
}

void HydroCodeGenerator::visitSwitch(SwitchStmt *stmt)
{
	visit(stmt->expr());
	visit(stmt->body());
}

void HydroCodeGenerator::visitCase(CaseClause *stmt)
{
	stmt->visitChildren(this);
}

void HydroCodeGenerator::visitDefault(DefaultClause *stmt)
{
	stmt->visitChildren(this);
}

void HydroCodeGenerator::visitReturn(ReturnStmt *stmt)
{
	if (stmt->expr())
	{
		visit(stmt->expr());
		emit("ret"); // returns a value
	}
	else
		emit("halt"); // returns void
}

void HydroCodeGenerator::visitName(Name *name)
{
}

void HydroCodeGenerator::visitPackageSpec(PackageSpec *pkg)
{
}

void HydroCodeGenerator::visitVar(VarDecl *var)
{
	std::string line = ".var " + var->name()->value() + " ";
	line += toAssemblyType(var->type());
	line += createModString(var->modifier());
	emit(line); // define symbol
    int32_t slot = addSlot();
    var->symbol()->slot = slot;
    
	if (var->hasDefaultValue())
	{
		// set value
		visit(var->defaultValue());
	}
	else
	{
		// TODO get actual default value (i.e., false, undefined, null, or 0)
		emit("und");
	}

    emit("store", std::to_string(slot));
	//emit("storex", var->name()->value());
}

void HydroCodeGenerator::emitFuncBody(FuncDecl *func, bool emitBlock)
{
	Modifier *mod = func->modifier();
	std::string modifier = createModString(mod);
	std::string returnType = toAssemblyType(func->returnType());
    FuncArgs *args = func->args();
    
	// emit arguments
	if (args && args->arity() > 0)
	{
		Ast *child = args->firstChild();
        tabUp();
        
		while (child)
		{
			Farg *a = child->tryCast<Farg>();
			std::string argType = toAssemblyType(a->type());
			std::string var = a->variadic() ? "$" : ""; // variadic flag
			child = child->next();
            a->symbol()->slot = func->slots++;
            
			if (a->hasDefaultValue())
			{
				emit(".arg", var + a->name()->value(), argType, "= ["); // start instruction set [
				tabUp();
				visit(a->defaultValue());
				tabDown();
				emit("]"); // end instruction set ]
			}
			else
				emit(".arg", var + a->name()->value(), argType);
		}

        tabDown();
	}
    
    if (emitBlock)
    {
        emit("{");
        tabUp();
        
        // emit body if exists
        if (func->hasBody())
           visit(func->body());
        
		tabDown();
		emit("}");
	}
}

void HydroCodeGenerator::visitFunc(FuncDecl *func)
{
	Modifier *modifier = func->modifier();
	std::string pkg = packageToAssemblyName(func->package());
	std::string mod = createModString(modifier);
	std::string returnType = toAssemblyType(func->returnType());
    
    if(!func->package())
    {
        // local function
        func->symbol()->slot = addSlot();
    }
    
    pushScope(func);
	comment(func->name() && func->name()->value().empty() ? "function" : func->name()->value() + " function");

	if (!pkg.empty())
		emit(".func", func->name()->tokenValue() + "()", pkg, mod + " " + returnType);
	else if (!mod.empty())
		emit(".func", func->name()->tokenValue() + "()", mod, returnType);
	else
		emit(".func", func->name()->tokenValue() + "()", returnType);
    
	// TODO validate modifiers
	if (modifier->hasMod("abstract"))
		appendError("Functions cannot be abstract.");
	else if (modifier->hasMod("final"))
		appendError("Functions are constants and cannot be finalized."); // TODO warning
    
    if(func->body())
    {
        // native functions cannot contain a body
        if (modifier->hasMod("native"))
            appendError("Native functions cannot contain a block.", func->body());
        
        // emit args and body
        emitFuncBody(func);
    }
    else
    {
        if(!modifier->hasMod("native"))
            appendError("Function body is required.", func);
        
        // emit args only
        emitFuncBody(func, false);
    }
    
    popScope();
	emit("");
}

void HydroCodeGenerator::visitClass(ClassDecl *clss)
{
	std::string mod = createModString(clss->modifier());
	std::string name = clss->name()->value();
	PackageSymbol *pkg = clss->package();
    std::string extends;
    tryResolveTypeBindings(clss);
    ClassSymbol *superClass = tryResolveSuperClass(clss);
    
    if(clss->symbol()->name()->value() == "Object" && pkg && pkg->fullPackageName() == "system")
        extends = ""; // system::Object does not have base type
    else if(superClass)
    {
        if(superClass->package())
        {
            // extends packaged class
            extends = "extends " + classToAssemblyName(superClass->package(), superClass);
        }
        else
        {
            // try local class
            int32_t index = getLocalIndex(superClass, _currentScope);
            if(index >= 0)
            {
                // extends local class
                extends = "extends " + std::to_string(index);
            }
            else
            {
                // extends upvalue class
                extends = "extends " + std::to_string(index);
            }
        }
    }
    else
    {
        extends = "extends @system/Object"; // default
    }
    
	comment(name + " class");
    Scope *scope = pushScope(clss);
	emit(".class", name + (pkg != nullptr ? " " + packageToAssemblyName(pkg) : ""), mod, extends);
	emit("{");
	tabUp();
	visit(clss->body());
	tabDown();
	emit("}");
    if(scope)
        popScope();
	emit("");
}

void HydroCodeGenerator::visitConstructor(ConstructorDecl *construct)
{
    Scope *scope = pushScope(construct);
    
	Modifier *mods = construct->modifier();
	PackageSymbol *pkg = construct->package();
    ClassSymbol *owner = getParentClass();
	std::string modifiers = createModString(mods);
	std::string returnType = toAssemblyType(construct->returnType());
    
	if (pkg)
    {
        // invalid
		appendError(HVM_COMPILE_ERROR_CONSTRUCTOR_PACKAGE, pkg->token());
    }
    
    if (!owner)
    {
        // parent must be a class
        appendError(HVM_COMPILE_ERROR_CONSTRUCTOR_NO_CLASS, construct->token());
    }

	// constructor
	if (construct->returnType() != nullptr && !construct->isUntyped())
		appendError(HVM_COMPILE_ERROR_CONSTRUCTOR_RETURN_TYPE, construct->returnType());

	comment("constructor()");
	emit(".construct", "()", modifiers);

	// emit opening blcok
	//emit("{");
	//tabUp();
    
    if(construct->body())
    {
        Call *superCall = findSuperCall(construct->body());
               
        if (mods->hasMod("native"))
            appendError("Native methods cannot contain a block.", construct->body());
        else if (mods->hasMod("abstract"))
            appendError("Abstract methods cannot contain a block.", construct->body());
        
        // emit args only
        emitFuncBody(construct, false);
        
        // emit block manually
        emit("{");
        tabUp();
        
        if(!superCall)
        {
            if(owner && owner->fullName() == "system::Object")
            // emit implicit call to super constructor
            emit("callsuper", "0");
        }
        
        visit(construct->body());
        tabDown();
        emit("}");
    }
    else
    {
        if(!mods->hasMod("native") && !mods->hasMod("abstract"))
            appendError("Required constructor body is missing.", construct);
        emitFuncBody(construct, false);
    }
    
	// emit closing block
	//tabDown();
	//emit("}");
    if(scope)
        popScope();
	emit("");

	// TODO check for explicit super constructor calls
	// emit super constructor call code explicitly or implicitly
}

void HydroCodeGenerator::visitProperty(PropertyDecl *prop)
{
	std::string mod = createModString(prop->modifier());
	std::string name = prop->name()->tokenValue();
	std::string type = toAssemblyType(prop->type());
    
    comment(prop->name()->value() + " property");
    
	if (prop->hasDefaultValue())
	{
		emit(".property", name, type, mod + "= ["); // start instruction set [
		prop->defaultValue()->accept(this);
		emit("]"); // end instruction set ]
	}
	else if (prop->hasBody())
	{
        Scope *scope = pushScope(prop);
        emit(".property", name, type, mod);
        emit("{");
        tabUp();
		Stmt *body = prop->body();
		Ast *child = body->firstChild();
        GetterDecl *getter = nullptr;
        SetterDecl *setter = nullptr;
        
		while (child)
		{
            if (GetterDecl *get = dynamic_cast<GetterDecl *>(child))
            {
                if(getter)
                    appendError("Redefinition of getter on property '" + name + "'.", get);
                getter = get;
                visit(getter);
            }
            else if (SetterDecl *set = dynamic_cast<SetterDecl *>(child))
            {
                if(setter)
                    appendError("Redefinition of setter on property '" + name + "'.", set);
                setter = set;
                visit(setter);
            }
            else
            {
                // error! parse will never produce this
                appendError("Statement is unexpected.", child);
            }
            
			child = child->next();
		}
        
        // TODO validate getter and setter
        tabDown();
        emit("}");
        if(scope)
            popScope();
	}
	else
		emit(".property", name, type, mod);
    
    emit("");
}

void HydroCodeGenerator::visitGetter(GetterDecl *get)
{
    Scope *scope = pushScope(get);
    if(get->args())
    {
        if(get->args()->numChildren())
            appendError("Getter definition defined with arguments.", get->args());
        
        if(!get->body())
        {
            appendError("Expecting getter body.", get);
            return; // skip
        }
        
        emit(".getter ()");
        emitFuncBody(get);
    }
    else if(get->body())
    {
        emit(".getter ()");
        emitFuncBody(get);
    }
    else
    {
        emit(".getter (auto)");
    }
    if(scope)
        popScope();
}

void HydroCodeGenerator::visitSetter(SetterDecl *set)
{
    Scope *scope = pushScope(set);
    
    if(set->args())
    {
        if(set->args()->numChildren() > 1)
            appendError("Setter definition defined with too many arguments.", set->args());
        else if(set->args()->numChildren() == 0)
            appendError("Setter definition defined with too few arguments. Excepting at least one argument.");
        
        if(!set->body())
        {
            appendError("Expecting setter body.", set);
            return; // skip
        }
        
        emit(".setter ()");
        emitFuncBody(set);
    }
    else
    {
        emit(".setter (auto)");
    }
    
    if(scope)
        popScope();
}

void HydroCodeGenerator::visitMethod(MethodDecl *meth)
{
	if (!parentDeclIsClass(meth))
	{
		// parent must be a class
		appendError(HVM_COMPILE_ERROR_METHOD_NO_CLASS, meth->token());
	}

	Modifier *mods = meth->modifier();
	std::string pkg = packageToAssemblyName(meth->package());
	std::string modifiers = createModString(mods);
	std::string returnType = toAssemblyType(meth->returnType());

	comment(meth->name() && meth->name()->value().empty() ? "method" : meth->name()->value() + "() method");

	if (!pkg.empty())
		emit(".method", meth->name()->tokenValue() + "()", pkg, modifiers + " " + returnType);
	else if (!modifiers.empty())
		emit(".method", meth->name()->tokenValue() + "()", modifiers, returnType);
	else
		emit(".method", meth->name()->tokenValue() + "()", returnType);
    
    Scope *scope = pushScope(meth);
    
	if(meth->body())
    {
        if (mods->hasMod("native"))
            appendError("Native methods cannot contain a block.", meth->body());
        else if (mods->hasMod("abstract"))
            appendError("Abstract methods cannot contain a block.", meth->body());
        
        // emit args and body
        emitFuncBody(meth);
    }
    else
    {
        if(!mods->hasMod("native") && !mods->hasMod("abstract"))
            appendError("Method body is required.", meth);
        
        // emit args only
        emitFuncBody(meth, false);
    }
    
    if(scope)
        popScope();
	emit("");
}

void HydroCodeGenerator::visitAction(ActDecl *act)
{
    Symbol *context;
    Scope *scope = pushScope(act);
    
    if(act->context()->isSimpleName())
    {
        SimpleName *simple = dynamic_cast<SimpleName *>(act->context());
        context = tryResolve(simple);
    }
    else
    {
        QualifiedName *qualified = dynamic_cast<QualifiedName *>(act->context());
        context = tryResolve(qualified);
    }
    
    if(EventSymbol *event = dynamic_cast<EventSymbol *>(context))
    {
        PackageSymbol *pkg = loadEnclosingPackage();
        std::string mod = createModString(act->modifier());
        
        if(pkg)
        {
            // global
            emit(".action", act->name()->value() + "[" + eventToAssemblyName(event->package(), event) + "]", packageToAssemblyName(pkg), mod);
        }
        else
        {
            // local
            emit(".action", act->name()->value() + "[" + eventToAssemblyName(event->package(), event) + "]", mod);
        }
        
        emit("{");
        tabUp();
        visit(act->body());
        tabDown();
        emit("}");
    }
    else if(context)
    {
        // not a valid context
        appendError("The symbol '" + context->name()->value() + "' is not a context.", act->context());
    }
    else
    {
        // context could not be resolved
        appendError("Unresolved context '" + act->context()->value() + "'", act->context());
    }
    
    if(scope)
        popScope();
}

void HydroCodeGenerator::visitEvent(EventDecl *evnt)
{
    std::string extends = "";
    comment(evnt->name()->value() + " event");
    PackageSymbol *package = loadEnclosingPackage();
    std::string mod = createModString(evnt->modifier());
    
    if(package)
    {
        emit(".event", evnt->name()->value(), packageToAssemblyName(package) + " " + mod, extends);
    }
    else
    {
        emit(".event", evnt->name()->value(), mod, extends);
    }
    
    emit("{");
    tabUp();
    visit(evnt->body());
    tabDown();
    emit("}");
}

void HydroCodeGenerator::visitEtype(EtypeDecl *etype)
{
    emit(".etype", etype->name()->value());
}

void HydroCodeGenerator::visitParam(ParamDecl *param)
{
    std::string mod = createModString(param->modifier());
    
    if(param->defaultValue())
    {
        emit(".param " + param->name()->value(), toAssemblyType(param->type()), mod, "= [");
        tabUp();
        visit(param->defaultValue());
        tabDown();
        emit("]");
    }
    else
    {
        emit(".param " + param->name()->value(), toAssemblyType(param->type()), mod);
    }
}

void HydroCodeGenerator::visitInterface(InterfaceDecl *it)
{
}

void HydroCodeGenerator::visitEnum(EnumDecl *enm)
{
}

PackageSymbol *HydroCodeGenerator::loadEnclosingPackage()
{
    Scope *scope = _currentScope;
    
    while(scope)
    {
        if(PackageSymbol *pkg = dynamic_cast<PackageSymbol *>(scope->owner()))
            return pkg;
        
        scope = scope->parent();
    }
    
    // fail
    return nullptr;
}

void HydroCodeGenerator::generate(Program *program, Scope *scopeToUse, hydro_compile_mode mode)
{
	_h3oasm = "";
    _subject = program;
    _currentScope = scopeToUse;
	_mode = mode;
	_tab = 0;
    
    // header
    comment("--- Program ---");

	// visit
	visit(_subject);

	if (!compiler()->failed() && evalMode())
	{
		if (_subject->numChildren() > 0)
			emit("ret"); // return result
		else
			emit("halt"); // empty instructions
	}

	if (static_cast<HydroCompiler *>(_compiler)->debug())
		std::cout << _h3oasm << std::endl;
}

void HydroCodeGenerator::visit(vp_element e)
{
    if(Ast *ast = dynamic_cast<Ast *>(e))
    {
        Token *token = ast->token();
        
        if(token)
        {
            if(_source != token->source())
            {
                _source = token->source();
                _line = 1;
                //emit(".source", '"' + _source + '"');
                //emit(".line", std::to_string(_line));
            }
            
            // line number
            if (token->line() > _line)
            {
                _line = token->line();
                //emit(".line", std::to_string(_line));
            }
        }
    }
    
    HydroVisitor::visit(e);
}

} // namespace hydro

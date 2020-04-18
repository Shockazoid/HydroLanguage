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

#include "HydroVisitor.hpp"
#include "Scope.hpp"
#include "../HydroCompiler.hpp"

namespace hydro
{

HydroVisitor::HydroVisitor(HydroCompiler *compiler, Scope *globals) : AstVisitor{compiler}, _currentScope{nullptr}, _scopes{}, _imports{}, _globals{globals} {}

HydroVisitor::~HydroVisitor() {}

bool HydroVisitor::isAssignment(BinaryOp *op)
{
	std::string val = op->tokenValue();

	return (
	    val == "=" ||
	    val == "**=" ||
	    val == "*=" ||
	    val == "/=" ||
	    val == "%=" ||
	    val == "+=" ||
	    val == "-=" ||
	    val == "&=" ||
	    val == "|=" ||
	    val == ">>=" ||
	    val == "<<=" ||
	    val == "^=" // (?)

	);
}

void HydroVisitor::visitProgram(Program *prog)
{
	// visit program statements
	prog->visitChildren(this);
}

void HydroVisitor::visitStmt(Stmt *stmt)
{
	if (EmptyStmt *estmt = stmt->tryCast<EmptyStmt>())
		visitEmptyStmt(estmt);
	else if (ExprStmt *exprstmt = stmt->tryCast<ExprStmt>())
		visitExprStmt(exprstmt);
	else if (Block *block = stmt->tryCast<Block>())
		visitBlock(block);
	else if (Decl *decl = stmt->tryCast<Decl>())
		visitDecl(decl);
	else if (IfStmt *ifstmt = stmt->tryCast<IfStmt>())
		visitIf(ifstmt);
	else if (ElseStmt *elsestmt = stmt->tryCast<ElseStmt>())
		visitElse(elsestmt);
	else if (ThrowStmt *thrw = stmt->tryCast<ThrowStmt>())
		visitThrow(thrw);
	else if (TryStmt *ty = stmt->tryCast<TryStmt>())
		visitTry(ty);
    else if (CatchClause *ctch = stmt->tryCast<CatchClause>())
        visitCatch(ctch);
	else if (ForStmt *fr = stmt->tryCast<ForStmt>())
		visitFor(fr);
	else if (ForEachStmt *fr = stmt->tryCast<ForEachStmt>())
		visitForEach(fr);
	else if (DoStmt *d = stmt->tryCast<DoStmt>())
		visitDo(d);
	else if (WhileStmt *whle = stmt->tryCast<WhileStmt>())
		visitWhile(whle);
	else if (ContinueStmt *cont = stmt->tryCast<ContinueStmt>())
		visitContinue(cont);
	else if (BreakStmt *brk = stmt->tryCast<BreakStmt>())
		visitBreak(brk);
	else if (SwitchStmt *swtch = stmt->tryCast<SwitchStmt>())
		visitSwitch(swtch);
	else if (CaseClause *cse = stmt->tryCast<CaseClause>())
		visitCase(cse);
	else if (DefaultClause *dfault = stmt->tryCast<DefaultClause>())
		visitDefault(dfault);
	else if (ReturnStmt *ret = stmt->tryCast<ReturnStmt>())
		visitReturn(ret);
	else if (ImportStmt *imp = stmt->tryCast<ImportStmt>())
		visitImport(imp);
	else
		; // do nothing (nil statement)
}

void HydroVisitor::visitBlock(Block *block)
{
	block->visitChildren(this);
}

void HydroVisitor::visitExpr(Expr *expr)
{

	if (EmptyExpr *emp = expr->tryCast<EmptyExpr>())
		visitEmpty(emp);
	else if (Nested *nst = expr->tryCast<Nested>())
		visitNested(nst);
	else if (Literal *lit = expr->tryCast<Literal>())
		visitLiteral(lit);
	else if (UnaryOp *uny = expr->tryCast<UnaryOp>())
		visitUnaryOp(uny);
	else if (BinaryOp *bin = expr->tryCast<BinaryOp>())
		visitBinaryOp(bin);
	else if (TernaryOp *trn = expr->tryCast<TernaryOp>())
		visitTernaryOp(trn);
}

void HydroVisitor::visitEmpty(EmptyExpr *expr)
{
	// do nothing
}

void HydroVisitor::visitNested(Nested *expr)
{
	visit(expr->expr());
}

void HydroVisitor::visitLiteral(Literal *expr)
{
	if (SimpleName *id = expr->tryCast<SimpleName>())
	{
		std::string val = id->value();

		// reserved keywords
		if (val == "null")
			visitNull(expr);
		else if (val == "undefined")
			visitUndefined(expr);
		else if (val == "NaN")
			visitNaN(expr);
		else if (val == "Infinity")
			visitInfinity(expr);
		else
			visitIdentifier(id);
	}
    else if(NullLiteral *null = expr->tryCast<NullLiteral>())
        visitNull(null);
	else if (ObjectInit *obj = expr->tryCast<ObjectInit>())
		visitObject(obj);
	else if (JsonInit *json = expr->tryCast<JsonInit>())
		visitJson(json);
	else if (ArrayInit *arr = expr->tryCast<ArrayInit>())
		visitArray(arr);
	else if (DictInit *dict = expr->tryCast<DictInit>())
		visitDict(dict);
	else if (XmlInit *xml = expr->tryCast<XmlInit>())
		visitXml(xml);
	else
	{
		switch (expr->tokenType())
		{
		case package_tkn:
		{
			visitPackage(expr);
			break;
		}
		case hash_tkn:
		{
			visitIdentity(expr);
			break;
		}
		case bool_tkn:
		{
			visitBool(expr);
			break;
		}
		case char_tkn:
		{
			visitChar(expr);
			break;
		}
		case string_tkn:
		{
			visitString(expr);
			break;
		}
		case template_tkn:
		{
			visitStringTemplate(expr);
			break;
		}
		case uint_tkn:
		{
			visitUint(expr);
			break;
		}
		case int_tkn:
		{
			visitInt(expr);
			break;
		}
		case hex_tkn:
		{
			visitHexadecimal(expr);
			break;
		}
		case bin_tkn:
		{
			visitBinary(expr);
			break;
		}
		case oct_tkn:
		{
			visitOctal(expr);
			break;
		}
		case ushort_tkn:
		{
			visitUshort(expr);
			break;
		}
		case short_tkn:
		{
			visitShort(expr);
			break;
		}
		case ulong_tkn:
		{
			visitUlong(expr);
			break;
		}
		case long_tkn:
		{
			visitUint(expr);
			break;
		}
		case float_tkn:
		{
			visitFloat(expr);
			break;
		}
		case double_tkn:
		{
			visitDouble(expr);
			break;
		}
		default:
		{
			// unexpected literal
			abrupt(expr);
			break;
		}
		}
	}
}

void HydroVisitor::visitIdentifier(SimpleName *id)
{
}

void HydroVisitor::visitNull(Literal *expr)
{
}

void HydroVisitor::visitUndefined(Literal *expr)
{
}

void HydroVisitor::visitPackage(Literal *expr)
{
}

void HydroVisitor::visitIdentity(Literal *expr)
{
}

void HydroVisitor::visitBool(Literal *expr)
{
}

void HydroVisitor::visitChar(Literal *expr)
{
}

void HydroVisitor::visitString(Literal *expr)
{
}

void HydroVisitor::visitStringTemplate(Literal *expr)
{
}

void HydroVisitor::visitUint(Literal *expr)
{
}

void HydroVisitor::visitInt(Literal *expr)
{
}

void HydroVisitor::visitHexadecimal(Literal *expr)
{
}

void HydroVisitor::visitBinary(Literal *expr)
{
}

void HydroVisitor::visitOctal(Literal *expr)
{
}

void HydroVisitor::visitUshort(Literal *expr)
{
}

void HydroVisitor::visitShort(Literal *expr)
{
}

void HydroVisitor::visitUlong(Literal *expr)
{
}

void HydroVisitor::visitLong(Literal *expr)
{
}

void HydroVisitor::visitFloat(Literal *expr)
{
}

void HydroVisitor::visitDouble(Literal *expr)
{
}

void HydroVisitor::visitInfinity(Literal *expr)
{
}

void HydroVisitor::visitNaN(Literal *expr)
{
}

void HydroVisitor::visitRegex(Literal *expr)
{
}

void HydroVisitor::visitObject(ObjectInit *expr)
{
	// visit object properties
	Ast *child = expr->firstChild();

	while (child)
	{
		if (ast_objectProp prop = child->tryCast<ObjectInitProperty>())
			visitObjectProp(prop);
		child = child->next();
	}
}

void HydroVisitor::visitObjectProp(ast_objectProp prop)
{
	visit(prop->name());
	visit(prop->value());
}

void HydroVisitor::visitJson(ast_json json)
{
	// visit json properties
	Ast *child = json->firstChild();

	while (child)
	{
		if (ast_jsonProp prop = child->tryCast<JsonInitProperty>())
			visitJsonProp(prop);
		child = child->next();
	}
}

void HydroVisitor::visitJsonProp(ast_jsonProp prop)
{
	visit(prop->key());
	visit(prop->value());
}

void HydroVisitor::visitJsonValue(Expr *expr)
{
	visit(expr);
}

void HydroVisitor::visitArray(ast_array arr)
{
	// visit elements
	Ast *child = arr->firstChild();

	while (child)
	{
		if (Expr *expr = child->tryCast<Expr>())
			visitArrayElement(expr);
		child = child->next();
	}
}

void HydroVisitor::visitArrayElement(Expr *elm)
{
	visitExpr(elm);
}

void HydroVisitor::visitDict(ast_dict dict)
{
	// visit entries
	Ast *child = dict->firstChild();

	while (child)
	{
		if (ast_dictentry entry = child->tryCast<DictInitEntry>())
			visitDictEntry(entry);
		child = child->next();
	}
}

void HydroVisitor::visitDictEntry(ast_dictentry entry)
{
	visit(entry->key());
	visit(entry->value());
}

void HydroVisitor::visitXml(ast_xml xml)
{
	Ast *child = xml->firstChild();

	while (child)
	{
		if (ast_xml xmlChild = child->tryCast<XmlInit>())
			visit(xmlChild);
		child = child->next();
	}
}

void HydroVisitor::visitLambda(AFunc *lambda)
{

	visit(lambda->body());
}

void HydroVisitor::visitUnaryOp(ast_unary op)
{
	visit(op->expr());
}

void HydroVisitor::visitBinaryOp(ast_binary op)
{
	if (isAssignment(op))
		visitAssign(op);
	else if (ast_call call = op->tryCast<Call>())
		visitCall(call);
	else
	{
		visit(op->lhs());
		visit(op->rhs());
	}
}

void HydroVisitor::visitAssign(ast_binary op)
{
	visit(op->lhs());
	visit(op->rhs());
}

void HydroVisitor::visitTernaryOp(ast_ternary op)
{
	visit(op->expr1());
	visit(op->expr2());
	visit(op->expr3());
}

void HydroVisitor::visitCall(ast_call call)
{
	visit(call->lhs());
	visitCallArgs(call->args());
}

void HydroVisitor::visitCallArgs(ast_cargs args)
{
	// visit params
	Ast *child = args->firstChild();

	while (child)
	{
		if (Expr *expr = child->tryCast<Expr>())
			visit(expr);
		child = child->next();
	}
}

void HydroVisitor::visitEmptyStmt(EmptyStmt *stmt)
{
	// do nothing
}

void HydroVisitor::visitExprStmt(ExprStmt *stmt)
{
	visit(stmt->expr());
}

void HydroVisitor::visitBody(Stmt *body)
{
	body->visitChildren(this);
}

void HydroVisitor::visitIf(IfStmt *stmt)
{
	visit(stmt->cond());
	visit(stmt->body());
}

void HydroVisitor::visitElse(ElseStmt *stmt)
{
	visit(stmt->body());
}

void HydroVisitor::visitThrow(ThrowStmt *stmt)
{
    visit(stmt->expr());
}

void HydroVisitor::visitTry(TryStmt *stmt)
{
    visit(stmt->body());
}

void HydroVisitor::visitCatch(CatchClause *stmt)
{
    visit(stmt->body());
}

void HydroVisitor::visitFor(ForStmt *stmt)
{
	visit(stmt->init());
	visit(stmt->cond());
	visit(stmt->it());
	visit(stmt->body());
}

void HydroVisitor::visitForEach(ForEachStmt *stmt)
{
	if (stmt->keyName())
		visit(stmt->keyName());
	visit(stmt->valueName());
	visit(stmt->body());
}

void HydroVisitor::visitDo(DoStmt *stmt)
{
	visit(stmt->body());
	visit(stmt->cond());
}

void HydroVisitor::visitWhile(WhileStmt *stmt)
{
	visit(stmt->cond());
	visit(stmt->body());
}

void HydroVisitor::visitContinue(ContinueStmt *stmt)
{
}

void HydroVisitor::visitBreak(BreakStmt *stmt)
{
}

void HydroVisitor::visitSwitch(SwitchStmt *stmt)
{
	visit(stmt->expr());
	visit(stmt->body());
}

void HydroVisitor::visitCase(CaseClause *stmt)
{
	stmt->visitChildren(this);
}

void HydroVisitor::visitDefault(DefaultClause *stmt)
{
	stmt->visitChildren(this);
}

void HydroVisitor::visitReturn(ReturnStmt *stmt)
{
	visit(stmt->expr());
}

void HydroVisitor::visitImport(ImportStmt *stmt)
{
    _imports.push_back(stmt->name());
	visitName(stmt->name());
}

void HydroVisitor::visitName(Name *name)
{
}

void HydroVisitor::visitPackageSpec(PackageSpec *pkg)
{
}

void HydroVisitor::visitDecl(Decl *decl)
{
	if (VarDecl *var = decl->tryCast<VarDecl>())
		visitVar(var);
	/*else if(ConstDecl *cnst = decl->tryCast<ConstDecl>())
            visitConst(csnt);
        else if(LetDecl *let = decl->tryCast<LetDecl>())
            visitLet(let);*/
    else if (PackageDecl *pkg = decl->tryCast<PackageDecl>())
        visitPackage(pkg);
    else if (GetterDecl *getter = decl->tryCast<GetterDecl>())
        visitGetter(getter);
    else if (SetterDecl *setter = decl->tryCast<SetterDecl>())
        visitSetter(setter);
	else if (ConstructorDecl *construct = decl->tryCast<ConstructorDecl>())
		visitConstructor(construct);
	else if (MethodDecl *meth = decl->tryCast<MethodDecl>())
		visitMethod(meth);
	else if (FuncDecl *func = decl->tryCast<FuncDecl>())
		visitFunc(func);
    else if (ActDecl *act = decl->tryCast<ActDecl>())
        visitAction(act);
    else if (EtypeDecl *etype = decl->tryCast<EtypeDecl>())
        visitEtype(etype);
    else if (EventDecl *evnt = decl->tryCast<EventDecl>())
        visitEvent(evnt);
    else if (ParamDecl *param = decl->tryCast<ParamDecl>())
        visitParam(param);
	else if (ClassDecl *clss = decl->tryCast<ClassDecl>())
		visitClass(clss);
	else if (PropertyDecl *prop = decl->tryCast<PropertyDecl>())
		visitProperty(prop);
}

void HydroVisitor::visitVar(VarDecl *var)
{
}
/*
void HydroVisitor::visitConst(ConstDecl* cnst)
{
}
    
void HydroVisitor::visitLet(LetDecl* let)
{
}
*/

void HydroVisitor::visitPackage(PackageDecl *pkg)
{
    _imports.push_back(pkg->packageName());
    visit(pkg->body());
}

void HydroVisitor::visitLibrary(LibraryDecl *lib)
{
    visit(lib->body());
}

void HydroVisitor::visitFunc(FuncDecl *func)
{
	visit(func->body());
}

void HydroVisitor::visitClass(ClassDecl *clss)
{
	visit(clss->body());
}

void HydroVisitor::visitConstructor(ConstructorDecl *construct)
{
	visit(construct->body());
}

void HydroVisitor::visitProperty(PropertyDecl *prop)
{
}

void HydroVisitor::visitGetter(GetterDecl *get)
{
}

void HydroVisitor::visitSetter(SetterDecl *set)
{
}

void HydroVisitor::visitMethod(MethodDecl *meth)
{
	visit(meth->body());
}

void HydroVisitor::visitAction(ActDecl *act)
{
    visit(act->body());
}

void HydroVisitor::visitEvent(EventDecl *evnt)
{
    visit(evnt->body());
}

void HydroVisitor::visitEtype(EtypeDecl *etype)
{
}

void HydroVisitor::visitParam(ParamDecl *param)
{
    if(param->defaultValue())
        visit(param->defaultValue());
}

void HydroVisitor::visitInterface(InterfaceDecl *it)
{
}

void HydroVisitor::visitEnum(EnumDecl *enm)
{
}

void HydroVisitor::visit(vp_element e)
{
	Ast *node = dynamic_cast<Ast *>(e);

	if (Stmt *stmt = dynamic_cast<Stmt *>(node))
	{
		// statement
		visitStmt(stmt);
	}
	else if (Expr *expr = dynamic_cast<Expr *>(node))
	{
		// expression
		visitExpr(expr);
	}
	else if (Name *name = dynamic_cast<Name *>(node))
	{
		// name
		visitName(name);
	}
	else if (PackageSpec *package = dynamic_cast<PackageSpec *>(node))
	{
		// package specifier
		visitPackageSpec(package);
	}
	else if (Program *prog = dynamic_cast<Program *>(node))
	{
		// program
		visitProgram(prog);
	}
	else
	{
		// unrecognized node (indicates an error in code)
		abrupt(node);
	}
}

Scope *HydroVisitor::pushScope(Decl *decl)
{
    if(Scope *scope = decl->ownScope())
    {
        _scopes.push_back(scope);
        _currentScope = scope;
        return _currentScope; // success!
    }
    
    // scope not found
    return nullptr;
}

Scope *HydroVisitor::popScope()
{
    if(_scopes.empty())
        throw std::runtime_error{"Popping empty scope."};
    
    Scope *top = _scopes.back();
    _scopes.pop_back();
    _currentScope = !_scopes.empty() ? _scopes.back() : nullptr;
    return top;
}

Symbol *HydroVisitor::resolve(std::string name, Scope *scopeToUse)
{
    if(scopeToUse)
    {
        // try resolve
        return scopeToUse->resolve(name);
    }
    
    // fail
    return nullptr;
}

Symbol *HydroVisitor::resolve(Name *name, Scope *scopeToUse)
{
    return resolve(name->value(), scopeToUse);
}

Symbol *HydroVisitor::resolve(std::string name)
{
    return resolve(name, _currentScope);
}

Symbol *HydroVisitor::resolve(Name *name)
{
    return resolve(name, _currentScope);
}

Symbol *HydroVisitor::loadImport(Name *import, std::string name, std::string path)
{
    HydroCompiler *compiler = static_cast<HydroCompiler *>(_compiler);
    std::string qname = import->value() + "::" + name;
    //std::string ext = ".hydro";
    
    /*
    // try load import
    std::string hydroPath = path + ext;
    FileSource *src = new FileSource{hydroPath};
    
    // try open first possible source
    src->open();

    if(!src->isOpen())
    {
        delete src; // dispose
        hydroPath = path + '/' + name + ext;
        src = new FileSource{hydroPath};
        
        src->open();
        
        if(!src->isOpen())
            return nullptr; // failed to open second possible source
    }
    */
    Source *src = compiler->resolveSourceByQName(qname);
    
    if(src)
    {
        compiler->parseSource(src);
        delete src;
        
        return resolveImportByName(import, name);
    }
    
    // fail
    return nullptr;
}

Symbol *HydroVisitor::resolveImportByName(Name *import, std::string name)
{
    // try resolve locally
    if(Symbol *symbol = resolve(name))
        return symbol; // success!
    
    // try resolve in package
    std::vector<std::string> tokens;
    split(tokens, import->value(), '.');
    Scope *scope = _globals;
    
    // resolve all
    for(std::string pkg : tokens)
    {
        if(Symbol *symbol = resolve(pkg, scope))
        {
            scope = symbol->ownScope();
            if(scope)
                continue;
        }
        
        // fail
        break;
    }
    
    if(scope)
    {
         // try to resolve in package
        return resolve(name, scope);
    }
    
    // fail
    return nullptr;
}

void HydroVisitor::tryResolveImport(std::list<Symbol *> &results, Name *import, std::string name)
{
    HydroCompiler *compiler = static_cast<HydroCompiler *>(_compiler);
    
    std::fstream file;
    std::string sdkPath = compiler->sdkPath();
    std::string srcPath = compiler->srcPath();
    
    std::string importPath = toPath(import);
    std::string pathFromSdk = !sdkPath.empty() ? sdkPath + importPath : "";
    std::string pathFromSrc = !srcPath.empty() ? srcPath + importPath : "";
    std::string *libPaths = compiler->libPaths();
    std::list<std::string> pathsToSearch;
    
    // populate library paths
    for(int i = 0; libPaths[i] != ""; i++)
    {
        std::string lib = libPaths[i] + importPath;
        pathsToSearch.push_back(lib);
    }
    
    if(!pathFromSrc.empty())
        pathsToSearch.push_back(pathFromSdk);
    
    if(!pathFromSrc.empty())
        pathsToSearch.push_back(pathFromSrc);
    
    // try resolve all
    for(std::string path : pathsToSearch)
        if(Symbol *symbol = loadImport(import, name, path))
            pushUnique(results, symbol);
}

Symbol *HydroVisitor::tryResolve(SimpleName *id)
{
    std::string name = id->value();
    HydroCompiler *compiler = static_cast<HydroCompiler *>(_compiler);
    std::list<Symbol *> results;
    
    // try resolve locally using current scope
    if(Symbol *local = resolve(name))
    {
        // success!
        pushUnique(results, local);
    }
    
    // try resolve already imported symbol
    for(Name *imp : _imports)
    {
        std::vector<std::string> tokens;
        split(tokens, imp->value(), '.');
        Scope *scope = _globals;
        
        // try resolve import if already loaded
        for(int i = 0; i < tokens.size(); i++)
        {
            std::string pkgOrDef = tokens[i];
            
            if(Symbol *symbol = resolve(pkgOrDef, scope))
            {
                scope = symbol->ownScope(); // look in next scope
                
                if(scope)
                {
                    // keep trying
                    continue;
                }
                else if(i == tokens.size() - 1 && symbol->nameOf() == name)
                {
                    // success!
                    pushUnique(results, symbol);
                    scope = nullptr;
                    break;
                }
            }
            
            // fail
            break;
        }
        
        if(scope && results.empty())
        {
            // try resolve symbol is scope
            if(Symbol *symbol = resolve(name, scope))
            {
                // success!
                pushUnique(results, symbol);
            }
        }
    }
    
    // if all not results exist or all results are packages
    if(results.empty() || (results.size() > 0 && results.size() == countPackages(results)))
    {
        // try resolve by loading imports
        for (Name *imp : _imports)
            tryResolveImport(results, imp, name);
    }

    // validate results
    if(results.size() == 1)
    {
        if(PackageSymbol *pkg = dynamic_cast<PackageSymbol *>(results.front()))
        {
            // error! packages cannot be referenced
            appendError("Encountered illegal access of referenced package '" + pkg->name()->value() + "'.", id);
            return nullptr;
        }
        
        // success! (subject is not ambiguous)
        return results.front();
    }
    else if(!results.empty())
    {
        Symbol *target = nullptr;
        
        for(Symbol *symbol : results)
        {
            if(PackageSymbol *pkg = dynamic_cast<PackageSymbol *>(symbol))
            {
                // ignore package reference
                continue;
            }
            else if(target)
            {
                // conflict error
                appendError("Multipe resource conflicts exist for reference '" + name + "'.", id);
                return nullptr;
            }
            else
            {
                target = nullptr;
            }
        }
        
        if(!target)
        {
            // not an object (packages are not objects)
            appendError("Reference '" + name + "' does not point to an object.", id);
        }
        
        return target;
    }
    else if(compiler->mode() == h3o_compile_aot)
    {
        // not found
        appendError("Could not resolve reference to unknown symbol '" + name + "'.", id);
    }
    
    // fail
    return nullptr;
}

Symbol *HydroVisitor::tryResolve(QualifiedName *qualifiedName)
{
    std::string name = qualifiedName->name()->value();
    HydroCompiler *compiler = static_cast<HydroCompiler *>(_compiler);
    std::list<Symbol *> results;
    
    if(Symbol *reference = resolveImportByName(qualifiedName->qualifier(), qualifiedName->name()->value()))
    {
        // success!
        pushUnique(results, reference);
    }
    
    // if all not results exist or all results are packages
    if(results.empty() || (results.size() > 0 && results.size() == countPackages(results)))
    {
        // try resolve as import
        tryResolveImport(results, qualifiedName->name(), name);
    }
    
    // validate results
    if(results.size() == 1)
    {
        if(PackageSymbol *pkg = dynamic_cast<PackageSymbol *>(results.front()))
        {
            // error! packages cannot be referenced
            appendError("Encountered illegal access of referenced package '" + pkg->name()->value() + "'.", qualifiedName->name());
            return nullptr;
        }
        
        // success! (subject is not ambiguous)
        return results.front();
    }
    else if(!results.empty())
    {
        Symbol *target = nullptr;
        
        for(Symbol *symbol : results)
        {
            if(PackageSymbol *pkg = dynamic_cast<PackageSymbol *>(symbol))
            {
                // ignore package reference
                continue;
            }
            else if(target)
            {
                // conflict error
                appendError("Multipe resource conflicts exist for reference '" + name + "'.", qualifiedName->name());
                return nullptr;
            }
            else
            {
                target = nullptr;
            }
        }
        
        if(!target)
        {
            // not an object (packages are not objects)
            appendError("Reference '" + name + "' does not point to an object.", qualifiedName->name());
        }
        
        return target;
    }
    else if(compiler->mode() == h3o_compile_aot)
    {
        // not found
        appendError("Could not resolve reference to unknown symbol '" + name + "'.", qualifiedName->name());
    }
    
    // fail
    return nullptr;
}


} // namespace hydro

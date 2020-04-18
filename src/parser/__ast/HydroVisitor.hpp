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

#ifndef __h3o_HydroVisitor__
#define __h3o_HydroVisitor__

#include <vector>
#include <list>
#include <fstream>

// visitor
#include "AstVisitor.hpp"

// compiler
#include "../Compiler.hpp"

// program and parse tree
#include "Program.hpp"

// expressions
#include "EmptyExpr.hpp"
#include "Nested.hpp"
#include "XmlInit.hpp"
#include "UnaryOp.hpp"
#include "BinaryOp.hpp"
#include "TernaryOp.hpp"
#include "Call.hpp"
#include "ArrayInit.hpp"
#include "ObjectInit.hpp"
#include "JsonInit.hpp"
#include "DictInit.hpp"
#include "TupleInit.hpp"
#include "NamedTupleInit.hpp"
#include "StringTemplate.hpp"
#include "AFunc.hpp"

// statements
#include "EmptyStmt.hpp"
#include "ExprStmt.hpp"
#include "Block.hpp"
#include "IfStmt.hpp"
#include "ForStmt.hpp"
#include "ForEachStmt.hpp"
#include "WhileStmt.hpp"
#include "DoStmt.hpp"
#include "ContinueStmt.hpp"
#include "BreakStmt.hpp"
#include "ThrowStmt.hpp"
#include "TryStmt.hpp"
#include "SwitchStmt.hpp"
#include "ReturnStmt.hpp"
#include "ImportStmt.hpp"

// declarations
#include "SimpleName.hpp"
#include "QualifiedName.hpp"
#include "VarDecl.hpp"
#include "PackageDecl.hpp"
#include "LibraryDecl.hpp"
#include "FuncDecl.hpp"
#include "ClassDecl.hpp"
#include "ConstructorDecl.hpp"
#include "PropertyDecl.hpp"
#include "GetterDecl.hpp"
#include "SetterDecl.hpp"
#include "MethodDecl.hpp"
#include "ActDecl.hpp"
#include "EventDecl.hpp"
#include "EtypeDecl.hpp"
#include "InterfaceDecl.hpp"
#include "EnumDecl.hpp"
#include "ParamDecl.hpp"

// helpers
#include "../../utility/stringhelper.hpp"

namespace hydro
{

class HydroVisitor : public AstVisitor
{
private:
    Symbol *loadImport(Name *import, std::string name, std::string path);
    void tryResolveImport(std::list<Symbol *> &results, Name *import, std::string name);
    Symbol *resolveImportByName(Name *import, std::string name);
    
protected:
    
    Scope *_currentScope;
    std::vector<Scope *> _scopes;
    std::vector<Name *> _imports;
    Scope *_globals;
    
    static void pushUnique(std::list<Symbol *> &l, Symbol *element)
    {
        for(Symbol *x : l)
            if(x == element)
                return; // conflict exists
        
        // unique
        l.push_back(element);
    }
    
    static uint32_t countPackages(std::list<Symbol *> &l)
    {
        uint32_t n = 0;
        
        for(Symbol *symbol : l)
            if(dynamic_cast<PackageSymbol *>(symbol))
                n++;
        
        return n;
    }
    
    static BinaryOp *isDot(Expr *expr)
    {
        if(BinaryOp *op = dynamic_cast<BinaryOp *>(expr))
            if (op->tokenValue() == ".")
                return op;
        
        // fail
        return nullptr;
    }
    static bool fileExists(std::string fullPath)
    {
        std::ifstream file{fullPath};
        return file.good();
    }
    static std::string toPath(Name *name)
    {
        if(QualifiedName *qualified = dynamic_cast<QualifiedName *>(name))
        {
            // path from qualified name
            std::string path = toPath(qualified->qualifier());
            path += toPath(qualified->name());
            return '/' + path;
        }
        else if(SimpleName *simple = dynamic_cast<SimpleName *>(name))
        {
            // path from simple name
            return '/' + simple->value();
        }
        
        // fail
        return "";
    }
    
	static bool isAssignment(BinaryOp *op);

	// common nodes
	virtual void visitProgram(Program *prog);
	virtual void visitStmt(Stmt *stmt);
	virtual void visitBlock(Block *block);
	virtual void visitExpr(Expr *expr);

	// expressions
	virtual void visitEmpty(EmptyExpr *expr);
	virtual void visitNested(Nested *expr);
	virtual void visitLiteral(Literal *expr);
	virtual void visitIdentifier(SimpleName *id);
	virtual void visitNull(Literal *expr);
	virtual void visitUndefined(Literal *expr);
	virtual void visitPackage(Literal *expr);
	virtual void visitIdentity(Literal *expr);
	virtual void visitBool(Literal *expr);
	virtual void visitChar(Literal *expr);
	virtual void visitString(Literal *expr);
	virtual void visitStringTemplate(Literal *expr);
	virtual void visitUint(Literal *expr);
	virtual void visitInt(Literal *expr);
	virtual void visitHexadecimal(Literal *expr);
	virtual void visitBinary(Literal *expr);
	virtual void visitOctal(Literal *expr);
	virtual void visitUshort(Literal *expr);
	virtual void visitShort(Literal *expr);
	virtual void visitUlong(Literal *expr);
	virtual void visitLong(Literal *expr);
	virtual void visitFloat(Literal *expr);
	virtual void visitDouble(Literal *expr);
	virtual void visitInfinity(Literal *expr);
	virtual void visitNaN(Literal *expr);
	virtual void visitRegex(Literal *expr);
	virtual void visitObject(ObjectInit *expr);
	virtual void visitObjectProp(ObjectInitProperty *expr);
	virtual void visitJson(JsonInit *expr);
	virtual void visitJsonProp(JsonInitProperty *expr);
	virtual void visitJsonValue(Expr *expr);
	virtual void visitArray(ArrayInit *expr);
	virtual void visitArrayElement(Expr *expr);
	virtual void visitDict(DictInit *expr);
	virtual void visitDictEntry(DictInitEntry *expr);
	virtual void visitXml(XmlInit *expr);
	virtual void visitLambda(AFunc *lambda); // anonymous functions

	// operators
	virtual void visitUnaryOp(UnaryOp *op);
	virtual void visitBinaryOp(BinaryOp *op);
	virtual void visitAssign(BinaryOp *op);
	virtual void visitTernaryOp(TernaryOp *op);
	virtual void visitCall(Call *call);
	virtual void visitCallArgs(CallArgs *args);

	// statements
	virtual void visitEmptyStmt(EmptyStmt *stmt);
	virtual void visitExprStmt(ExprStmt *stmt);
	virtual void visitBody(Stmt *body);
	virtual void visitIf(IfStmt *stmt);
	virtual void visitElse(ElseStmt *stmt);
	virtual void visitThrow(ThrowStmt *stmt);
	virtual void visitTry(TryStmt *stmt);
    virtual void visitCatch(CatchClause *stmt);
	virtual void visitFor(ForStmt *stmt);
	virtual void visitForEach(ForEachStmt *stmt);
	virtual void visitDo(DoStmt *stmt);
	virtual void visitWhile(WhileStmt *stmt);
	virtual void visitContinue(ContinueStmt *stmt);
	virtual void visitBreak(BreakStmt *stmt);
	virtual void visitSwitch(SwitchStmt *stmt);
	virtual void visitCase(CaseClause *stmt);
	virtual void visitDefault(DefaultClause *stmt);
	virtual void visitReturn(ReturnStmt *stmt);
	virtual void visitImport(ImportStmt *stmt);
    
	// declarations
	virtual void visitName(Name *name);
	virtual void visitPackageSpec(PackageSpec *pkg);
	virtual void visitDecl(Decl *decl);
	virtual void visitVar(VarDecl *var);
	//virtual void visitConst(ConstDecl* cnst);
	//virtual void visitLet(LetDecl* let);
    virtual void visitPackage(PackageDecl *pkg);
    virtual void visitLibrary(LibraryDecl *lib);
	virtual void visitFunc(FuncDecl *func);
	virtual void visitClass(ClassDecl *clss);
	virtual void visitConstructor(ConstructorDecl *construct);
	virtual void visitProperty(PropertyDecl *prop);
	virtual void visitGetter(GetterDecl *get);
	virtual void visitSetter(SetterDecl *set);
	virtual void visitMethod(MethodDecl *meth);
	virtual void visitAction(ActDecl *act);
	virtual void visitEvent(EventDecl *evnt);
	virtual void visitEtype(EtypeDecl *etype);
    virtual void visitParam(ParamDecl *param);
	virtual void visitInterface(InterfaceDecl *it);
	virtual void visitEnum(EnumDecl *enm);
	//virtual void visitElement(ElementDecl *elem); (?)
	//virtual void visitState(StateDecl *state); (?)
    
    virtual Scope *pushScope(Decl *decl);
    virtual Scope *popScope();
    virtual Symbol *resolve(std::string name, Scope *scopeToUse);
    virtual Symbol *resolve(Name *name, Scope *scopeToUse);
    virtual Symbol *resolve(std::string name);
    virtual Symbol *resolve(Name *name);
    virtual Symbol *tryResolve(SimpleName *id);
    virtual Symbol *tryResolve(QualifiedName *qualifiedName);
    
public:
	HydroVisitor(class HydroCompiler *compiler, Scope *globals);
	virtual ~HydroVisitor();
	virtual void visit(vp_element e);
};

} // namespace hydro

#endif /* __h3o_HydroVisitor__ */

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

#ifndef __h3o_HydroParser__
#define __h3o_HydroParser__

// parser
#include "errors.hpp"
#include "Compiler.hpp"
#include "Parser.hpp"
#include "HydroLexer.hpp"
#include "__ast/Program.hpp"

// expressions
#include "__ast/EmptyExpr.hpp"
#include "__ast/Nested.hpp"
#include "__ast/UnaryOp.hpp"
#include "__ast/BinaryOp.hpp"
#include "__ast/TernaryOp.hpp"
#include "__ast/Call.hpp"
#include "__ast/Literal.hpp"
#include "__ast/NullLiteral.hpp"
#include "__ast/ArrayInit.hpp"
#include "__ast/DictInit.hpp"
#include "__ast/TupleInit.hpp"
#include "__ast/NamedTupleInit.hpp"
#include "__ast/ObjectInit.hpp"
#include "__ast/JsonInit.hpp"
#include "__ast/XmlInit.hpp"
#include "__ast/StringTemplate.hpp"
#include "__ast/FuncExpr.hpp"
#include "__ast/ActExpr.hpp"

// statements
#include "__ast/ExprStmt.hpp"
#include "__ast/EmptyStmt.hpp"
#include "__ast/Block.hpp"
#include "__ast/IfStmt.hpp"
#include "__ast/ForStmt.hpp"
#include "__ast/ForEachStmt.hpp"
#include "__ast/DoStmt.hpp"
#include "__ast/WhileStmt.hpp"
#include "__ast/ContinueStmt.hpp"
#include "__ast/BreakStmt.hpp"
#include "__ast/ReturnStmt.hpp"
#include "__ast/BreakStmt.hpp"
#include "__ast/TryStmt.hpp"
#include "__ast/ThrowStmt.hpp"
#include "__ast/SwitchStmt.hpp"
#include "__ast/ImportStmt.hpp"

// declarations
#include "__ast/PackageDecl.hpp"
#include "__ast/LibraryDecl.hpp"
#include "__ast/VarDecl.hpp"
#include "__ast/FuncDecl.hpp"
#include "__ast/ActDecl.hpp"
#include "__ast/ClassDecl.hpp"
#include "__ast/ConstructorDecl.hpp"
#include "__ast/PropertyDecl.hpp"
#include "__ast/GetterDecl.hpp"
#include "__ast/SetterDecl.hpp"
#include "__ast/MethodDecl.hpp"
#include "__ast/EventDecl.hpp"
#include "__ast/EtypeDecl.hpp"
#include "__ast/ParamDecl.hpp"

// symbols
#include "__ast/SimpleName.hpp"
#include "__ast/QualifiedName.hpp"
#include "__ast/PackageSymbol.hpp"
#include "__ast/LibrarySymbol.hpp"
#include "__ast/VarSymbol.hpp"
#include "__ast/FuncSymbol.hpp"
#include "__ast/ClassSymbol.hpp"
#include "__ast/ConstructorSymbol.hpp"
#include "__ast/EtypeSymbol.hpp"
#include "__ast/EventSymbol.hpp"
#include "__ast/ParamSymbol.hpp"

// scope
#include "__ast/SymbolTable.hpp"

namespace hydro
{

class HydroParser final : public Parser
{
private:
	ast_scope _globalScope;
	ast_scope _fileScope;
	ast_program _program;

	// speculation
	bool specAllowLine();
	bool specID();
	bool specBool();
	bool specChar();
	bool specString();
	bool specStringTemplate();
	bool specBinary();
	bool specHexadecimal();
	bool specOctal();
	bool specUshort();
	bool specShort();
	bool specUint();
	bool specInt();
	bool specUlong();
	bool specLong();
	bool specFloat();
	bool specDouble();
	bool specNumber();
	bool specNaN();
	bool specInfinity();
	bool specNull();
	bool specUndefined();
	bool specIdentityRef();
	bool specPackageRef();
	bool specDirective();
	bool specExpr(int minPrec);
	bool specExpr();
	bool assumeOpnd();
	bool specOpnd();
	bool specUnaryOp();
	bool assumeUnaryOp();
	bool assumeObject();
	bool specObject();
	bool assumeJson();
	bool specJson();
	bool assumeArray();
	bool specArray();
	bool assumeDict();
	bool specDict();
	bool assumeRegex();
	bool specRegex();
	bool assumeXml();
	bool specXml();
	bool specNestedExpr();
	bool assumeTuple();
	bool specTuple();
	bool assumeNamedTuple();
	bool specNamedTuple();
    bool assumeNamedArgs();
    
	// other expressions
	bool specAfunc(); // anonymous function

	// control structure speculation
	bool specEos();
	bool specStmt();
	bool specBlock();
	bool specNilBlock();
	bool specIf();
	bool specElseIf();
	bool specElse();
	bool specWhile();
	bool specDoWhile();
	bool specAlways(); // ?
	bool specFor();
	bool specForIn();
	bool specForEachIn();
	bool specSwitch();
	bool specCase();
	bool specDefault();
	bool specBreak();
	bool specTry();
	bool specCatch();
    bool assumeThrow();

	// statement speculation
	bool specImport();

	// declaration speculation
	bool isModifier(std::string);
	bool specModifiers(bool modRequired = false);
    bool specPackage();
    bool specLibrary();
	bool specVar();
	bool assumeFunc();
    bool assumeAnonymousFunc();
    bool assumeAction();
    bool assumeAnonymousAction();
	bool assumeReturn();
    bool assumeEtype();
    bool assumeEvent();
    bool assumeParam();
	bool assumeClass();
	bool specProperty();
	bool specGetter();
	bool specSetter();
	bool specConstructor();
	bool specMethod();
	bool specEvent();
	bool specEtype();
    bool specAttribute();

	// operator precedence table
	int binaryPrec(lex_token op);
	int binaryPrec(std::string op);
	int binaryAssoc(lex_token op);
	int binaryAssoc(std::string op);
	bool isBinaryOp(lex_token op);
	bool isBinaryOp(std::string op);
	int unaryPrec(lex_token op);
	int unaryPrec(std::string op);
	int postfixPrec(lex_token op);
	int postfixPrec(std::string op);
	bool isPostfix(lex_token op);
	bool isPostfix(std::string op);

	// parsing
	ast_expr computeExpr();
	ast_expr computeExpr(int minPrec);
	ast_expr emptyExpr(); // helper
	ast_expr parseOpnd();
	ast_literal buildString(std::vector<lex_token> &stringTokens);
	ast_expr parseStringTemplate();
	SimpleName *parseSimpleName();
    QualifiedName *parseQualifiedName();
    ast_name parseName();
	ast_package parsePackageSpec();
	ast_name parseID();
	ast_unary parseUnaryOp();
	ast_binary parseBinaryOp(ast_expr lhs, lex_token token, ast_expr rhs);
	ast_cargs parseCallArgs();
    NamedCallArgs *parseNamedArgs();
	ast_nested parseNested();
	ast_tuple parseTuple();
	ast_ntuple parseNamedTuple();
	ast_object parseObject();
	ast_json parseJson();
	ast_expr parseJsonValue();
	ast_array parseJsonArray();
	ast_array parseArray();
	ast_dict parseDict();
	ast_expr parseRegex();
	ast_xml parseXml();

	// parsing
	void parseEos();
	ast_stmt parseStmt(bool eos = true);
	ast_import parseImport();
	ast_mod parseModifiers();
	ast_var parseVar();
	ast_typeref parseTypeRef();
	ast_import parseImportType();
	ast_htype parseTypeSpec();
	ast_htype parseTypeDecl();
	ast_block parseBlock();
	ast_stmt parseBody();
	ast_if parseIf();
	ast_try parseTry();
    CatchClause *parseCatch();
    ThrowStmt *parseThrow();
	ast_break parseBreak();
	ast_continue parseContinue();
	ast_for parseFor();
	//ast_foreach parseForEach();
	ast_do parseDoWhile();
	ast_while parseWhile();
	ast_switch parseSwitch();
	ast_case parseCase();
	ast_default parseDefault();
    PackageDecl *parsePackage();
    LibraryDecl *parseLibrary();
	FuncDecl *parseFunc();
    FuncExpr *parseFuncExpr();
    ActDecl *parseAction();
    ActExpr *parseActionExpr();
	ast_fargs parseFuncArgs();
	ast_return parseReturn();
    EventDecl *parseEvent();
    EtypeDecl *parseEtype();
    ParamDecl *parseParam();
	ast_class parseClass();
	ast_construct parseConstructor();
	ast_property parseProperty();
	ast_getter parseGetter();
	ast_setter parseSetter();
	ast_method parseMethod();
    
    // package handlers and helpers
    PackageSymbol *buildPackageTree(Name *name, Scope *scope = nullptr);
    PackageSymbol *resolvePackage(Name *name, Scope *scopeToUse = nullptr);
    PackageSymbol *loadEnclosingPackage();
    PackageSymbol *loadParentPackage(Name *packageName);
    
    // class helpers
    ClassSymbol *getEnclosingClass();
    Symbol *getOwnerOf(Scope *scope);
    
protected:
	virtual void parseNext() override;
	virtual void onParseInit() override;
	virtual void reset() override;

public:
	HydroParser(class HydroCompiler *compiler, Program *program, Scope* globalScope);
	virtual ~HydroParser();
    void import(Name *name);
	ast_scope globals() const { return _globalScope; }
	void globals(Scope *scope) { _globalScope = scope; }
	ast_scope locals() const { return _fileScope; }
	ast_program program() const { return _program; }
};

typedef HydroParser *hydro_parser;

} // namespace hydro

#endif /* __h3o_HydroParser__ */

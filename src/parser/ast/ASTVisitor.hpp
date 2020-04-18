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

#ifndef __h3o_ASTVisitor__
#define __h3o_ASTVisitor__

namespace hydro
{

class ASTVisitor
{
public:
    
    ASTVisitor() {}
    virtual ~ASTVisitor() {}
    virtual bool visit(class ParseTree *tree) = 0;
    virtual bool visit(class ASTNode *node) = 0;
    virtual bool visit(class SingleLineComment *comment) = 0;
    virtual bool visit(class MultiLineComment *comment) = 0;
    virtual bool visit(class Expression *expr) = 0;
    virtual bool visit(class BooleanLiteral *expr) = 0;
    virtual bool visit(class CharacterLiteral *expr) = 0;
    virtual bool visit(class StringLiteral *expr) = 0;
    virtual bool visit(class BinaryLiteral *expr) = 0;
    virtual bool visit(class HexadecimalLiteral *expr) = 0;
    virtual bool visit(class UnsignedIntegerLiteral *expr) = 0;
    virtual bool visit(class IntegerLiteral *expr) = 0;
    virtual bool visit(class UnsignedShortLiteral *expr) = 0;
    virtual bool visit(class ShortLiteral *expr) = 0;
    virtual bool visit(class UnsignedLongLiteral *expr) = 0;
    virtual bool visit(class LongLiteral *expr) = 0;
    virtual bool visit(class FloatLiteral *expr) = 0;
    virtual bool visit(class DoubleLiteral *expr) = 0;
    virtual bool visit(class RegularExpressionLiteral *expr) = 0;
    virtual bool visit(class ObjectInitializer *expr) = 0;
    virtual bool visit(class JsonInitializer *expr) = 0;
    virtual bool visit(class ListInitialzier *expr) = 0;
    virtual bool visit(class SimpleName *name) = 0;
    virtual bool visit(class QualifiedName *name) = 0;
    virtual bool visit(class LambdaExpression *expr) = 0;
    virtual bool visit(class XMLElementNode *node) = 0;
    virtual bool visit(class XMLAttributeNode *node) = 0;
    virtual bool visit(class XMLTextNode *node) = 0;
    virtual bool visit(class XMLCommentNode *node) = 0;
    virtual bool visit(class XMLCDATANode *node) = 0;
    virtual bool visit(class XMLProcessingInstructionNode *node) = 0;
    virtual bool visit(class UnaryExpression *op) = 0;
    virtual bool visit(class BinaryExpression *op) = 0;
    virtual bool visit(class TernaryExpression *op) = 0;
    virtual bool visit(class Invocation *op) = 0;
    virtual bool visit(class ConstructorInvocation *op) = 0;
    virtual bool visit(class SuperConstructorInvocation *op) = 0;
    virtual bool visit(class MethodInvocation *op) = 0;
    virtual bool visit(class SuperMethodInvocation *op) = 0;
    virtual bool visit(class SuperMethodReference *expr) = 0;
    virtual bool visit(class Assignment *op) = 0;
    virtual bool visit(class EmptyStatement *stmt) = 0;
    virtual bool visit(class ExprStatement *stmt) = 0;
    virtual bool visit(class IfStatement *stmt) = 0;
    virtual bool visit(class ElseStatement *stmt) = 0;
    virtual bool visit(class ThrowStatement *stmt) = 0;
    virtual bool visit(class TryStatement *stmt) = 0;
    virtual bool visit(class CatchClause *stmt) = 0;
    virtual bool visit(class SwitchStatement *stmt) = 0;
    virtual bool visit(class CaseClause *stmt) = 0;
    virtual bool visit(class DefaultClause *stmt) = 0;
    virtual bool visit(class BreakStatement *stmt) = 0;
    virtual bool visit(class ContinueStatement *stmt) = 0;
    virtual bool visit(class ForStatement *stmt) = 0;
    virtual bool visit(class ForEachStatement *stmt) = 0;
    virtual bool visit(class DoStatement *stmt) = 0;
    virtual bool visit(class WhileStatement *stmt) = 0;
    virtual bool visit(class ImportDeclaration *decl) = 0;
    virtual bool visit(class VariableDeclaration *decl) = 0;
    virtual bool visit(class FunctionDeclaration *decl) = 0;
    virtual bool visit(class ClassDeclaration *decl) = 0;
    virtual bool visit(class ConstructorDeclaration *decl) = 0;
    virtual bool visit(class PropertyDeclaration *decl) = 0;
    virtual bool visit(class GetterDeclaration *decl) = 0;
    virtual bool visit(class SetterDeclaration *decl) = 0;
    virtual bool visit(class MethodDeclaration *decl) = 0;
    virtual bool visit(class ContextDeclaration *decl) = 0;
    virtual bool visit(class EventDeclaration *decl) = 0;
    virtual bool visit(class EtypeDeclaration *decl) = 0;
    virtual bool visit(class EventParameterDeclaration *decl) = 0;
    virtual bool visit(class StateDeclaration *decl) = 0;
    virtual bool visit(class InterfaceDeclaration *decl) = 0;
    virtual bool visit(class OperatorDeclaration *decl) = 0;
    virtual bool visit(class CastDeclaration *delc) = 0;
    
};

} // namespace hydro

#endif /* __h3o_ASTVisitor__ */

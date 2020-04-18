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

#ifndef __h3o_errors__
#define __h3o_errors__

#define HVM_COMPILE_ERROR_INVALID_NUMBER_SYNTAX "Invalid syntax for assumed numerical literal."

#define HVM_COMPILE_ERROR_UNKNOWN_ESCAPE_SEQUENCE "Unrecognized escape sequence."

#define HVM_COMPILE_ERROR_INCOMPLETE_ESCAPE_SEQUENCE "Incomplete escape sequence. Reached the end of the token stream."

#define HVM_COMPILE_ERROR_UNTERMINATED_STRING_LITERAL "Unterminated string literal."

#define HVM_COMPILE_ERROR_UNTERMINATED_STRING_TEMPLATE "Unterminated string template."

#define HVM_COMPILE_ERROR_INVALID_UNARY "Expecting a unary operator."

#define HVM_COMPILE_ERROR_INVALID_BINARY "Expecting a binary operator."

#define HVM_COMPILE_ERROR_LAMBDA_PACKAGE "Package specification is unexpected on anonomous function."

#define HVM_COMPILE_ERROR_SYMBOL_CONFLICT "Redefinition of the symbol '%s'."

#define HVM_COMPILE_ERROR_CONSTRUCTOR_NO_CLASS "Constructor defined outside of a class."

#define HVM_COMPILE_ERROR_CLOSURE_PACKAGE "Package specification is unexpected on nested delaration."

#define HVM_COMPILE_ERROR_CONSTRUCTOR_PACKAGE "Package specification is unexpected on constructor declaration."

#define HVM_COMPILE_ERROR_CONSTRUCTOR_RETURN_TYPE "Type specifier is unexpected on constructor declaration."

#define HVM_COMPILE_ERROR_CONSTRUCTOR_RETURN "Constructors cannot return a value."

#define HVM_COMPILE_ERROR_INVALID_SUPER_CONSTRUCTOR_INVOCATION "Call to the super constructor must be the first statement in the constructor body."

#define HVM_COMPILE_ERROR_METHOD_NO_CLASS "Method defined outside of a class."

#endif /* __h3o_errors__ */

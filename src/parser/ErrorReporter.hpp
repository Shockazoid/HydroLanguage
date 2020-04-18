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

#ifndef __h3o_ErrorReporter__
#define __h3o_ErrorReporter__

#include "Compiler.hpp"
#include "Token.hpp"

namespace hydro
{

class ErrorReporter
{
public:
    ErrorReporter(Compiler *compiler);
    virtual void appendError(std::string errorTxt, std::string source, uint32_t errorLine, int32_t errorIndex);
    virtual void appendError(std::string errorTxt, lex_token erroneousToken = nullptr);
    virtual void appendWarning(std::string errorTxt, std::string source, uint32_t errorLine, int32_t errorIndex);
    virtual void appendWarning(std::string errorTxt, lex_token erroneousToken = nullptr);
};

} // namespace hydro

#endif /* __h3o_ErrorReporter__ */

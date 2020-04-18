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

#include "Process.hpp"

namespace hydro
{

Process::Process(Token *token, Modifier *mod, SimpleName *name, Block *body, PackageSymbol *package, ProcessSymbol *symbol) : ScopedDecl{token, mod, name, body, package, symbol} {}

Process::~Process() {}

} // namespace hydro

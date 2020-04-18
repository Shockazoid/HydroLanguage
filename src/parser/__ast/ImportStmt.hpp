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

#ifndef __h3o_ImportStmt__
#define __h3o_ImportStmt__

#include "Name.hpp"
#include "Stmt.hpp"

namespace hydro
{

class ImportStmt : public Stmt
{
private:
	Name *_name;

public:
	ImportStmt(Token *token, Name *name);
	virtual ~ImportStmt();
	Name *name() const { return _name; }
};

typedef ImportStmt *ast_import;

} // namespace hydro

#endif /* __h3o_ImportStmt__ */

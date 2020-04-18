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

#ifndef __h3o_Name__
#define __h3o_Name__

#include "Literal.hpp"

namespace hydro
{

class Name : public Literal
{
protected:
    ast_node _reference;
    
public:
    Symbol *binding{nullptr};
	Name(lex_token tkn = nullptr);
    virtual ~Name();
    void reference(ast_node reference) { _reference = reference; }
    ast_node reference() const { return _reference; }
    virtual std::string value() const = 0;
    virtual bool isSimpleName() const = 0;
    virtual bool isQualifiedName() const = 0;
};

typedef Name *ast_name;

} // namespace hydro

#endif /* __h3o_Name__ */

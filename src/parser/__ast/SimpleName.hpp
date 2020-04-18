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

#ifndef __h3o_SimpleName__
#define __h3o_SimpleName__

#include "Name.hpp"

namespace hydro
{

class SimpleName final : public Name
{
private:
    std::string _value;
    
public:
    bool isIdentifier;
    SimpleName(Token *token, bool identifier = false);
    SimpleName(std::string value, bool identifier = false);
    virtual ~SimpleName();
    virtual std::string value() const override { return _value; }
    virtual bool isSimpleName() const override { return true; }
    virtual bool isQualifiedName() const override { return false; }
};

} // namespace hydro

#endif /* __h3o_SimpleName__ */

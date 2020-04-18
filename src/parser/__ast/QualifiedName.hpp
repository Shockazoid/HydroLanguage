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

#ifndef __h3o_QualifiedName__
#define __h3o_QualifiedName__

#include "SimpleName.hpp"

namespace hydro
{
    
class QualifiedName final : public Name
{
private:
    SimpleName *_name;
    Name *_qualifier;
    
public:
    QualifiedName(Name *qualifier, SimpleName *name);
    virtual ~QualifiedName();
    SimpleName *name() const { return _name; }
    Name *qualifier() const { return _qualifier; }
    virtual std::string value() const override { return _qualifier->value() + "." + _name->value(); }
    virtual bool isSimpleName() const override { return false; }
    virtual bool isQualifiedName() const override { return true; }
};

} // namespace hydro

#endif /* __h3o_QualifiedName__ */

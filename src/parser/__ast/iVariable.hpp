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

#ifndef __h3o_iVariable__
#define __h3o_iVariable__

namespace hydro
{

class iVariable
{
public:
    virtual Name *name() const = 0;
    virtual TypeSpec *type() const = 0;
    virtual Expr *defaultValue() const = 0;
};

} // namespace hydro

#endif /* __h3o_iVariable__ */

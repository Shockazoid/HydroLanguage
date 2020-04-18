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

#ifndef __h3o_Library__
#define __h3o_Library__

#include <string>

#include "../parser/Source.hpp"
#include "../utility/classhelper.hpp"

namespace hydro
{

class Library
{
public:
    Library() {}
    virtual ~Library() {}
    Source *resolve(std::string qname) const
    {
        qname = get_assembly_name(qname); // convert qname is assembly name
        if(!qname.empty())
            return tryResolve(qname);
        
        // fail
        return nullptr;
    }
    
protected:
    virtual Source *tryResolve(std::string qname) const = 0;
    static bool compareNames(std::string a, std::string b)
    {
        if(a == b)
            return true;
        
        a = get_assembly_name(a);
        b = get_assembly_name(b);
        
        return a == b;
    }
};

} // namespace hydro

#endif /* __h3o_Library__ */

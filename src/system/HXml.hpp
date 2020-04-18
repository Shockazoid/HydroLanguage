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

#ifndef __h3o_HXml__
#define __h3o_HXml__

#include "HObject.hpp"

namespace hydro
{

class HXml : public HObject
{
    friend class HvmEnv;
    
private:
    HXml(HvmEnv *env, HClass *xmlClass);
    void construct(hvalue arg0);
    
public:
    virtual ~HXml();
};

typedef object_ptr<HXml> hxml;

} // namespace hydro

#endif /* __h3o_HXml__ */

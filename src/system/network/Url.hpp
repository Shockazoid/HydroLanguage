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

#ifndef __h3o_Url__
#define __h3o_Url__

#include "../HObject.hpp"

namespace hydro
{

class Url : public HObject
{
private:
    std::string _url;
    
public:
    Url(HvmEnv *env, HClass *urlClass);
    virtual ~Url();
    void construct(HvmEnv *env, std::string url);
    std::string valueOf() const;
};

typedef object_ptr<Url> hurl;

} // namespace hydro

#endif /* __h3o_Url__ */

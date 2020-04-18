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

#ifndef __h3o_OnService__
#define __h3o_OnService__

#include "RuntimeService.hpp"

namespace hydro
{

struct OnService : RuntimeService
{
    OnService(HvmEnv *env) : RuntimeService{env} {}
    virtual ~OnService() {}
    void event(hvalue target) const;
    void activate(hvalue target) const;
    void deactivate(hvalue target) const;
    void init(hvalue target) const;
    void ready(hvalue target) const;
    void complete(hvalue target) const;
    void click(hvalue target) const;
    void mouseDown(hvalue target) const;
    void mouseUp(hvalue target) const;
    void mouseOver(hvalue target) const;
    void mouseLeave(hvalue target) const;
    void mouseWheel(hvalue target) const;
    void doubleClick(hvalue target) const;
    void progress(hvalue target) const;
    void ioError(hvalue target) const;

};

} // namespace hydro

#endif /* __h3o_OnService__ */

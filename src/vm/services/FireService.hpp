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

#ifndef __h3o_FireService__
#define __h3o_FireService__

#include <string>
#include <map>

#include "RuntimeService.hpp"
#include "../hvalue.hpp"
#include "../../system/HEventContext.hpp"
#include "../../system/HEventType.hpp"

namespace hydro
{

struct FireService : RuntimeService
{
    FireService(HvmEnv *env) : RuntimeService{env} {}
    virtual ~FireService() {}
    void event(hobject target, hetype type, std::map<std::string, hvalue> &params) const;
    void init(hobject target) const;
    void complete(hobject target) const;
    void activate(hobject target) const;
    void deactivate(hobject target) const;
    void click(hobject target, double x, double y, double delta = 0.0) const;
    void doubleClick(hobject target, double x, double y, double delta = 0.0) const;
    void mouseOver(hobject target, double x, double y, double delta = 0.0) const;
    void mouseLeave(hobject target, double x, double y, double delta = 0.0) const;
    void mouseUp(hobject target, double x, double y, double delta = 0.0) const;
    void mouseDown(hobject target, double x, double y, double delta = 0.0) const;
    void mouseWheel(hobject target, double x, double y, double delta = 0.0) const;
    void keyUp(hobject target, uint32_t charCode);
    void keyDown(hobject target, uint32_t charCode);
    void keyPressed(hobject target, uint32_t charCode);
    void touch(hobject target);
    void progress(hobject target, double bytesLoaded, double bytesTotal) const;
    void error(hobject target, std::string errorText) const;
    void ioError(hobject target, std::string errorText) const;
    void status() const;
};

} // namespace hydro

#endif /* __h3o_FireService__ */

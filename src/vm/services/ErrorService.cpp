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

#include "ErrorService.hpp"
#include "../HvmEnv.hpp"

namespace hydro
{
/*
template <typename... Arg>
static void callVoidFunc(void (*func)(Arg...), std::list<hvalue> &args)
{
    if (sizeof...(Arg) != args.size())
        throw std::runtime_error("Argument count mismatch on function pointer!");

    args.reverse();
    func(hvm_ref_cast<Arg>().do_cast(fetch_back(args))...);
}

template <typename ...Args>
const ErrorService::UncaughtHandler &ErrorService::UncaughtHandler::uncaught(void (*fn)(Args...)) const
{
    return *this;
}

template <typename ...Args>
const ErrorService::UncaughtHandler &ErrorService::UncaughtHandler::uncaught(hclass type, void (*fn)(Args...)) const
{
    return *this;
}

template <typename ...Args>
void ErrorService::UncaughtHandler::unsafe(void (*fn)(Args...)) const
{
    try
    {
        std::list<hvalue> args;
        AutoWireContext autoWire = AutoWireContext{};
        autoWire.env = service->_env;
        autoWire.vm = service->_vm;
        autoWire.cxt = service->_env->thread()->current();
        uint16_t nwired = 0;
        int _[] = {0, (hvm_auto_wire<Args>{}.put(args, autoWire, nwired), 0)...};
        (void)_;
        //if (args.size() < sizeof...(Args))
        //    args.push_back();
    }
    catch (VM::hvm_uncaught_error *err)
    {
        
    }
}
*/

bool ErrorService::pending() const
{
    auto cxt = _env->thread()->current();
    if(cxt)
        return cxt->errorPending;
    return false;
}

bool ErrorService::pending(HvmContext *cxt) const
{
    return cxt->errorPending;
}

} // namespace hydro

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

#include "RuntimeService.hpp"
#include "../HvmEnv.hpp"

namespace hydro
{

HObjectContext *RuntimeService::objectContext(hobject obj) const
{
    return obj->_cxt;
}

RuntimeService::RuntimeService(HvmEnv *env) : _env{env}, _vm{_env->_vm}, _globals{_env->_globals}, _debug{_env->_debug}, _currentContext{_env->_currentContext}, _mainContext{_env->_mainContext},  _sdkPath{_env->_sdkPath}, _srcPath{_env->_srcPath}, _libPath{_env->_libPath} {}

} // namespace hydro

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

#ifndef __h3o_hvm_compile_mode__
#define __h3o_hvm_compile_mode__

namespace hydro
{

enum hydro_compile_mode
{
    // code is compiled at build time
	h3o_compile_aot,
    
    // code is compiled at runtime before the application is running
    h3o_compile_jit,
    
    // code is compiled at runtime for and by the running application (some high-level language features are prohibited)
    h3o_compile_eval
};

} // namespace hydro

#endif /* __h3o_hvm_compile_mode__ */

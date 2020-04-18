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

#ifndef __h3o_defaults__
#define __h3o_defaults__

// thread limit
#ifndef HVM_MAX_THREADS
#define HVM_MAX_THREADS 40
#endif /* HVM_MAX_THREADS */

// operand limit
#ifndef HVM_MAX_OPNDS
#define HVM_MAX_OPNDS 4000
#endif /* HVM_MAX_THREADS */

// maximum size for the call stack (should be no more than 1000)
#ifndef HVM_MAX_STACK
#define HVM_MAX_STACK 1000
#endif /* HVM_MAX_STACK */

//#define HVM_DEBUG 1

#endif /* __h3o_hvmdefaults__ */

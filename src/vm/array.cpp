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

#include "array.hpp"

namespace hydro
{

array::array() : _length{0}
{
    // default
}

array::array(const array &arr) : _length{arr._length}
{
    // copy
}

array::array(array &&arr) : _length{arr._length} {}

array::array(uint64_t length) {}

} // namespace hydro

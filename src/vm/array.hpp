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

#ifndef __h3o_array__
#define __h3o_array__

#include <cstdlib>

namespace hydro
{

class array final
{
private:
    uint64_t _length;
    
public:
    array(); // default
    array(const array &arr); // copy
    array(array &&arr); // move
    array(uint64_t length);
    ~array();
    uint64_t length() const { return _length; }
};

} // namespace hydro

#endif /* __h3o_array__ */

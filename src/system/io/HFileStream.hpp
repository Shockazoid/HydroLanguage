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

#ifndef __h3o_HFileStream__
#define __h3o_HFileStream__

#include <fstream>

#include "../HObject.hpp"

namespace hydro
{

class HFileStream : public HObject
{
    friend HvmEnv;
    
    template <typename X>
    friend class NativeClassAllocator;
    
private:
    std::fstream _stream;
    HFileStream(HvmEnv *env, HClass *fileStreamClass);
    void construct();

public:
    virtual ~HFileStream();
    bool open(HvmEnv *env, hvalue path);
    void close();
    std::string read();
    char get();
    bool isOpen() { return _stream.is_open(); }
    bool isEof() { return _stream.eof(); }
};

} // namespace hydro

#endif /* __h3o_HFileStream__ */

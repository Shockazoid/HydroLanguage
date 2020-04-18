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

#include "HFileStream.hpp"
#include "../../vm/HvmEnv.hpp"

namespace hydro
{

HFileStream::HFileStream(HvmEnv *env, HClass *fileStreamClass) : HObject{env, fileStreamClass} {}

HFileStream::~HFileStream()
{
    close();
}

void HFileStream::construct() {}

bool HFileStream::open(HvmEnv *env, hvalue path)
{
    std::string spath = env->cast()->string(path);
    close();
    _stream.open(spath);
    return _stream.is_open();
}

void HFileStream::close()
{
    if(_stream.is_open())
        _stream.close();
}

std::string HFileStream::read()
{
    std::string s;
    
    while(!isEof())
    {
        if(char ch = get())
            if(!isEof())
                s += ch;
    }
    
    return s;
}

char HFileStream::get()
{
    if(_stream.eof() || !_stream.is_open())
        return '\0';
    
    return _stream.get();
}

} // namespace hydro

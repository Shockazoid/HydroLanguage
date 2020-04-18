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

#ifndef __h3o_Label__
#define __h3o_Label__

#include <string>
#include <vector>
#include <cstdlib>

#include "AssemblyNode.hpp"

namespace hydro
{

class LabelNode : public AssemblyNode
{
private:
    std::string _value;
    int32_t _addr;
    bool _defined;
    bool _written;
    std::vector<uint32_t> _fwdRefs;
    void resolveForwardReferences(bytearray &bytes);

public:
    LabelNode(std::string value);
    virtual ~LabelNode();
    virtual void build(Chunk *chunk) override;
    void addForwardReference(uint32_t addr) { _fwdRefs.push_back(addr); }
    void define() { _defined = true; }
    bool isDefined() const { return _defined; }
    bool isWritten() const { return _written; }
    std::string value() const { return _value; }
    int32_t addr() const { return _addr; }
};

} // namespace hydro

#endif /* __h3o_Label__ */

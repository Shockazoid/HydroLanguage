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

#ifndef __h3o_AssemblyNode__
#define __h3o_AssemblyNode__

#include <list>

#include "../../../parser/Token.hpp"
#include "../../../vm/Chunk.hpp"

namespace hydro
{

class AssemblyNode
{
private:
    Token *m_token;
    std::list<AssemblyNode *> m_children;
    AssemblyNode *m_parent;
    
public:
    AssemblyNode(Token *token = nullptr);
    virtual ~AssemblyNode();
    virtual void build(Chunk *chunk);
    void prependChild(AssemblyNode *child);
    void appendChild(AssemblyNode *child);
    void removeChild(AssemblyNode *child);
    Token *token() const { return m_token; }
};

} // namespace hydro

#endif /* __h3o_AssemblyNode__ */

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

#ifndef __h3o_StringLibrary__
#define __h3o_StringLibrary__

#include "Library.hpp"
#include "../parser/StringSource.hpp"

namespace hydro
{

class StringLibrary : public Library
{
public:
    StringLibrary() {}
    virtual ~StringLibrary() {}
    void appendScriptFromString(std::string hydroCode, std::string relativePath, std::string qname);
    
    std::string stripHydroExtension(std::string path)
    {
        // TODO traverse from back
        std::string noext = "";
        
        for(char ch : path)
            if (ch == '.')
                break;
            else
                noext += ch;
        
        return noext;
    }

    bool convertsToUnderscore(char ch)
    {
        return ch == '/' || ch == '_' || ch == '-' || ch == ' ';
    }

    std::string pathToQName(std::string relativePath)
    {
        relativePath = stripHydroExtension(relativePath);
        
        if(relativePath.length() > 0 && relativePath[0] == '/')
            relativePath = relativePath.substr(1);

        std::string qn = "";
        
        for(char ch : relativePath)
        {
            if(convertsToUnderscore(ch))
                qn += '/';
            else
                qn += ch;
        }
        
        return qn;
    }
    
    std::string escapeString(std::string value)
    {
        std::string newString = "";
        
        for(char ch : value)
            if(ch == '"') // double quotes
                newString += "\\\"";
            else if(ch == '\n') // linefeed
                newString += "\\n";
            else // everything else
                newString += ch;

        return newString;
    }
    
protected:
    struct hydro_script
    {
        std::string qname;
        std::string code;
        std::string path;
    };

    std::vector<hydro_script *> m_scripts;
    
    virtual Source *tryResolve(std::string qname) const override;
    
    friend class HvmEnv;
};

} // namespace hydro

#endif /* __h3o_StringLibrary__ */

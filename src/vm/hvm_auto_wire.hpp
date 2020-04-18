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

#ifndef __h3o_hvm_auto_wire__
#define __h3o_hvm_auto_wire__

#include <cstdlib>
#include <list>

#include "AutoWireContext.hpp"
#include "hvalue.hpp"

namespace hydro
{

template <typename T>
struct hvm_auto_wire
{
    
    void put(std::list<hvalue> &args, const AutoWireContext &autoWire, uint16_t &nwired)
    {
        if (typeid(T) != typeid(HvmEnv *) && typeid(T) != typeid(VM *) && typeid(T) != typeid(HvmContext *))
            return; // skip

        nwired++;

        for (auto it = args.begin(); it != args.end(); ++it)
        {
            hvalue val = *it; // dereference

            if (val.type() == typeid(HvmEnv *) || val.type() == typeid(VM *) || val.type() == typeid(HvmContext *))
            {
                // do nothing
                continue;
            }

            // insert before first non-wired argument
            if (typeid(T) == typeid(HvmEnv *))
                args.insert(it, hvalue{autoWire.env});
            else if (typeid(T) == typeid(VM *))
                args.insert(it, hvalue{autoWire.vm});
            else if (typeid(T) == typeid(HvmContext *))
                args.insert(it, hvalue{autoWire.cxt});
            return; // end
        }

        // push back
        if (typeid(T) == typeid(HvmEnv *))
            args.push_back(hvalue{autoWire.env});
        else if (typeid(T) == typeid(VM *))
            args.push_back(hvalue{autoWire.vm});
        else if (typeid(T) == typeid(VM *))
            args.push_back(hvalue{autoWire.cxt});
        // end
    }
    
    void put(hvalue instance, std::list<hvalue> &args, const AutoWireContext &autoWire, uint16_t &nwired)
    {
        if (typeid(T) != typeid(HvmEnv *) && typeid(T) != typeid(VM *) && typeid(T) != typeid(HvmContext *))
            return; // skip

        nwired++;

        // TODO insert instance
        
        for (auto it = args.begin(); it != args.end(); ++it)
        {
            hvalue val = *it; // dereference

            if (val.type() == typeid(HvmEnv *) || val.type() == typeid(VM *) || val.type() == typeid(HvmContext *))
            {
                // do nothing
                continue;
            }

            // insert before first non-wired argument
            if (typeid(T) == typeid(HvmEnv *))
                args.insert(it, hvalue{autoWire.env});
            else if (typeid(T) == typeid(VM *))
                args.insert(it, hvalue{autoWire.vm});
            else if (typeid(T) == typeid(HvmContext *))
                args.insert(it, hvalue{autoWire.cxt});
            return; // end
        }

        // push back
        if (typeid(T) == typeid(HvmEnv *))
            args.push_back(hvalue{autoWire.env});
        else if (typeid(T) == typeid(VM *))
            args.push_back(hvalue{autoWire.vm});
        else if (typeid(T) == typeid(VM *))
            args.push_back(hvalue{autoWire.cxt});
        // end
    }
};

template <typename T>
struct hvm_auto_wire <object_ptr<T>>
{
    
    void put(std::list<hvalue> &args, const AutoWireContext &autoWire, uint16_t &nwired)
    {
        if (typeid(T) != typeid(HvmEnv *) && typeid(T) != typeid(VM *) && typeid(T) != typeid(HvmContext *))
            return; // skip

        nwired++;

        for (auto it = args.begin(); it != args.end(); ++it)
        {
            hvalue val = *it; // dereference

            if (val.type() == typeid(HvmEnv *) || val.type() == typeid(VM *) || val.type() == typeid(HvmContext *))
            {
                // do nothing
                continue;
            }

            // insert before first non-wired argument
            if (typeid(T) == typeid(HvmEnv *))
                args.insert(it, hvalue{autoWire.env});
            else if (typeid(T) == typeid(VM *))
                args.insert(it, hvalue{autoWire.vm});
            else if (typeid(T) == typeid(HvmContext *))
                args.insert(it, hvalue{autoWire.cxt});
            return; // end
        }

        // push back
        if (typeid(T) == typeid(HvmEnv *))
            args.push_back(hvalue{autoWire.env});
        else if (typeid(T) == typeid(VM *))
            args.push_back(hvalue{autoWire.vm});
        else if (typeid(T) == typeid(VM *))
            args.push_back(hvalue{autoWire.cxt});
        // end
    }
    
    void put(hvalue instance, std::list<hvalue> &args, const AutoWireContext &autoWire, uint16_t &nwired)
    {
        if (typeid(T) != typeid(HvmEnv *) && typeid(T) != typeid(VM *) && typeid(T) != typeid(HvmContext *))
            return; // skip

        nwired++;
        
        // TODO insert instance

        for (auto it = args.begin(); it != args.end(); ++it)
        {
            hvalue val = *it; // dereference

            if (val.type() == typeid(HvmEnv *) || val.type() == typeid(VM *) || val.type() == typeid(HvmContext *))
            {
                // do nothing
                continue;
            }

            // insert before first non-wired argument
            if (typeid(T) == typeid(HvmEnv *))
                args.insert(it, hvalue{autoWire.env});
            else if (typeid(T) == typeid(VM *))
                args.insert(it, hvalue{autoWire.vm});
            else if (typeid(T) == typeid(HvmContext *))
                args.insert(it, hvalue{autoWire.cxt});
            return; // end
        }

        // push back
        if (typeid(T) == typeid(HvmEnv *))
            args.push_back(hvalue{autoWire.env});
        else if (typeid(T) == typeid(VM *))
            args.push_back(hvalue{autoWire.vm});
        else if (typeid(T) == typeid(VM *))
            args.push_back(hvalue{autoWire.cxt});
        // end
    }

};

} // namespace hydro

#endif /* __h3o_hvm_auto_wire__ */

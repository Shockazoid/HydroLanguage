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

#ifndef __h3o_Runtime__
#define __h3o_Runtime__

#include <vector>

#include "../system/HFunction.hpp"
#include "../system/HAction.hpp"
#include "../system/HClass.hpp"
#include "../system/HEventContext.hpp"
#include "../utility/classhelper.hpp"

namespace hydro
{

class Runtime final
{
	friend class HvmEnv;

private:
	struct runtime_definition
	{
		class_info *info; // qualified name
		hobject content;
		~runtime_definition() { delete info; }
	};

	Runtime *_parent;
	std::vector<runtime_definition *> _definitions;

public:
    Runtime();
    Runtime(Runtime *parent);
	~Runtime();
    
    hfunction bind(std::string qualifiedName, hfunction function);
    haction bind(std::string qualifiedName, haction action);
    hclass bind(std::string qualifiedName, hclass clss);
    hevent bind(std::string qualifiedName, hevent event);
    //void bind(std::string qualifiedName, hinterface interface);
    //void bind(std::string qualifiedName, hevent event);
    //void bind(std::string qualifiedName, haction action); (?)
	hobject find(std::string qualifiedName);
	hfunction findFunction(std::string qualifiedName);
	hclass findClass(std::string qualifiedName);
    hevent findEvent(std::string qualifiedName);
    Runtime *parent() const { return _parent; }
};

} // namespace hydro

#endif /* __h3o_Runtime__ */

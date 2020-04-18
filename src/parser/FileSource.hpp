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

#ifndef __h3o_FileSource__
#define __h3o_FileSource__

#include <fstream>

#include "Source.hpp"

namespace hydro
{

class FileSource : public Source
{
public:
	FileSource(std::string path);
	virtual ~FileSource();
	virtual char nextChar() override;
	void open();
	void close();
	bool isOpen() const;
	std::string filepath() const { return location(); } // alias
};

typedef FileSource *lex_fsource;

} // namespace hydro

#endif /* __h3o_FileSource__ */

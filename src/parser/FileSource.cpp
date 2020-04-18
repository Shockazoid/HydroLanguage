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

#include "FileSource.hpp"

namespace hydro
{

FileSource::FileSource(std::string path) : Source{*(new std::fstream{}), path} {}

FileSource::~FileSource() { close(); }

char FileSource::nextChar()
{
	char ch = Source::nextChar();
	if (eof())
		close();
	return ch;
}

void FileSource::open()
{
	std::fstream &file = *(std::fstream *)&_inputStream;
	file.open(location());
}

void FileSource::close()
{
	std::fstream &file = *(std::fstream *)&_inputStream;
	file.close();
}

bool FileSource::isOpen() const
{
	std::fstream &file = *(std::fstream *)&_inputStream;
	return file.is_open();
}

} // namespace hydro

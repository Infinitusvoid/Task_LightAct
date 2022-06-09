#pragma once

#include <string>

namespace Cosmos_file_IO
{
	//bool does file exit
	bool rename_file(const char* from,const char* to);
	bool rename_file_if_exist(const char* from, const char* to);

	bool file_exist(const char* name);

	void write_file_append(std::string& path, std::string& content);
	void write_file_overwrite(const std::string& path, std::string& content);
	bool read_file(const std::string& path, std::string& content);
}


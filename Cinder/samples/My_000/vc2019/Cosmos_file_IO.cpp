#include "Cosmos_file_IO.h"

#include <iostream>
#include <fstream>
#include<sstream>
#include <cstdio>
#include <filesystem>

namespace Cosmos_file_IO
{
	// -Cosmos_file_io
	void write_file_overwrite(const std::string& path, std::string& content)
	{
		std::ofstream out_file{ path };

		if (!out_file)
		{
			std::cerr << "Error creating file" << std::endl;
			return;
		}

		out_file << content << std::endl;
		out_file.close();
	}

	bool rename_file_if_exist(const char* from, const char* to)
	{
		if (std::filesystem::exists(from))
		{
			rename_file(from, to);
			return true;
		}
		return false;
	}

	bool file_exist(const char* name) {  return std::filesystem::exists(name); }

	bool rename_file(const char* from,const char* to)
	{
		if (std::rename(from, to) != 0)
		{
			//error
			return false;
		}
		else {
			return true;
		}

	}

	void write_file_append(std::string& path, std::string& content)
	{
		std::ofstream out_file{ path , std::ios::app };

		if (!out_file)
		{
			std::cerr << "Error creating file" << std::endl;
			return;
		}

		out_file << content << std::endl;
		out_file.close();
	}
	
	bool read_file(const std::string& path, std::string& content) {

		std::fstream  file;
		file.open(path, std::ios::in);

		if (!file)
		{
			//Can't read file
			return false;
		}

		std::ostringstream ss;
		ss << file.rdbuf(); // reading data
		content = ss.str();

		file.close();

		return true;
	}
	
}
#pragma once

#include <string>
#include <memory>


#include "blocks/json/rapidjson/writer.h"
#include "blocks/json/rapidjson/stringbuffer.h"

class Cosmos_JsonWriter
{
public:
	Cosmos_JsonWriter();

	void Key(const char* key);

	void make_string(const char* value);
	void make_int(const int& value);
	void make_double(const double& value);
	void make_bool(const bool& value);
	void make_null();
	void make_uint(const unsigned int& value);

	void make_start_object();
	void make_end_object();

	void make_start_array();
	void make_end_array();

	void make(const char* key, const char* value);
	void make(const char* key, const int& value);
	void make(const char* key, const double& value);
	void make(const char* key, const bool& value);
	void make(const char* key, const unsigned int& value);

	

	void generate_string(std::string& str);
private:

	std::unique_ptr<rapidjson::StringBuffer> string_buffer;
	std::unique_ptr<rapidjson::Writer<rapidjson::StringBuffer>> writer;

};


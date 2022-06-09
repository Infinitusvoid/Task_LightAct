#include "Cosmos_JsonWriter.h"

#include "blocks/json/rapidjson/rapidjson.h"
#include "blocks/json/rapidjson/document.h"

using namespace rapidjson;

Cosmos_JsonWriter::Cosmos_JsonWriter()
{
	string_buffer = std::make_unique<StringBuffer>();
	writer = std::make_unique<Writer<StringBuffer>>(*string_buffer);
}

void Cosmos_JsonWriter::Key(const char* key)
{
	writer->Key(key);
}

void Cosmos_JsonWriter::make_string(const char* value)
{
	writer->String(value);
}

void Cosmos_JsonWriter::make_int(const int& value)
{
	writer->Int(value);
}

void Cosmos_JsonWriter::make_double(const double& value)
{
	writer->Double(value);
}

void Cosmos_JsonWriter::make_bool(const bool& value)
{
	writer->Bool(value);
}

void Cosmos_JsonWriter::make_null()
{
	writer->Null();
}

void Cosmos_JsonWriter::make_uint(const unsigned int& value)
{
	writer->Uint(value);
}

void Cosmos_JsonWriter::make_start_object()
{
	writer->StartObject();
}

void Cosmos_JsonWriter::make_end_object()
{
	writer->EndObject();
}

void Cosmos_JsonWriter::make_start_array()
{
	writer->StartArray();
}

void Cosmos_JsonWriter::make_end_array()
{
	writer->EndArray();
}

void Cosmos_JsonWriter::make(const char* key, const char* value)
{
	Key(key); make_string(value);
}

void Cosmos_JsonWriter::make(const char* key, const int& value)
{
	Key(key); make_int(value);
}

void Cosmos_JsonWriter::make(const char* key, const double& value)
{
	Key(key); make_double(value);
}
void Cosmos_JsonWriter::make(const char* key, const bool& value)
{
	Key(key); make_bool(value);
}

void Cosmos_JsonWriter::make(const char* key, const unsigned int& value)
{
	Key(key); make_uint(value);
}

void Cosmos_JsonWriter::generate_string(std::string& str)
{
	str = string_buffer->GetString();
}
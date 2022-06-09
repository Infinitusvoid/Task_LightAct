#pragma once


#include "Cosmos_JsonWriter.h"
#include <vector>
#include "Data_Types.h"

#include "blocks/json/rapidjson/rapidjson.h"
#include "blocks/json/rapidjson/document.h"
#include "blocks/json/rapidjson/writer.h"
#include "blocks/json/rapidjson/stringbuffer.h"

#include <string>
#include <memory>


class ShapesManger
{
public:

	bool validate_selection();

	struct Selection
	{
		int id;
		int index;
	};

	ShapesManger();

	void move_last_up();
	void move_last_down();
	void move_last_rigth();
	void move_last_left();
	void delete_if_overlap(float x, float y);


	void add_squere(float x, float y);
	void add_rectangle(float x, float y);
	//CreateDefaultCircle() -> id
	//RemoveObjectId(id) 
	//CreateCircle(const CircleInfo& info);

	void walk_the_structure(Cosmos_JsonWriter& json_writer);

	void ShapesManger::create_from_json(std::string path);

	void unselect();

	Selection sel;

	std::vector<Data_Types::Image_info> info_images_list;
	std::vector<std::unique_ptr<Data_Types::Shape>> obj_list;
};


// -Cosmos_JsonRead
class Cosmos_JsonRead
{
public:
	Cosmos_JsonRead();
	bool load(const std::string& path);

	void Cosmos_JsonRead::construct_shapes_from_json(
		std::vector<std::unique_ptr<Data_Types::Shape>>& obj_list,
		std::vector<Data_Types::Image_info>& image_info_list
	);
	
private:
	bool parse(const std::string& json_string);
	rapidjson::Document document;

};

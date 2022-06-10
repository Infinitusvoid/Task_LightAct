#pragma once

#include "Data_Types.h"
#include "Cosmos_file_IO.h"
#include "Cosmos_Constants.h"
#include "ShapesManger.h"

class Serialization
{
	public:
		Serialization() {

		}
	
	Serialization& run(ShapesManger& manager)
	{
		json_writer = Cosmos_JsonWriter();

		json_writer.make_start_object();
		walk(manager.info_images_list, json_writer);
		walk_the_structure(json_writer, manager);//manager.walk_the_structure(e);
		json_writer.make_end_object();
		return *this;
	}

	void get_string(std::string& str)
	{
		json_writer.generate_string(str);
	}

	private:
		Cosmos_JsonWriter json_writer;

	void walk_the_structure(Cosmos_JsonWriter& json_writer, ShapesManger& manager)
	{
		json_writer.Key("data");
		json_writer.make_start_array();
		for (int i = 0; i < manager.obj_list.size(); i++)
		{
			json_writer.make_start_object();
			manager.obj_list[i]->serialize(json_writer);
			json_writer.make_end_object();
		}
		json_writer.make_end_array();
	}

	void walk(const std::vector<Data_Types::Image_info>& info_images, Cosmos_JsonWriter& json_writer)
	{
		namespace JC = Constants::Json_Constants::Img;

		json_writer.Key(JC::info_images);
		json_writer.make_start_array();

		for (auto info : info_images)
		{
			json_writer.make_start_object();

			json_writer.make(JC::path, info.path.c_str());
			json_writer.make(JC::enable_color, info.enable_color);
			json_writer.make(JC::mult_r, info.mult_r);
			json_writer.make(JC::mult_g, info.mult_g);
			json_writer.make(JC::mult_b, info.mult_b);

			json_writer.make_end_object();
		}

		json_writer.make_end_array();
	}

};


#pragma once


#include "Cosmos_JsonWriter.h"
#include <vector>
#include "Data_Types.h"
#include "Info.h"

#include "blocks/json/rapidjson/rapidjson.h"
#include "blocks/json/rapidjson/document.h"
#include "blocks/json/rapidjson/writer.h"
#include "blocks/json/rapidjson/stringbuffer.h"

#include <string>
#include <memory>

#include "Texture_controller.h"
#include "Image_Presenter.h"
#include "ShapesFactory.h"
#include "UI_List.h"

#include <stdio.h>


class Manager
{
	friend void write_JSON(Manager& manager, std::string& str);
	friend void read_JSON(Manager& manager, std::string path);
public:

	bool validate_selection();

	struct Selection
	{
		int id;
		int index;
	};

	Manager();

	void move_last_up();
	void move_last_down();
	void move_last_rigth();
	void move_last_left();
	void delete_if_overlap(float x, float y);

	void draw_UI_textures();
	void draw_UI_List();
	void draw_UI_Properties();
	void draw_textures(const ci::gl::GlslProgRef& shader_prog);
	void load_textures_from_file();

	void add_image(std::filesystem::path& file_path);

	void update(double dt, Data_Types::Update_Info& update_info);
	void draw();

	void create_randomized_circle(const float x, const float y);

	void add_squere(float x, float y);
	void add_rectangle(float x, float y);
	//CreateDefaultCircle() -> id
	//RemoveObjectId(id) 
	//CreateCircle(const CircleInfo& info);

	//void walk_the_structure(Cosmos_JsonWriter& json_writer);

	//void create_from_json(std::string path);

	void unselect();



private:
	Selection sel;
	std::vector<std::unique_ptr<Data_Types::Shape>> obj_list;
	std::vector<Info::Image_info> info_images_list;
	std::unique_ptr<Image_Presenter> image_presenter;
	std::unique_ptr<Texture_controller> tex_controller;
	
	std::unique_ptr<UI_List> ui_list;
};


// -Cosmos_JsonRead
class Cosmos_JsonRead
{
public:
	Cosmos_JsonRead();
	bool load(const std::string& path);

	void Cosmos_JsonRead::construct_shapes_from_json(
		std::vector<std::unique_ptr<Data_Types::Shape>>& obj_list,
		std::vector<Info::Image_info>& image_info_list
	);
	
private:
	bool parse(const std::string& json_string);
	rapidjson::Document document;

};

//Write_JSON

void write_JSON(Manager& manager, std::string& str);
void read_JSON(Manager& manager, std::string path);
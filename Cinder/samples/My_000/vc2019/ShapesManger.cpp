#include "ShapesManger.h"
#include "Cosmos_file_IO.h"
#include "Cosmos_Constants.h"

using namespace rapidjson;


bool ShapesManger::validate_selection()
{
	//selection { index, id }
	//if at the index there is not the right id
	//-> true :  we search thrue all, if there is none
	//  -> true  : selection turns into unselected
	//  -> false : we found it so we update the index in selection and display it
	//-> false : we just use the id
	if (sel.index < 0 || obj_list.size() == 0)
	{
		sel.id = -1;
		sel.index = -1;
		return false;
	}

	bool search = false;
	if (sel.index > obj_list.size() - 1)
	{
		search = true;
	}
	else
	{
		if (obj_list[sel.index]->id != sel.id)
		{
			search = true;
		}
	}

	if (search)
	{
		for (int i = 0; i < obj_list.size(); i++)
		{
			if (obj_list[i]->id == sel.id)
			{
				sel.index = i;
				return true;
			}
		}
		sel.index = -1;
		sel.id = -1;
		return false;
	}


	return true;
}
// ShapesManger
ShapesManger::ShapesManger()
{
	//selected = -1;

	sel.id = -1;
	sel.index = -1;
}

void ShapesManger::move_last_up()
{
	if (obj_list.size() > 0)
	{
		obj_list[obj_list.size() - 1]->y -= 10;
	}
}

void ShapesManger::move_last_down()
{
	if (obj_list.size() > 0)
	{
		obj_list[obj_list.size() - 1]->y += 10;
	}
}

void ShapesManger::move_last_rigth()
{
	if (obj_list.size() > 0)
	{
		obj_list[obj_list.size() - 1]->x += 10;
	}
}

void ShapesManger::move_last_left()
{
	if (obj_list.size() > 0)
	{
		obj_list[obj_list.size() - 1]->x -= 10;
	}
}

void ShapesManger::delete_if_overlap(float x, float y)
{
	//delete all that overlap
	if (false)
	{
		obj_list.erase(
			std::remove_if(
				obj_list.begin(),
				obj_list.end(),
				[&x, &y](std::unique_ptr<Data_Types::Shape>& ptr) { return ptr->is_clicked(x, y); }),
			obj_list.end()
		);
	}

	//TODO delete the topmost only (last added higher index)

	//We get the list of overlaping

	int index_to_delete = -1;
	for (int i = 0; i < obj_list.size(); i++)
	{
		if (obj_list[i]->is_clicked(x, y))
		{
			if (i > index_to_delete)
			{
				index_to_delete = i;
			}
		}
	}

	if (index_to_delete != -1)
	{
		if (sel.index == index_to_delete) //We have deleted the selected
		{
			sel.index = -1;
			sel.id = -1;
		}

		obj_list.erase(obj_list.begin() + index_to_delete);
	}
}

void ShapesManger::add_squere(float x, float y)
{
	std::unique_ptr<Data_Types::Square> square = std::make_unique<Data_Types::Square>();
	square->shape_type = Data_Types::Shape::ShapeType::Squere;
	square->x = x;
	square->y = y;
	square->r = 0.1f;
	square->g = 0.8f;
	square->b = 0.2f;
	square->rotation = 0.0f;
	square->rogue = false;
	square->size = 25;

	obj_list.push_back(std::move(square));
}

void ShapesManger::add_rectangle(float x, float y)
{
	std::unique_ptr<Data_Types::Rectangle> rectangle = std::make_unique<Data_Types::Rectangle>();
	rectangle->shape_type = Data_Types::Shape::ShapeType::Squere;
	rectangle->x = x;
	rectangle->y = y;
	rectangle->r = 0.01f;
	rectangle->g = 0.8f;
	rectangle->b = 0.2f;
	rectangle->rotation = 0.0f;
	rectangle->rogue = false;
	rectangle->width = 125;
	rectangle->height = 25;

	obj_list.push_back(std::move(rectangle));
	
}

//void ShapesManger::walk_the_structure(Cosmos_JsonWriter& json_writer)
//{
//	
//	json_writer.Key("data");
//	json_writer.make_start_array();
//	for (int i = 0; i < obj_list.size(); i++)
//	{
//		//json_writer.Key(std::to_string(i).c_str());
//		json_writer.make_start_object();
//		obj_list[i]->serialize(json_writer);
//		json_writer.make_end_object();
//	}
//	json_writer.make_end_array();
//}

void ShapesManger::create_from_json(std::string path)
{
	Cosmos_JsonRead json_read;
	if (!json_read.load(path)) { //unable to read
		return;
	}
	
	//Data_Types::ShapesBuilder builder;

	//overide previsus
	std::vector<std::unique_ptr<Data_Types::Shape>> obj_list_tmp;
	json_read.construct_shapes_from_json(obj_list_tmp, info_images_list);
	obj_list = std::move(obj_list_tmp);
	
}

void ShapesManger::unselect()
{
	sel.id = -1;
	sel.index = -1;
}

// -Cosmos_JsonRead
Cosmos_JsonRead::Cosmos_JsonRead()
{
}



void Cosmos_JsonRead::construct_shapes_from_json( 
	std::vector<std::unique_ptr<Data_Types::Shape>>& obj_list,
	std::vector<Data_Types::Image_info>& image_info_list
)
{
	namespace JC = Constants::Json_Constants;

	// -data geometric shapes
	{
		
		const Value& a = document[JC::data];//"data"];
		assert(a.IsArray());

		for (SizeType i = 0; i < a.Size(); i++)
		{
			int type = a[i][JC::type].GetInt();

			if (type == 0) //circle
			{
				std::unique_ptr<Data_Types::Circle> circle = std::make_unique<Data_Types::Circle>();

				//circle unique
				circle->radius = a[i][JC::radius].GetDouble();
				circle->direction.x = a[i][JC::direction_x].GetDouble();
				circle->direction.y = a[i][JC::direction_y].GetDouble();

				//common 
				circle->id = a[i][JC::id].GetInt();
				circle->x = a[i][JC::coordinate_x].GetDouble();
				circle->y = a[i][JC::coordinate_y].GetDouble();
				circle->r = a[i][JC::color_r].GetDouble();
				circle->g = a[i][JC::color_g].GetDouble();
				circle->b = a[i][JC::color_b].GetDouble();
				circle->rogue = a[i][JC::rogue].GetBool();
				circle->rotation = a[i][JC::rotation].GetDouble();

				obj_list.push_back(std::move(circle));
			}
			else if (type == 1) //squere
			{
				std::unique_ptr<Data_Types::Square> square = std::make_unique<Data_Types::Square>();

				//square unique
				square->size = a[i][JC::size].GetDouble();

				//common

				square->id = a[i][JC::id].GetInt();
				square->x = a[i][JC::coordinate_x].GetDouble();
				square->y = a[i][JC::coordinate_y].GetDouble();
				square->r = a[i][JC::color_r].GetDouble();
				square->g = a[i][JC::color_g].GetDouble();
				square->b = a[i][JC::color_b].GetDouble();
				square->rogue = a[i][JC::rogue].GetBool();
				square->rotation = a[i][JC::rotation].GetDouble();

				obj_list.push_back(std::move(square));
			}
			else if (type == 2) //rectangle
			{
				std::unique_ptr<Data_Types::Rectangle> rectangle = std::make_unique<Data_Types::Rectangle>();

				//square unique
				rectangle->width = a[i][JC::width].GetDouble();
				rectangle->height = a[i][JC::height].GetDouble();

				//common
				rectangle->id = a[i][JC::id].GetInt();
				rectangle->x = a[i][JC::coordinate_x].GetDouble();
				rectangle->y = a[i][JC::coordinate_y].GetDouble();
				rectangle->r = a[i][JC::color_r].GetDouble();
				rectangle->g = a[i][JC::color_g].GetDouble();
				rectangle->b = a[i][JC::color_b].GetDouble();
				rectangle->rogue = a[i][JC::rogue].GetBool();
				rectangle->rotation = a[i][JC::rotation].GetDouble();

				obj_list.push_back(std::move(rectangle));
			}
		}
	}


	// -data info_images
	{
		const Value& a = document[JC::Img::info_images];
		assert(a.IsArray());

		image_info_list.clear();

		for (SizeType i = 0; i < a.Size(); i++)
		{
			std::string path = a[i][JC::Img::path].GetString();
			bool enable_color = a[i][JC::Img::enable_color].GetBool();
			float mult_r = a[i][JC::Img::mult_r].GetDouble();
			float mult_g = a[i][JC::Img::mult_g].GetDouble();
			float mult_b = a[i][JC::Img::mult_b].GetDouble();
			
			image_info_list.push_back(Data_Types::Image_info(path, mult_r, mult_g, mult_b, enable_color));
		}
	}

	
}

bool Cosmos_JsonRead::load(const std::string& path)
{
	std::string str;
	if (Cosmos_file_IO::read_file(path, str))
	{
		std::string json_string = str;
		if (parse(json_string))
		{
			return true;
		}
		return false;
	}
	return false;
}


bool Cosmos_JsonRead::parse(const std::string& json_string)
{
	const char* json = json_string.c_str();

	document = Document();
	document.Parse(json);

	return true;
}


#include "UI_List.h"

#include "cinder/Shape2d.h"
#include "Random.h"

// ImGui
#include "cinder/CinderImGui.h"

void UI_List::draw_UI_List(std::vector<std::unique_ptr<Data_Types::Shape>>& obj_list, int& sel_index, int& sel_id)
{
	ImGui::Begin("Window : List");
	glm::vec2 window_size{ 400, 500 };
	ImGui::SetWindowSize(window_size);
	glm::vec2 window_pos{ 50, 50 };
	ImGui::SetWindowPos(window_pos);

	float window_heigth = ImGui::GetWindowSize().y;

	int rows = 27;
	if (window_heigth > 10)
	{
		rows = static_cast<int>(((window_heigth / (float)500) * 27.0f));
	}


	std::vector<const char*> items;
	std::vector<std::string> vec;

	//constructing items
	{

		for (int i = 0; i < obj_list.size(); i++) //TODO : you should not be doing this every frame manager should do this when adding new object
		{
			vec.push_back("object " + std::to_string(i) + " id : " + std::to_string(obj_list[i]->id));
		}

		for (int i = 0; i < vec.size(); i++) {
			items.push_back(&*vec[i].begin());
		}
	}


	if (obj_list.size() > 0)
	{
		//ImGui::ListBox("ListBox", &manager.sel.index, &items[0], items.size());
		ImGui::ListBox("Object List", &sel_index, &items[0], items.size(), rows);//27);
	}

	if (sel_index >= 0 && sel_index < obj_list.size())
	{
		sel_id = obj_list[sel_index]->id;
	}

	ImGui::End();
}

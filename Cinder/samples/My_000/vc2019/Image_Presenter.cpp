#include "Image_Presenter.h"

// ImGui
#include "cinder/CinderImGui.h"

Image_Presenter::Image_Presenter()
{
	first_time = true;
	selected = -1;
	edit = false;
}
Image_Presenter::~Image_Presenter()
{
}

void Image_Presenter::draw(std::vector<ci::gl::Texture2dRef>& images, std::vector<Info::Image_info>& infos)
{
	ImGui::Begin("Harmonica");

	// First time only
	if (first_time)
	{
		first_time = false;
		glm::vec2 window_size{ 500, 500 };
		ImGui::SetWindowSize(window_size);
		glm::vec2 window_pos{ 100, 50 };
		ImGui::SetWindowPos(window_pos);
	}

	if (images.size() != infos.size())
	{
		return;
	}

	int num_images = images.size();

	for (int i = 0; i < num_images; i++)
	{
		ImGui::Spacing();
		ci::gl::Texture2dRef& el = images[i];
		ImGui::Image(el, glm::vec2(50, 50));

		ImGui::Text(infos[i].path.c_str());
		std::string str = std::to_string(i);

		if (i != selected)
		{
			if (ImGui::Button(("Edit : " + str).c_str()))
			{
				selected = i;
				edit = true;
			}
		}

		if (selected != -1 && i == selected)
		{
			if (selected != -1)
			{
				if (ImGui::Button("Close"))
				{
					selected = -1;
					continue;
				}
			}

			ImGui::Checkbox("enable color", &infos[selected].enable_color);
			ImGui::SliderFloat("r", &infos[selected].mult_r, 0, 10, "%.3f");
			ImGui::SliderFloat("g", &infos[selected].mult_g, 0, 10, "%.3f");
			ImGui::SliderFloat("b", &infos[selected].mult_b, 0, 10, "%.3f");


			ImGui::Spacing();
			ImGui::Spacing();
			ImGui::Spacing();
		}
	}


	ImGui::End();
}

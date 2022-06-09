#pragma once

#include <string>

// -ImGui
#include "cinder/CinderImGui.h"
#include "cinder/gl/gl.h"

#include <map>

#include <sstream> //for std::stringstream 
#include <string>  //for std::string


class Custom_Control
{
public:
	float size;
	bool animate;
	bool animation_forward;
	float t;
	std::string name_id;

	Custom_Control()
	{
	/*	size = 100.0f;
		animate = false;
		animation_forward = false;
		t = 0;
		name_id = "GRRRRR";*/
	}

	bool isOn()
	{
		return animation_forward;
	}

	bool isOff()
	{
		return !animation_forward;
	}

	Custom_Control(float size, std::string name, bool value) : size{ size }
	{
		animate = false;
		animation_forward = value;// true;
		t = 0.0f;
		
		
		t = animation_forward ? 0.0f : 1.0f;
		


		name_id = name;
	}

	
	float lerp(float a, float b, float t)
	{
		return a + t * (b - a);
	}

	int lerp(int a, int b, float t)
	{
		return (int)(a + t * (float)(b - a));
	}
	
	/*float lerpClamped(int a, int b, float t)
	{
		t = fmaxf(fminf(t, 0.0f), 0.0f);
		return a + t * (float)(b - a);
	}*/


	template<typename T>
	T lerpClamp(T a, T b, float t)
	{
		t = fmaxf(fminf(t, 0.0f), 0.0f);
		if (std::is_same<T, float>::value)
		{
			return a + t * (b - a);
		}
		return static_cast<T>(a + t * (float)(b - a));
	}

	Custom_Control& draw() {

		//if (ImGui::Button(name_id.c_str(), ImVec2(size, size)))
		if (ImGui::InvisibleButton(name_id.c_str(), ImVec2(size, size)))
		{
			animate = !animate;
		}

		animation_processing();

		ImDrawList* draw_list = ImGui::GetWindowDrawList();

		// Get the current ImGui cursor position
		ImVec2 p = ImGui::GetCursorScreenPos();

		float max_circle_radius = size * 0.5f;

		float radius = max_circle_radius * 0.5f + max_circle_radius * 0.25f * t;
		
		
		float px = lerp(p.x + max_circle_radius * 0.5f, p.x + max_circle_radius * 1.5f, t);

		float fak = std::fmaxf(fminf(t, 1.0f), 0.0f);
		int r = lerp(0, 255, fak);
		int g = lerp(128, 140, fak);
		int b = lerp(0, 0, fak);
		
		
		//std::sinf(t);
		//draw_list->AddCircleFilled(ImVec2(p.x + size, p.y), size, IM_COL32(10, 255, 15, 255), 16);
		draw_list->AddCircle(ImVec2(p.x + max_circle_radius, p.y - max_circle_radius), max_circle_radius, IM_COL32(r, g, b, 255), 16);

		

		//draw_list->AddCircle(ImVec2(p.x + max_circle_radius, p.y - max_circle_radius), radius, IM_COL32(10, 255, 15, 255), 16);

		
		

		draw_list->AddCircle(ImVec2(px, p.y - max_circle_radius), max_circle_radius * 0.5f, IM_COL32(r, g, b, 255), 16);

		if (animation_forward)
		{
			draw_list->AddText(ImVec2(px - 15, p.y - max_circle_radius - 5), IM_COL32(10, 255, 15, 255), "View");
		}
		else
		{
			draw_list->AddText(ImVec2(px - 15, p.y - max_circle_radius - 5), IM_COL32(10, 255, 15, 255), "Edit");
		}

		//draw_list->AddCircle(ImVec2(px, p.y - max_circle_radius), radius, IM_COL32(10, 255, 15, 255), 16);

		//ImGui::Dummy(ImVec2(size, size));

		//ImGui::Dummy(ImVec2(max_circle_radius, max_circle_radius * 0.0f));
		return *this;
	}

private:
	void animation_processing()
	{
		if (animate)
		{
			if (animation_forward)
			{
				t += 0.10f;
			}
			else {
				t -= 0.10f;
			}

			if (t >= 1.0f) {
				animate = false;
				animation_forward = false;
			}
			else if (t <= 0.0f) {
				animate = false;
				animation_forward = true;
			}

			t = std::fminf(std::fmaxf(0.0f, t), 1.0f);
		}
	}

};


class UI_Controller
{
public:
	UI_Controller(const UI_Controller&) = delete;

	static UI_Controller& Get() {
		static UI_Controller instance;
		return instance;
	}

	

	Custom_Control& add_button(float size, std::string name, bool init_value) {

		

		if (elements.find(name) != elements.end())
		{
			return elements[name];
		}


		elements.insert(std::make_pair(name, Custom_Control(size, name, init_value)));
		return elements[name];
	}

	void Show_Locked()
	{
		if (img_texture_locked)
		{
			//ImGui::Image(*img_texture_locked, ImVec2(my_image_width, my_image_height));
			glm::vec2 size = glm::vec2(100, 100);
			ImGui::Image(img_texture_locked, size);

		}
	}

	void Show_Unlocked()
	{
		if (img_texture_unlocked)
		{
			//ImGui::Image(*img_texture_locked, ImVec2(my_image_width, my_image_height));
			glm::vec2 size = glm::vec2(100, 100);
			ImGui::Image(img_texture_unlocked, size);

		}
	}


private:
	std::map<std::string, Custom_Control> elements;
	
	cinder::gl::Texture2dRef img_texture_locked;
	cinder::gl::Texture2dRef img_texture_unlocked;

	UI_Controller() {
		

		try {
			cinder::gl::Texture2dRef texture = cinder::gl::Texture::create(cinder::loadImage(cinder::loadFile("locked.jpeg")));
			img_texture_locked = texture;
		}
		catch (cinder::Exception& exc) {
			//CI_LOG_EXCEPTION("failed to load image: " << event.getFile(0), exc);
		}


		try {
			img_texture_unlocked = cinder::gl::Texture::create(cinder::loadImage(cinder::loadFile("unlocked.jpeg")));
		}
		catch (cinder::Exception& exc) {
			//CI_LOG_EXCEPTION("failed to load image: " << event.getFile(0), exc);
		}
	
	}
};


#include "Data_Types.h"

#include "cinder/gl/gl.h"
#include "cinder/CinderImGui.h"

#include "UI_Controller.h"
#include "Cosmos_Constants.h"

namespace Data_Types
{
	// -Circle
	Circle::Circle() : Shape{}
	{
		shape_type = shape_type = Shape::ShapeType::Circle;
	}

	Circle::~Circle() {}
	
	void Circle::draw(ci::Shape2d& shape2d)
	{
		ci::gl::color(r, g, b);

		glm::vec2 center = glm::vec2(x, y);
		shape2d.arc(center, radius, 0.0, glm::two_pi<float>());
	}

	bool Circle::is_clicked(float ix, float iy) const
	{
		float xd = (ix - x);
		xd *= xd;
		float yd = (iy - y);
		yd *= yd;

		return (xd + yd) < (radius * radius);
	}

	void Circle::update(float dt, Update_Info& info)
	{
		x += direction.x * dt;
		y += direction.y * dt;

		info.guard(radius, x, y, direction.x, direction.y);
	}

	void Circle::display_UI()
	{
		ImGui::Text(Constants::UI::type_circle);
		rogue = UI_Controller::Get().add_button(100, "circle_rogule" + std::to_string(this->id), !rogue).draw().isOff();
		//ImGui::Checkbox("Rogue", &rogue);
		

		draw_shared_UI();
		
		ImGuiInputTextFlags_ flags = rogue ? ImGuiInputTextFlags_None : ImGuiInputTextFlags_ReadOnly;

		ImGui::InputFloat(Constants::UI::radius, &radius, 0.01f, 1.0f, Constants::UI::precision, flags);
			
		float  dir[2] = { direction.x, direction.y };
	
		ImGui::InputFloat2(Constants::UI::direction, dir, 3, flags);//InputFloat2("direction", &direction, 3);

		direction.x = dir[0];
		direction.y = dir[1];
		
		
	}

	void Circle::serialize(Cosmos_JsonWriter& json_writer) //TODO : Rename Serialize and all others
	{
		using namespace Constants;

		json_writer.make(Json_Constants::type, Json_Constants::type_circle);
		
		serialize_shared(json_writer);

		json_writer.make(Json_Constants::direction_x, direction.x);
		json_writer.make(Json_Constants::direction_y, direction.y);
		json_writer.make(Json_Constants::radius, radius);
	}
	
	// -Square
	
	Square::Square() : size{0}, Shape {}
	{
		shape_type = Shape::ShapeType::Squere;
	}

	Square::~Square() {}

	void Square::draw(ci::Shape2d& shape2d)
	{
		ci::Rectf rect;
		rect.set(-size, -size, size, size);

		ci::mat3 matrix;
		
		// preserve the Model matrix
		ci::gl::pushModelMatrix();

		ci::gl::translate(glm::vec2(x, y));
		ci::gl::rotate(this->rotation);
		
		ci::gl::color(r, g, b);
		ci::gl::drawStrokedRect(rect);

		// restore the Model matrix
		ci::gl::popModelMatrix();
	}

	bool Square::is_clicked(float ix, float iy) const
	{
		//TODO : this is the formula for circle i need one for squere
		float xd = (ix - x);
		xd *= xd;
		float yd = (iy - y);
		yd *= yd;

		if ((xd + yd) < (size * size))//(radius * radius))
		{
			return true;
		}

		return false;
	}

	void Square::update(float dt, Update_Info& info) {}

	void Square::display_UI()
	{
		ImGui::Text(Constants::UI::type_square);
		rogue = UI_Controller::Get().add_button(100, "circle_rogule" + std::to_string(this->id), !rogue).draw().isOff();
		//ImGui::Checkbox("Rogue", &rogue);

		draw_shared_UI();

		ImGuiInputTextFlags_ flags = rogue ? ImGuiInputTextFlags_None : ImGuiInputTextFlags_ReadOnly;
		ImGui::InputFloat("size", &size, 0.01f, 1.0f, Constants::UI::precision, flags);
	}

	void Square::serialize(Cosmos_JsonWriter& json_writer)
	{
		using namespace Constants;
		json_writer.Key(Json_Constants::type);
		json_writer.make_int(Json_Constants::type_square);

		serialize_shared(json_writer);

		json_writer.Key(Json_Constants::size);
		json_writer.make_double(size);

	}

	// -Rectangle

	Rectangle::Rectangle() : 
		Shape{ 
		0.0f,
		Shape::ShapeType::Rectangle,
		0.0f,
		0.0f,
		0.0f,
		0.0f,
		0.0f
	},
		width { 0.0f },
		height{ 0.0f }
	{
		
	}

	Rectangle::~Rectangle() {}

	void Rectangle::draw(ci::Shape2d& shape2d)
	{
		ci::Rectf rect;
		rect.set(-width, -height, width, height);

		ci::mat3 matrix;
	
		// preserve the Model matrix
		ci::gl::pushModelMatrix();

		ci::gl::translate(glm::vec2(x, y));
		ci::gl::rotate(this->rotation);

		ci::gl::color(r, g, b);
		ci::gl::drawStrokedRect(rect);

		// restore the Model matrix
		ci::gl::popModelMatrix();

	}

	bool Rectangle::is_clicked(float ix, float iy) const
	{
		//TODO : this is the formula for circle i need one for rectangle
		float xd = (ix - x);
		xd *= xd;
		float yd = (iy - y);
		yd *= yd;

		if ((xd + yd) < (width * height))//(radius * radius))
		{
			return true;
		}

		return false;
	}

	void Rectangle::update(float dt, Update_Info& info) { }

	void Rectangle::display_UI()
	{
		ImGui::Text(Constants::UI::type_rectangle);
		
		rogue = UI_Controller::Get().add_button(100, "circle_rogule" + std::to_string(this->id), !rogue).draw().isOff();
		//ImGui::Checkbox("Rogue", &rogue);

		draw_shared_UI();
		
		ImGuiInputTextFlags_ flags = rogue ? ImGuiInputTextFlags_None : ImGuiInputTextFlags_ReadOnly;
		ImGui::InputFloat("w", &width, 0.01f, 1.0f, Constants::UI::precision, flags);
		ImGui::InputFloat("h", &height, 0.01f, 1.0f, Constants::UI::precision, flags);
	}

	void Rectangle::serialize(Cosmos_JsonWriter& json_writer)
	{
		using namespace Constants;

		json_writer.Key(Json_Constants::type);
		json_writer.make_int(Json_Constants::type_rectangle);

		serialize_shared(json_writer);

		json_writer.Key(Json_Constants::width);
		json_writer.make_double(width);

		json_writer.Key(Json_Constants::height);
		json_writer.make_double(height);
	}

	void Image_info::display_ui(int i, int k, std::function<void(int)> f)
	{
		ImGui::Text(path.c_str());
		std::string str = std::to_string(i);

		if (i == k)
		{
			return;
		}

		if (ImGui::Button(("Edit : " + str).c_str()))
		{
			f(i);
		}
	}

	Image_info::Image_info()
	{
	}

	Image_info::Image_info(std::string path, float mult_r, float mult_g, float mult_b, bool enable_color) :
		path{ path },
		mult_r{ mult_r },
		mult_g{ mult_g },
		mult_b{ mult_b },
		enable_color{ enable_color }
	{

	}

	Shape::Shape()
	{
		rogue = false;
		id = Id_generator::next_id();
		rotation = 0.0f;

		shape_type = ShapeType::Abstract;
		x = 0;
		y = 0;
		r = 0;
		g = 0;
		b = 0;
	}
	Shape::Shape(float rotation, ShapeType shape_type, float x, float y, float r, float g, float b) :
		rogue{ false },
		id{ Id_generator::next_id() },
		rotation{ rotation },
		shape_type{ shape_type },
		x{ x },
		y{ y },
		r{ r },
		g{ g },
		b{ b }
	{

	}
	Shape::~Shape()
	{

	}
	void Shape::draw_shared_UI()
	{
		
		

		ImGuiInputTextFlags_ flags = rogue ? ImGuiInputTextFlags_None : ImGuiInputTextFlags_ReadOnly;

		ImGui::Text("--------");

		ImGui::Text("id : ");
		ImGui::SameLine();
		ImGui::Text(std::to_string(id).c_str());

		if (flags == ImGuiInputTextFlags_None)
		{
			ImGui::SliderAngle("rotation", &rotation, -360.0f, 360.0f);
		}
		else
		{
			ImGui::InputFloat("rotation", &rotation, 0.01f, 1.0f, Constants::UI::precision, flags);
		}
		
		

		ImGui::InputFloat("x", &x, 0.01f, 1.0f, Constants::UI::precision, flags);
		ImGui::InputFloat("y", &y, 0.01f, 1.0f, Constants::UI::precision, flags);


		
		
		if (flags == ImGuiInputTextFlags_None)
		{
			cinder::Colorf color{ r, g, b };
			const char* name = "color";
			ImGui::ColorPicker3(name, &color);//ImGuiColorEditFlags_NoPicker);//ImGuiColorEditFlags_DisplayRGB);//ImGuiColorEditFlags_None);
			r = color.r;
			g = color.g;
			b = color.b;
		}
		else {
			ImVec4 color2 = ImVec4(r, g, b, 1.0);
			ImGui::ColorButton("color_button", color2);
		}

		//("color", color, flags);//("picker", wtf);//("Picker",)
		//ImGui::InputFloat("r", &r, 0.01f, 1.0f, Constants::UI::precision, flags);
		//ImGui::InputFloat("g", &g, 0.01f, 1.0f, Constants::UI::precision, flags);
		//ImGui::InputFloat("b", &b, 0.01f, 1.0f, Constants::UI::precision, flags);
	}

	

	void Shape::serialize_shared(Cosmos_JsonWriter& json_writer)
	{
		namespace JC = Constants::Json_Constants;
		
		json_writer.make(JC::id, id);
		json_writer.make(JC::rotation, rotation);
		json_writer.make(JC::coordinate_x, x);
		json_writer.make(JC::coordinate_y, y);
		json_writer.make(JC::color_r, r);
		json_writer.make(JC::color_g, g);
		json_writer.make(JC::color_b, b);
		json_writer.make(JC::rogue, rogue);
	}

	void Update_Info::guard(const float radius, float& x, float& y, float& dx, float& dy)
	{
		//bouncing
		if (x - radius < 0 || x + radius > bound_x) { dx *= -1; }
		if (y - radius < 0 || y + radius > bound_y) { dy *= -1; }

		//taking care of window resize
		if (x - radius < 0) {
			x = radius;
		}
		else if (x > bound_x - radius)
		{
			x = bound_x - radius;
		}

		if (y - radius < 0)
		{
			y = radius;
		}
		else if (y > bound_y - radius)
		{
			y = bound_y - radius;
		}
	}
}

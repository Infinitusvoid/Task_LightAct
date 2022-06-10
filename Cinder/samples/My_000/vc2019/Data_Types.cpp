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

	void Circle::draw(ci::Shape2d& shape2d) { Draw(*this, shape2d); }

	//Draw::draw(*this, shape2d); }
	bool Circle::is_clicked(float ix, float iy) const { return Is_Clicked::check(*this, ix, iy); }
	void Circle::update(float dt, Update_Info& info)
	{ 
		x += direction.x * dt;
		y += direction.y * dt;

		info.guard(radius, x, y, direction.x, direction.y);
	}
	void Circle::display_UI() { Display_UI(*this); }
	void Circle::serialize(Cosmos_JsonWriter& json_writer) { Serialize(json_writer, *this); }

	// -Square
	Square::Square() : size{ 0 }, Shape{}
	{
		shape_type = Shape::ShapeType::Squere;
	}

	Square::~Square() {}

	void Square::draw(ci::Shape2d& shape2d) { Draw(*this, shape2d); }
	bool Square::is_clicked(float ix, float iy) const { return Is_Clicked::check(*this, ix, iy); }
	void Square::update(float dt, Update_Info& info) {}
	void Square::display_UI() { Display_UI(*this); }
	void Square::serialize(Cosmos_JsonWriter& json_writer) { Serialize(json_writer, *this); }

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
		width{ 0.0f },
		height{ 0.0f }
	{

	}

	Rectangle::~Rectangle() {}

	void Rectangle::draw(ci::Shape2d& shape2d) { Draw(*this, shape2d); }
	bool Rectangle::is_clicked(float ix, float iy) const { return Is_Clicked::check(*this, ix, iy); }
	void Rectangle::update(float dt, Update_Info& info) { }
	void Rectangle::display_UI() { Display_UI(*this); }
	void Rectangle::serialize(Cosmos_JsonWriter& json_writer) { Serialize(json_writer, *this); }
	void Image_info::display_ui(int i, int k, std::function<void(int)> f) { Display_UI(*this, i, k, f); }

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
	Shape::~Shape() { }


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

	// -Draw
	Draw::Draw(Circle& circle, ci::Shape2d& shape2d)
	{
		ci::gl::color(circle.r, circle.g, circle.b);

		glm::vec2 center = glm::vec2(circle.x, circle.y);
		shape2d.arc(center, circle.radius, 0.0, glm::two_pi<float>());
	}

	Draw::Draw(Square& square, ci::Shape2d& shape2d)
	{
		ci::Rectf rect;
		rect.set(-square.size, -square.size, square.size, square.size);

		ci::mat3 matrix;

		// preserve the Model matrix
		ci::gl::pushModelMatrix();

		ci::gl::translate(glm::vec2(square.x, square.y));
		ci::gl::rotate(square.rotation);

		ci::gl::color(square.r, square.g, square.b);
		ci::gl::drawStrokedRect(rect);

		// restore the Model matrix
		ci::gl::popModelMatrix();
	}

	Draw::Draw(Rectangle& rectangle, ci::Shape2d& shape2d)
	{
		ci::Rectf rect;
		rect.set(-rectangle.width, -rectangle.height, rectangle.width, rectangle.height);

		ci::mat3 matrix;

		// preserve the Model matrix
		ci::gl::pushModelMatrix();

		ci::gl::translate(glm::vec2(rectangle.x, rectangle.y));
		ci::gl::rotate(rectangle.rotation);

		ci::gl::color(rectangle.r, rectangle.g, rectangle.b);
		ci::gl::drawStrokedRect(rect);

		// restore the Model matrix
		ci::gl::popModelMatrix();
	}

	// -Display_UI
	Display_UI::Display_UI(Circle& circle)
	{
		ImGui::Text(Constants::UI::type_circle);
		circle.rogue = UI_Controller::Get().add_button(100, "circle_rogule" + std::to_string(circle.id), !circle.rogue).draw().isOff();
		//ImGui::Checkbox("Rogue", &rogue);


		display_UI_Shape_Shared(circle);

		ImGuiInputTextFlags_ flags = circle.rogue ? ImGuiInputTextFlags_None : ImGuiInputTextFlags_ReadOnly;

		ImGui::InputFloat(Constants::UI::radius, &circle.radius, 0.01f, 1.0f, Constants::UI::precision, flags);

		float  dir[2] = { circle.direction.x, circle.direction.y };

		ImGui::InputFloat2(Constants::UI::direction, dir, 3, flags);//InputFloat2("direction", &direction, 3);

		circle.direction.x = dir[0];
		circle.direction.y = dir[1];
	}

	Display_UI::Display_UI(Square& square)
	{
		ImGui::Text(Constants::UI::type_square);
		square.rogue = UI_Controller::Get().add_button(100, "circle_rogule" + std::to_string(square.id), !square.rogue).draw().isOff();

		display_UI_Shape_Shared(square);

		ImGuiInputTextFlags_ flags = square.rogue ? ImGuiInputTextFlags_None : ImGuiInputTextFlags_ReadOnly;
		ImGui::InputFloat("size", &square.size, 0.01f, 1.0f, Constants::UI::precision, flags);
	}

	Display_UI::Display_UI(Rectangle& rect)
	{
		ImGui::Text(Constants::UI::type_rectangle);

		rect.rogue = UI_Controller::Get().add_button(100, "circle_rogule" + std::to_string(rect.id), !rect.rogue).draw().isOff();

		display_UI_Shape_Shared(rect);

		ImGuiInputTextFlags_ flags = rect.rogue ? ImGuiInputTextFlags_None : ImGuiInputTextFlags_ReadOnly;
		ImGui::InputFloat("w", &rect.width, 0.01f, 1.0f, Constants::UI::precision, flags);
		ImGui::InputFloat("h", &rect.height, 0.01f, 1.0f, Constants::UI::precision, flags);
	}

	Display_UI::Display_UI(Image_info& image_info, int i, int k, std::function<void(int)> f)
	{
		ImGui::Text(image_info.path.c_str());
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

	void Display_UI::display_UI_Shape_Shared(Shape& shape)
	{
		ImGuiInputTextFlags_ flags = shape.rogue ? ImGuiInputTextFlags_None : ImGuiInputTextFlags_ReadOnly;

		ImGui::Text("--------");

		ImGui::Text("id : ");
		ImGui::SameLine();
		ImGui::Text(std::to_string(shape.id).c_str());

		if (flags == ImGuiInputTextFlags_None)
		{
			ImGui::SliderAngle("rotation", &shape.rotation, -360.0f, 360.0f);
		}
		else
		{
			ImGui::InputFloat("rotation", &shape.rotation, 0.01f, 1.0f, Constants::UI::precision, flags);
		}

		ImGui::InputFloat("x", &shape.x, 0.01f, 1.0f, Constants::UI::precision, flags);
		ImGui::InputFloat("y", &shape.y, 0.01f, 1.0f, Constants::UI::precision, flags);

		if (flags == ImGuiInputTextFlags_None)
		{
			cinder::Colorf color{ shape.r, shape.g, shape.b };
			const char* name = "color";
			ImGui::ColorPicker3(name, &color);
			shape.r = color.r;
			shape.g = color.g;
			shape.b = color.b;
		}
		else {
			ImVec4 color2 = ImVec4(shape.r, shape.g, shape.b, 1.0);
			ImGui::ColorButton("color_button", color2);
		}
	}

	// -Serialize
	Serialize::Serialize(Cosmos_JsonWriter& json_writer, Circle& circle)
	{
		using namespace Constants;

		json_writer.make(Json_Constants::type, Json_Constants::type_circle);

		serialize_shapes_shared(json_writer, circle);

		json_writer.make(Json_Constants::direction_x, circle.direction.x);
		json_writer.make(Json_Constants::direction_y, circle.direction.y);
		json_writer.make(Json_Constants::radius, circle.radius);
	}

	Serialize::Serialize(Cosmos_JsonWriter& json_writer, Square& square)
	{
		using namespace Constants;
		json_writer.Key(Json_Constants::type);
		json_writer.make_int(Json_Constants::type_square);

		serialize_shapes_shared(json_writer, square);

		json_writer.Key(Json_Constants::size);
		json_writer.make_double(square.size);
	}

	Serialize::Serialize(Cosmos_JsonWriter& json_writer, Rectangle& rect)
	{
		using namespace Constants;

		json_writer.Key(Json_Constants::type);
		json_writer.make_int(Json_Constants::type_rectangle);

		serialize_shapes_shared(json_writer, rect);

		json_writer.Key(Json_Constants::width);
		json_writer.make_double(rect.width);

		json_writer.Key(Json_Constants::height);
		json_writer.make_double(rect.height);
	}

	void Serialize::serialize_shapes_shared(Cosmos_JsonWriter& json_writer, Shape& shape)
	{
		namespace JC = Constants::Json_Constants;

		json_writer.make(JC::id, shape.id);
		json_writer.make(JC::rotation, shape.rotation);
		json_writer.make(JC::coordinate_x, shape.x);
		json_writer.make(JC::coordinate_y, shape.y);
		json_writer.make(JC::color_r, shape.r);
		json_writer.make(JC::color_g, shape.g);
		json_writer.make(JC::color_b, shape.b);
		json_writer.make(JC::rogue, shape.rogue);
	}

}

namespace Data_Types::Is_Clicked
{
	inline float distance_squered(const float x1, const float y1, const float x2, const float y2)
	{
		return (x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2);
	}

	bool check(const Rectangle& rect, float ix, float iy)
	{
		if (distance_squered(rect.x, rect.y, ix, iy) < (rect.width * rect.height)) //TODO : this is the formula for circle i need one for rectangle
		{
			return true;
		}

		return false;
	}

	bool check(const Square& square, float ix, float iy)
	{
		if (distance_squered(square.x, square.y, ix, iy) < (square.size * square.size)) //TODO : this is the formula for circle i need one for Square
		{
			return true;
		}

		return false;
	}

	bool check(const Circle& circle, float ix, float iy)
	{
		return distance_squered(circle.x, circle.y, ix, iy) < (circle.radius * circle.radius);
	}
}


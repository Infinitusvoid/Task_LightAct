#include "Data_Types.h"

#include "cinder/gl/gl.h"
#include "cinder/CinderImGui.h"

#include "UI_Controller.h"
#include "Cosmos_Constants.h"

namespace Data_Types
{
	
#pragma region Shapes
	namespace F_gen
	{
		namespace Details
		{
			float distance(const float x1, const float y1, const float x2, const float y2)
			{
				return (x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2);
			}

			void draw_rect(const Shape& shape, float sx, float sy)
			{
				ci::Rectf rect;
				rect.set(-sx, -sy, sx, sy);

				ci::mat3 matrix;

				// preserve the Model matrix
				ci::gl::pushModelMatrix();

				ci::gl::translate(glm::vec2(shape.x, shape.y));
				ci::gl::rotate(shape.rotation);

				ci::gl::color(shape.r, shape.g, shape.b);
				ci::gl::drawStrokedRect(rect);

				// restore the Model matrix
				ci::gl::popModelMatrix();
			}
		}

		std::function<bool(float, float)>  Clicked(const Rectangle& rectangle)
		{
			return [&, distance_squered = &Details::distance](float ix, float iy) {
				if (distance_squered(rectangle.x, rectangle.y, ix, iy) < (rectangle.width * rectangle.height)) //TODO : this is the formula for circle i need one for rectangle
				{
					return true;
				}

				return false;
			};
		}

		std::function<bool(float, float)>  Clicked(const Square& square)
		{
			return [&square, distance_squered = &Details::distance](float ix, float iy) {

				if (distance_squered(square.x, square.y, ix, iy) < (square.size * square.size)) //TODO : this is the formula for circle i need one for Square
				{
					return true;
				}

				return false;
			};

		}

		std::function<bool(float, float)>  Clicked(const Circle& circle)
		{
			return [&circle, distance_squered = &Details::distance](float ix, float iy) { return distance_squered(circle.x, circle.y, ix, iy) < (circle.radius * circle.radius); };
		}

		std::function<void(ci::Shape2d& shape2d)> Draw(const Circle& circle)
		{
			return [&circle](ci::Shape2d& shape2d) {
				ci::gl::color(circle.r, circle.g, circle.b);

				glm::vec2 center = glm::vec2(circle.x, circle.y);
				shape2d.arc(center, circle.radius, 0.0, glm::two_pi<float>());
			};
		}

		std::function<void(ci::Shape2d& shape2d)> Draw(const Square& square)
		{
			return [&square, &draw_rect = Details::draw_rect](ci::Shape2d& shape2d) {
				draw_rect(square, square.size, square.size);
			};
		}
		
		std::function<void(ci::Shape2d& shape2d)> Draw(const Rectangle& rectangle)
		{
			return[&rectangle, &draw_rect = Details::draw_rect](ci::Shape2d& shape2d) {
				draw_rect(rectangle, rectangle.width, rectangle.height);
			};
		}
		
		std::function<void(float ft, Update_Info& info)> Update(Circle& circle)
		{
			return [&circle](float dt, Update_Info& info)
			{
				circle.x += circle.direction.x * dt;
				circle.y += circle.direction.y * dt;

				info.guard(circle.radius, circle.x, circle.y, circle.direction.x, circle.direction.y);
			};
		}
		
		std::function<void(float ft, Update_Info& info)> Update(Square& square)
		{
			return [](float dt, Update_Info& info) {

			};
		}

		std::function<void(float ft, Update_Info& info)> Update(Rectangle& square)
		{
			return [](float dt, Update_Info& info) {

			};
		}

		std::function<void(void)> display_UI(Shape& shape)
		{
			return [&shape]() {
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
					cinder::Colorf color{ shape.r,shape.g, shape.b };
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

			};
		}

		std::function<void(void)> display_UI(Circle& circle)
		{
			return [&circle]()
			{
				ImGui::Text(Constants::UI::type_circle);
				circle.rogue = UI_Controller::Get().add_button(100, "circle_rogule" + std::to_string(circle.id), !circle.rogue).draw().isOff();
				//ImGui::Checkbox("Rogue", &rogue);


				circle.display_UI_shared();//display_UI_Shape_Shared(circle);

				ImGuiInputTextFlags_ flags = circle.rogue ? ImGuiInputTextFlags_None : ImGuiInputTextFlags_ReadOnly;

				ImGui::InputFloat(Constants::UI::radius, &circle.radius, 0.01f, 1.0f, Constants::UI::precision, flags);

				float  dir[2] = { circle.direction.x, circle.direction.y };

				ImGui::InputFloat2(Constants::UI::direction, dir, 3, flags);//InputFloat2("direction", &direction, 3);

				circle.direction.x = dir[0];
				circle.direction.y = dir[1];
			};
		}

		std::function<void(void)> display_UI(Square& square)
		{
			return [&square]() {
				ImGui::Text(Constants::UI::type_square);
				square.rogue = UI_Controller::Get().add_button(100, "circle_rogule" + std::to_string(square.id), !square.rogue).draw().isOff();

				square.display_UI_shared();

				ImGuiInputTextFlags_ flags = square.rogue ? ImGuiInputTextFlags_None : ImGuiInputTextFlags_ReadOnly;
				ImGui::InputFloat("size", &square.size, 0.01f, 1.0f, Constants::UI::precision, flags);
			};
		}

		std::function<void(void)> display_UI(Rectangle& rectangle)
		{
			return [&rectangle]() {
				ImGui::Text(Constants::UI::type_rectangle);

				rectangle.rogue = UI_Controller::Get().add_button(100, "circle_rogule" + std::to_string(rectangle.id), !rectangle.rogue).draw().isOff();

				rectangle.display_UI_shared();//display_UI_Shape_Shared(rect);

				ImGuiInputTextFlags_ flags = rectangle.rogue ? ImGuiInputTextFlags_None : ImGuiInputTextFlags_ReadOnly;
				ImGui::InputFloat("w", &rectangle.width, 0.01f, 1.0f, Constants::UI::precision, flags);
				ImGui::InputFloat("h", &rectangle.height, 0.01f, 1.0f, Constants::UI::precision, flags);
			};
		}

		std::function<void(Cosmos_JsonWriter& json_writer)> serialize(Shape& shape)
		{
			return [&shape](Cosmos_JsonWriter& json_writer) {
				namespace JC = Constants::Json_Constants;

				json_writer.make(JC::id, shape.id);
				json_writer.make(JC::rotation, shape.rotation);
				json_writer.make(JC::coordinate_x, shape.x);
				json_writer.make(JC::coordinate_y, shape.y);
				json_writer.make(JC::color_r, shape.r);
				json_writer.make(JC::color_g, shape.g);
				json_writer.make(JC::color_b, shape.b);
				json_writer.make(JC::rogue, shape.rogue);
			};
		}

		std::function<void(Cosmos_JsonWriter& json_writer)> serialize(Circle& circle)
		{
			return [&circle](Cosmos_JsonWriter& json_writer) {
				using namespace Constants;

				json_writer.make(Json_Constants::type, Json_Constants::type_circle);

				circle.serialize_shared(json_writer);

				json_writer.make(Json_Constants::direction_x, circle.direction.x);
				json_writer.make(Json_Constants::direction_y, circle.direction.y);
				json_writer.make(Json_Constants::radius, circle.radius);
			};
		}

		std::function<void(Cosmos_JsonWriter& json_writer)> serialize(Square& square)
		{
			return [&square](Cosmos_JsonWriter& json_writer) {
				using namespace Constants;
				json_writer.Key(Json_Constants::type);
				json_writer.make_int(Json_Constants::type_square);

				square.serialize_shared(json_writer);

				json_writer.Key(Json_Constants::size);
				json_writer.make_double(square.size);
			};
		}

		std::function<void(Cosmos_JsonWriter& json_writer)> serialize(Rectangle& rectangle)
		{
			return [&rectangle](Cosmos_JsonWriter& json_writer) {
				using namespace Constants;

				json_writer.Key(Json_Constants::type);
				json_writer.make_int(Json_Constants::type_rectangle);

				rectangle.serialize_shared(json_writer);

				json_writer.Key(Json_Constants::width);
				json_writer.make_double(rectangle.width);

				json_writer.Key(Json_Constants::height);
				json_writer.make_double(rectangle.height);
			};
		}

	}
	
	namespace Type_for
	{
		Data_Types::Shape::ShapeType constexpr shape(const Circle&)
		{
			return Data_Types::Shape::ShapeType::Circle;
		}

		Data_Types::Shape::ShapeType constexpr shape(const Rectangle&)
		{
			return Data_Types::Shape::ShapeType::Rectangle;
		}

		Data_Types::Shape::ShapeType constexpr shape(const Square&)
		{
			return Data_Types::Shape::ShapeType::Squere;
		}
	}
	
	// -Circle
	Circle::Circle() : radius(0.0f) { Connect<Circle>(*this); }
	Circle::~Circle() {}

	// -Square
	Square::Square() : size{ 0 } { Connect<Square>(*this); }
	Square::~Square() {}

	// -Rectangle
	Rectangle::Rectangle() :
		width{ 0.0f },
		height{ 0.0f }
	{
		Connect<Rectangle>(*this);
	}

	Rectangle::~Rectangle() {}

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

		display_UI_shared = F_gen::display_UI(*this);
		serialize_shared = F_gen::serialize(*this);
	}
	Shape::~Shape() { }
	
	void Update_Info::guard(const float radius, float& x, float& y, float& dx, float& dy)
	{
		auto bounce = [&radius](float& value, float& d, const float& bound)
		{
			if (value - radius < 0 || value + radius > bound) { d *= -1; }
		};

		auto limit_to = [&radius](float &value, const float &bound)
		{
			if (value - radius < 0)
			{
				value = radius;
			}
			else if (value > (bound - radius))
			{
				value = bound - radius;
			}
		};

		bounce(x, dx, bound_x);
		bounce(y, dy, bound_y);

		limit_to(x, bound_x);
		limit_to(y, bound_y);

		//bouncing
		//if (x - radius < 0 || x + radius > bound_x) { dx *= -1; }
		//if (y - radius < 0 || y + radius > bound_y) { dy *= -1; }


		//taking care of window resize
		/*
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
		*/
	}
#pragma endregion

	


	
}




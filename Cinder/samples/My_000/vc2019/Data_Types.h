#pragma once

#include <vector>
#include <assert.h>

#include "cinder/Shape2d.h"
#include "cinder/CinderGlm.h";

#include "Id_Generator.h"
#include "Cosmos_JsonWriter.h"

namespace Data_Types
{	
	
	


	struct Image_info
	{
		std::string path;
		float mult_r;
		float mult_g;
		float mult_b;
		bool enable_color;

		Image_info();//TODO : Delete this?
		Image_info(std::string path, float mult_r, float mult_g, float mult_b, bool enable_color);

		void Image_info::display_ui(int i,int k, std::function<void(int)> f);
	};

	struct Update_Info
	{
		float bound_x;
		float bound_y;

		void guard(const float radius, float& x, float& y, float& dx, float& dy);
	};

	//Base class
	class Shape
	{
	public:
		enum class ShapeType
		{
			Circle,
			Squere,
			Rectangle,
			Abstract,
		};

		float x, y;
		float r, g, b;
		bool rogue;
		int id;
		float rotation;

		ShapeType shape_type;
		
		Shape(); //TODO : this should not be used anywhere! Can I delete the constctor?
		Shape(float rotation, ShapeType shape_type, float x, float y, float r, float g, float b);
		virtual ~Shape();

		virtual void draw(ci::Shape2d& shape2d) = 0;
		virtual bool is_clicked(float ix, float iy) const = 0;
		virtual void update(float dt, Update_Info& info) = 0;
		virtual void display_UI() = 0;
		virtual void serialize(Cosmos_JsonWriter& json_writer) = 0;

		void draw_shared_UI();
		void serialize_shared(Cosmos_JsonWriter& json_writer);

		
		
	};

	//Derived classes
	class Circle : public Shape
	{
	public:
		float radius{};
		glm::vec2 direction{};

		Circle();
		virtual ~Circle();

		virtual void draw(ci::Shape2d& shape2d) override;
		virtual bool is_clicked(float ix, float iy) const override;
		virtual void update(float dt, Update_Info& info) override;
		virtual void display_UI() override;
		virtual void serialize(Cosmos_JsonWriter& json_writer) override;
	private:

	};

	class Square : public Shape
	{
	public:
		float size;

		Square();
		virtual ~Square();

		virtual void draw(ci::Shape2d& shape2d) override;
		virtual bool is_clicked(float ix, float iy) const override;
		virtual void update(float dt, Update_Info& info) override;
		virtual void display_UI() override;
		virtual void serialize(Cosmos_JsonWriter& json_writer) override;
	};

	class Rectangle : public Shape
	{
	public:
		float width;
		float height;

		Rectangle();
		virtual ~Rectangle();

		virtual void draw(ci::Shape2d& shape2d) override;
		virtual bool is_clicked(float ix, float iy) const override;
		virtual void update(float dt, Update_Info& info) override;
		virtual void display_UI() override;
		virtual void serialize(Cosmos_JsonWriter& json_writer) override;
	};

}


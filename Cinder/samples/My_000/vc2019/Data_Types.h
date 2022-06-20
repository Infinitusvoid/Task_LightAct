#pragma once

#include <vector>
#include <assert.h>
#include <functional>

#include "cinder/Shape2d.h"
#include "cinder/CinderGlm.h";

#include "Id_Generator.h"
#include "Cosmos_JsonWriter.h"

namespace Data_Types
{	
	

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

		ShapeType shape_type;

		float x, y;
		float r, g, b;
		bool rogue;
		int id;
		float rotation;

		Shape(); //TODO : this should not be used anywhere! Can I delete the constctor?
		virtual ~Shape();

		std::function<void(void)> display_UI_shared;
		std::function<void(Cosmos_JsonWriter& json_writer)> serialize_shared;

		std::function<void(ci::Shape2d& shape2d)> draw;
		std::function<bool(float, float)> is_clicked;
		std::function<void(float ft, Update_Info& info)> update;
		std::function<void(void)> display_UI;
		std::function<void(Cosmos_JsonWriter& json_writer)> serialize;
		
	};

	//Derived classes
	class Circle : public Shape
	{
	public:
		float radius{};
		glm::vec2 direction{};

		Circle();
		virtual ~Circle();
	};
	
	class Square : public Shape
	{
	public:
		float size;

		Square();
		virtual ~Square();
	};

	class Rectangle : public Shape
	{
	public:
		float width;
		float height;

		Rectangle();
		virtual ~Rectangle();
	};

	template<typename T>
	void Connect(T& obj)
	{
		obj.shape_type = Type_for::shape(obj);
		obj.draw = F_gen::Draw(obj);
		obj.is_clicked = F_gen::Clicked(obj);
		obj.update = F_gen::Update(obj);
		obj.display_UI = F_gen::display_UI(obj);
		obj.serialize = F_gen::serialize(obj);
	}

}


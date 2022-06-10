#pragma once

#include<string>

namespace Constants
{
	namespace FilePaths
	{
		const std::string path_base = ""; 
		

		const std::string path_file_name = "project.json"; //TODO : has to be for project
		const std::string path = path_base + path_file_name;
	}

	namespace UI
	{
		static constexpr char* type_circle = "Type Circle";
		static constexpr char* type_square = "Type Square";
		static constexpr char* type_rectangle = "Type Rectangle";
		static constexpr char* radius = "radius";
		static constexpr char* precision = "%.3f";
		static constexpr char* direction = "direction";
	}

	namespace Json_Constants
	{
		static constexpr char* data = "data";

		static constexpr char* type = "type";
		static constexpr int type_circle = 0;
		static constexpr int type_square = 1;
		static constexpr int type_rectangle = 2;

		//Shared
		static constexpr char* id = "id";
		static constexpr char* coordinate_x = "x";
		static constexpr char* coordinate_y = "y";
		static constexpr char* rotation = "rotation";
		static constexpr char* color_r = "r";
		static constexpr char* color_g = "g";
		static constexpr char* color_b = "b";
		static constexpr char* rogue = "rogue";

		//Rectangle
		static constexpr char* width = "width";
		static constexpr char* height = "height";

		//Circle
		static constexpr char* radius = "radius";
		static constexpr char* direction_x = "dx";
		static constexpr char* direction_y = "dy";

		//Square
		static constexpr char* size = "size";

		//Images
		namespace Img
		{
			
			static constexpr char* info_images = "info_images";
			static constexpr char* path = "path";
			static constexpr char* enable_color = "enable_color";
			static constexpr char* mult_r = "mult_r";
			static constexpr char* mult_g = "mult_g";
			static constexpr char* mult_b = "mult_b";
		}
		

	}

	namespace Shader {

		const std::string program_frag =
			"#version 150 \n"

			"uniform sampler2D iSrc; \n"
			"uniform sampler2D iDst; \n"
			"uniform float     iFade; \n"

			"uniform vec4 color_mult; \n"

			"in vec4 vertColor; \n"
			"in vec2 vertTexCoord0; \n"

			"out vec4 fragColor; \n"

			"void main() \n"
			"{ \n"
			"vec4 srcColor = texture(iSrc, vertTexCoord0); \n"

			"if (color_mult[3] == 1.0) \n"
			"{ \n"
			"fragColor = srcColor * vec4(vec3(color_mult), 1.0); \n"
			"} \n"
			"else \n"
			"{ \n"
			"fragColor = vec4(srcColor.rrr, 1.0); \n"
			"} \n"

			"} \n";

		const std::string program_vert =
			"#version 150 \n"

			"uniform mat4 ciModelViewProjection; \n"
			"uniform vec4 color_mult; \n"

			"in vec4 ciPosition; \n"
			"in vec4 ciColor; \n"
			"in vec2 ciTexCoord0; \n"

			"out vec4 vertColor; \n"
			"out vec2 vertTexCoord0; \n"
			"void main() \n"
			"{ \n"
			"vertColor = vec4(vertTexCoord0[0], 1.0, 0.0, 0.0);//ciColor; \n"
			"vertTexCoord0 = ciTexCoord0; \n"
			"gl_Position = ciModelViewProjection * ciPosition; \n"
			"} \n";

	}
}
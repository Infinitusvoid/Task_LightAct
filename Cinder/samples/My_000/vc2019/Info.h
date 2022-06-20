#pragma once

#include <string>

namespace Info
{
	struct Image_info
	{
		std::string path;
		float mult_r;
		float mult_g;
		float mult_b;
		bool enable_color;
	};
}
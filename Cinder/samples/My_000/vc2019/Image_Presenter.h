#pragma once

#include <vector>
#include "Info.h"
#include "cinder/gl/gl.h"

class Image_Presenter
{
private:
	int selected;
	bool edit;
public:
	bool first_time;

	Image_Presenter();
	

	~Image_Presenter();
	void draw(std::vector<ci::gl::Texture2dRef>& images, std::vector<Info::Image_info>& infos);
	
};


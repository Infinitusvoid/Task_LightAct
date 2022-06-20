#pragma once

#include <vector>
#include "Info.h"
#include "cinder/gl/gl.h"

class Texture_controller
{
public:
	std::vector<ci::gl::Texture2dRef> info_images_textures;

	void add_texture(const cinder::ImageSourceRef& img);
	void load_from_file(std::vector<Info::Image_info>& infos);
	void draw(const std::vector<Info::Image_info>& infos, const ci::gl::GlslProgRef& shader_prog);
};


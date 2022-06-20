#include "Texture_controller.h"
#include "cinder/app/RendererGl.h"
#include "cinder/app/App.h"

void Texture_controller::add_texture(const cinder::ImageSourceRef& img)
{
	ci::gl::Texture::Format fmt;
	fmt.setWrap(GL_REPEAT, GL_REPEAT);
	info_images_textures.push_back(ci::gl::Texture2d::create(img, fmt));
}

void Texture_controller::load_from_file(std::vector<Info::Image_info>& infos)
{
	info_images_textures.clear();

	for (auto img_info : infos)
	{
		auto img = ci::loadImage(img_info.path); //ok
		ci::gl::Texture::Format fmt;
		fmt.setWrap(GL_REPEAT, GL_REPEAT);
		info_images_textures.push_back(ci::gl::Texture2d::create(img, fmt));
	}
}

void Texture_controller::draw(const std::vector<Info::Image_info>& infos, const ci::gl::GlslProgRef& shader_prog)
{
	int num_images = infos.size();
	float width_image = 1.0f / (float)(num_images);
	for (int i = 0; i < num_images; i++)
	{
		float factor_r = infos[i].mult_r;
		float factor_g = infos[i].mult_g;
		float factor_b = infos[i].mult_b;
		float factor_grayscale = infos[i].enable_color ? 1.0f : 0.0f; //if's it's anything but one (make grayscale red = red, green = red, blue = red)

		info_images_textures[i]->bind();

		ci::gl::ScopedGlslProg shader(shader_prog);

		{
			auto shader = ci::gl::context()->getGlslProg();
			if (!shader)
			{
				//error
				return;
			}

			shader->uniform("color_mult", glm::vec4(factor_r, factor_g, factor_b, factor_grayscale));

		}

		ci::gl::drawSolidRect(ci::Rectf(
			(float)ci::app::getWindowWidth() * width_image * i,
			0,
			(float)ci::app::getWindowWidth() * width_image * (i + 1),
			(float)ci::app::getWindowHeight()
		));
		info_images_textures[i]->unbind();
	}
}

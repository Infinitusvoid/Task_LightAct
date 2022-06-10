// Cinder
#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"
#include "cinder/Shape2d.h"
//#include "cinder/Rand.h"


// ImGui
#include "cinder/CinderImGui.h"

// Cosmos
#include "UI_Controller.h"
#include "Serialization.h"
#include "Random.h"

// std
#include <filesystem>
#include <map>



using namespace ci;
using namespace ci::app;


namespace Shader
{
	namespace Helpers
	{
		static gl::GlslProgRef create(const char* path_frag, const char* path_vert, const std::string fallback_frag, const std::string fallback_vert)
		{
			std::string vert_prog;

			if (Cosmos_file_IO::file_exist(path_vert))
			{
				Cosmos_file_IO::read_file(path_vert, vert_prog);
			}
			else {
				vert_prog = fallback_vert;
			}

			std::string shader_prog;
			if (Cosmos_file_IO::file_exist(path_frag))
			{
				Cosmos_file_IO::read_file(path_frag, shader_prog);
			}
			else {
				shader_prog = fallback_frag;
			}

			return gl::GlslProg::create(vert_prog, shader_prog);
		}
	}

	static void Create(gl::GlslProgRef& prog)
	{
		prog = Helpers::create("effect.frag", "passThrough.vert", Constants::Shader::program_frag, Constants::Shader::program_vert);
	}
}

namespace Utils
{
	void randomizer_radius_and_color_of_circle(const std::unique_ptr<Data_Types::Circle>& circle) {
		circle->radius = Random::nextFloat(10.0f, 100.0f);//.nextFloat(10.0f, 100.0f);
		circle->r = Random::rnd_01();// rnd.nextFloat(0.0f, 1.0f);
		circle->g = Random::rnd_01();// rnd.nextFloat(0.0f, 1.0f);
		circle->b = Random::rnd_01();// rnd.nextFloat(0.0f, 1.0f);
	}

	void create_circle_at_position(std::unique_ptr<Data_Types::Circle>& circle, float x, float y)
	{
		circle = std::make_unique<Data_Types::Circle>();
		circle->x = x;
		circle->y = y;
	}

	void create_circle_at_position_with_random_direction_vector(std::unique_ptr<Data_Types::Circle>& circle, float x, float y)
	{
		circle = std::make_unique<Data_Types::Circle>();
		circle->x = x;
		circle->y = y;



		circle->direction = glm::vec2(Random::nextFloat(-100.0, 100.0), Random::nextFloat(-100.0, 100.0));
	}

	std::string random_txt()
	{
		constexpr int length = 15;

		char arr[length + 1];
		for (int i = 0; i < length; i++)
		{

			arr[i] = 97 + Random::nextUint() % 10;
		}
		arr[length] = '\0';
		std::string str = (char*)arr;
		return str;
	}
}


class CosmosApp : public App {
public:
	// Cinder will call 'mouseDrag' when the user moves the mouse while holding one of its buttons.
	// See also: mouseMove, mouseDown, mouseUp and mouseWheel.
	void mouseDrag(MouseEvent event) override;

	void mouseDown(MouseEvent event) override;

	// Cinder will call 'keyDown' when the user presses a key on the keyboard.
	// See also: keyUp.
	void keyDown(KeyEvent event) override;

	// Cinder will call 'draw' each time the contents of the window need to be redrawn.
	void draw() override;

	void setup() override;

	void update() override;

	void fileDrop(FileDropEvent event) override;

	
	gl::GlslProgRef m_shader_prog;

	ShapesManger manager;
	std::vector<gl::Texture2dRef> info_images_textures;

	void save_to_file();
	void load_images_from_disk();
private:
	//Rand rand;
	
	double ellapsed_time_past;

	void draw_fbo();

	
};

namespace Task
{
	void create_randomized_circles(ShapesManger& manager, const float x, const float y) {
		std::unique_ptr<Data_Types::Circle> circle;


		Utils::create_circle_at_position_with_random_direction_vector(circle, x, y);
		Utils::randomizer_radius_and_color_of_circle(circle);
		manager.obj_list.push_back(std::move(circle));
	}

	void calculate_dt(double& ellapsed_time_past, double& dt)
	{
		double ellapset_current = getElapsedSeconds();
		dt = ellapset_current - ellapsed_time_past;
		ellapsed_time_past = ellapset_current;
	}

	namespace Draw::UI
	{
		void Harmonica(CosmosApp& cosmosApp)
		{
			ImGui::Begin("Harmonica");
			glm::vec2 window_size{ 500, 500 };
			ImGui::SetWindowSize(window_size);
			glm::vec2 window_pos{500, 50 };
			ImGui::SetWindowPos(window_pos);

			if (cosmosApp.info_images_textures.size() != cosmosApp.manager.info_images_list.size())
			{
				return;
			}

			int num_images = cosmosApp.info_images_textures.size();

			static int k = -1;
			static bool edit = false;
			for (int i = 0; i < num_images; i++)
			{
				ImGui::Spacing();
				gl::Texture2dRef& el = cosmosApp.info_images_textures[i];
				ImGui::Image(el, glm::vec2(50, 50));

				cosmosApp.manager.info_images_list[i].display_ui(i, k, [&](int i) {k = i; edit = true; });
				

				if (k != -1 && i == k)
				{
					//std::string clicked = "Click to edit : " + std::to_string(k);
					//ImGui::Text(clicked.c_str());

					if (k != -1)
					{
						if (ImGui::Button("Close"))
						{
							k = -1;
							continue;
						}
					}
					

					ImGui::Checkbox("enable color", &cosmosApp.manager.info_images_list[k].enable_color);

					ImGui::SliderFloat("r", &cosmosApp.manager.info_images_list[k].mult_r, 0, 10, "%.3f");
					//ImGui::InputFloat("r", &cosmosApp.manager.info_images_list[k].mult_r, 0.01f, 1.0f, "%.3f");//, flags);

					ImGui::SliderFloat("g", &cosmosApp.manager.info_images_list[k].mult_g, 0, 10, "%.3f");
					//ImGui::InputFloat("g", &cosmosApp.manager.info_images_list[k].mult_g, 0.01f, 1.0f, "%.3f");//, flags);

					ImGui::SliderFloat("b", &cosmosApp.manager.info_images_list[k].mult_b, 0, 10, "%.3f");
					//ImGui::InputFloat("b", &cosmosApp.manager.info_images_list[k].mult_b, 0.01f, 1.0f, "%.3f");//, flags);

					ImGui::Spacing();
					ImGui::Spacing();
					ImGui::Spacing();
				}
			}

			ImGui::End();
		}

		void Menu(CosmosApp& cosmosApp)
		{
			if (ImGui::BeginMainMenuBar())
			{

				if (ImGui::BeginMenu("File"))
				{
					if (ImGui::MenuItem("Save", "CTRL+S")) {
						cosmosApp.save_to_file();
					}
					if (ImGui::MenuItem("Open", "CTRL+O")) {
						cosmosApp.manager.create_from_json(Constants::FilePaths::path);
						cosmosApp.load_images_from_disk();
					}

					ImGui::EndMenu();
				}

				ImGui::EndMainMenuBar();
			}
		}

		void List(ShapesManger& manager)//, int& selectedItem)
		{
			ImGui::Begin("Window : List");
			glm::vec2 window_size{ 400, 500 };
			ImGui::SetWindowSize(window_size);
			glm::vec2 window_pos{ 50, 50 };
			ImGui::SetWindowPos(window_pos);
			
			float window_heigth = ImGui::GetWindowSize().y;

			int rows = 27;
			if (window_heigth > 10)
			{
				rows = static_cast<int>(((window_heigth / (float)500) * 27.0f));
			}
			
			
			std::vector<const char*> items;
			std::vector<std::string> vec;

			//constructing items
			{
				
				for (int i = 0; i < manager.obj_list.size(); i++) //TODO : you should not be doing this every frame manager should do this when adding new object
				{
					vec.push_back("object " + std::to_string(i) + " id : " + std::to_string(manager.obj_list[i]->id));
				}

				for (int i = 0; i < vec.size(); i++) {
					items.push_back(&*vec[i].begin());
				}
			}
			

			if (manager.obj_list.size() > 0)
			{
				//ImGui::ListBox("ListBox", &manager.sel.index, &items[0], items.size());
				ImGui::ListBox("Object List", &manager.sel.index, &items[0], items.size(), rows);//27);
			}

			if (manager.sel.index >= 0 && manager.sel.index < manager.obj_list.size())
			{
				manager.sel.id = manager.obj_list[manager.sel.index]->id;
			}

			ImGui::End();
		}

		void Properties(ShapesManger& manager)// int& selectedItem)
		{
			if (!manager.validate_selection() ||
				 manager.obj_list.size() == 0  ||
				 manager.sel.index == -1)
			{
				return;
			}



			ImGui::Begin("Window : Properties");
			ImGui::SetWindowSize("Window : Properties", ImVec2(500, 1000));
			
			manager.obj_list[manager.sel.index]->display_UI();

			ImGui::End();
		}
	}

}

void CosmosApp::load_images_from_disk()
{
	info_images_textures.clear();

	for (auto img_info : manager.info_images_list)
	{
		auto img = loadImage(img_info.path); //ok
		gl::Texture::Format fmt;
		fmt.setWrap(GL_REPEAT, GL_REPEAT);
		info_images_textures.push_back(gl::Texture2d::create(img, fmt));
	}
}

void CosmosApp::fileDrop(FileDropEvent event)
{
	int i = 0;
	while (i < event.getNumFiles())
	{
		try {
			auto file = event.getFile(i);
			auto img = loadImage(file);
			gl::Texture::Format fmt;
			fmt.setWrap(GL_REPEAT, GL_REPEAT);
			info_images_textures.push_back(gl::Texture2d::create(img, fmt));
			
			std::string path_string{ file.u8string() };
			manager.info_images_list.push_back(Data_Types::Image_info(path_string, 1.0, 1.0, 1.0, true));
		}
		catch (Exception& exc) {
			//CI_LOG_EXCEPTION("failed to load image: " << event.getFile(0), exc);
		}
		i++;
	}

}

void prepareSettings(CosmosApp::Settings* settings)
{
	settings->setMultiTouchEnabled(false);
}





void CosmosApp::setup()
{
	
	ImGui::Initialize();

	ellapsed_time_past = 0.0;

	Shader::Create(m_shader_prog);
	//Utils::read_shaders_and_create_a_program(m_shader_prog);
}





void CosmosApp::mouseDown(MouseEvent event)
{
	auto pos = event.getPos();

	if (event.isLeft())
	{
		Task::create_randomized_circles(manager, pos.x, pos.y);
	}

	if (event.isRight())
	{
		manager.delete_if_overlap(pos.x, pos.y);
	}

}

void CosmosApp::mouseDrag(MouseEvent event)
{
	// Store the current mouse position in the list.
	
}

void CosmosApp::keyDown(KeyEvent event)
{

	if (event.getChar() == 'f') {
		// Toggle full screen when the user presses the 'f' key.
		setFullScreen(!isFullScreen());
	}
	else if (event.getCode() == KeyEvent::KEY_SPACE) {
		// Clear the list of points when the user presses the space bar.
		
	}
	else if (event.getCode() == KeyEvent::KEY_ESCAPE) {
		// Exit full screen, or quit the application, when the user presses the ESC key.
		if (isFullScreen())
			setFullScreen(false);
		else {
			//quit(); //Leave it here for now
		}

	}
	else if (event.getCode() == KeyEvent::KEY_w)
	{
		manager.move_last_up();
	}
	else if (event.getCode() == KeyEvent::KEY_a)
	{
		manager.move_last_left();
	}
	else if (event.getCode() == KeyEvent::KEY_s)
	{
		manager.move_last_down();
	}
	else if (event.getCode() == KeyEvent::KEY_d)
	{
		manager.move_last_rigth();
	}
	else if (event.getCode() == KeyEvent::KEY_k)
	{
		manager.unselect();
	}
	else if (event.getCode() == KeyEvent::KEY_1)
	{
		glm::ivec2 pos = getMousePos();
		manager.add_squere(pos.x, pos.y);
	}
	else if (event.getCode() == KeyEvent::KEY_2)
	{
		glm::ivec2 pos = getMousePos();
		manager.add_rectangle(pos.x, pos.y);
	}

}

void CosmosApp::save_to_file()
{
	// -String generation
	Serialization s;
	std::string str;
	s.run(manager).get_string(str);
	
	// -File rename, File_write
	std::string unique_name = Constants::FilePaths::path_base + Utils::random_txt() + ".json";
	Cosmos_file_IO::rename_file_if_exist(
		Constants::FilePaths::path.c_str(),
		unique_name.c_str()
	);
	std::string path = Constants::FilePaths::path;
	Cosmos_file_IO::write_file_overwrite(path, str);
}

void CosmosApp::update()
{

	double dt;
	Task::calculate_dt(ellapsed_time_past, dt);

	auto area = getWindowBounds();
	Data_Types::Update_Info update_info{ area.getWidth() ,  area.getHeight() };

	// Update loop
	for (const auto& sha : manager.obj_list)
	{
		sha->update(dt, update_info);
	}

	if (ImGui::IsKeyPressed('k') || ImGui::IsKeyPressed(27)) //27 esc
	{
		manager.unselect();
	}

}

void CosmosApp::draw_fbo()
{
	int num_images = manager.info_images_list.size();
	float width_image = 1.0f / (float)(num_images);
	for (int i = 0; i < num_images; i++)
	{
		float factor_r = manager.info_images_list[i].mult_r;
		float factor_g = manager.info_images_list[i].mult_g;
		float factor_b = manager.info_images_list[i].mult_b;
		float factor_grayscale = manager.info_images_list[i].enable_color ? 1.0f : 0.0f; //if's it's anything but one (make grayscale red = red, green = red, blue = red)

		info_images_textures[i]->bind();

		gl::ScopedGlslProg shader(m_shader_prog);

		{
					auto shader = gl::context()->getGlslProg();
					if (!shader)
					{
						//error
						return;
					}

					shader->uniform("color_mult", glm::vec4(factor_r, factor_g, factor_b, factor_grayscale));
					
		}

		gl::drawSolidRect(Rectf(
			(float)getWindowWidth() * width_image * i,
			0,
			(float)getWindowWidth() * width_image * (i + 1),
			(float)getWindowHeight()
		));
		info_images_textures[i]->unbind();
	}
}

void CosmosApp::draw()
{
	gl::clear(Color(0, 0, 0));
	
	
	Task::Draw::UI::Menu(*this);

	Task::Draw::UI::List(manager);
	Task::Draw::UI::Properties(manager);

	Task::Draw::UI::Harmonica(*this);
	


	// Clear the contents of the window. This call will clear
	// both the color and depth buffers. 
	gl::clear(Color::gray(0.1f));

	

	
	draw_fbo();
	
	
	
	
	
	// Set the current draw color to orange by setting values for
	// red, green and blue directly. Values range from 0 to 1.
	// See also: gl::ScopedColor
	gl::color(1.0f, 0.5f, 0.25f);

	//gl::clear(Color(0.01f * cosmos_counter, 0.01f * cosmos_counter,0.01f * cosmos_counter));

	for (const auto& sha : manager.obj_list)
	{
		Shape2d s;
		sha->draw(s);
		gl::draw(s);
	}

}

// This line tells Cinder to actually create and run the application.
CINDER_APP(CosmosApp, RendererGl, prepareSettings)
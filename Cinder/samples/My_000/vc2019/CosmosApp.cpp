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
//#include "Serialization.h"
#include "Data_Types.h"
#include "Cosmos_file_IO.h"
#include "Cosmos_Constants.h"
#include "Manager.h"

#include "Random.h"

// std
#include <filesystem>
#include <map>


#include "Image_Presenter.h"
#include "Texture_controller.h"


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

//let's start

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

	Manager manager;
	//std::vector<gl::Texture2dRef> info_images_textures;

	
	void save_to_file();
	void load_images_from_disk();
private:
	//Rand rand;
	
	double ellapsed_time_past;
	
};

namespace Task
{
	void calculate_dt(double& ellapsed_time_past, double& dt)
	{
		double ellapset_current = getElapsedSeconds();
		dt = ellapset_current - ellapsed_time_past;
		ellapsed_time_past = ellapset_current;
	}

	namespace Draw::UI
	{
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
						//cosmosApp.manager.create_from_json(Constants::FilePaths::path);
						read_JSON(cosmosApp.manager, Constants::FilePaths::path);
						cosmosApp.load_images_from_disk();
					}

					ImGui::EndMenu();
				}

				ImGui::EndMainMenuBar();
			}
		}
	}

}

void CosmosApp::load_images_from_disk()
{
	manager.load_textures_from_file(); //manager.tex_controller->load_from_file(manager.info_images_list);
}

void CosmosApp::fileDrop(FileDropEvent event)
{
	int i = 0;
	while (i < event.getNumFiles())
	{
		try {
			auto file = event.getFile(i);
			manager.add_image(file);
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
}

void CosmosApp::mouseDown(MouseEvent event)
{
	auto pos = event.getPos();

	if (event.isLeft())
	{
		//Task::create_randomized_circles(manager, pos.x, pos.y);
		manager.create_randomized_circle(pos.x, pos.y);
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
	//Serialization s;
	std::string str;
	write_JSON(manager, str);
	//s.run(manager).get_string(str);
	
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
	manager.update(dt, update_info);
	
	if (ImGui::IsKeyPressed('k') || ImGui::IsKeyPressed(27)) //27 esc
	{
		manager.unselect();
	}

}

void CosmosApp::draw()
{
	gl::clear(Color(0, 0, 0));
	
	
	Task::Draw::UI::Menu(*this);

	manager.draw_UI_List();
	manager.draw_UI_Properties();
	
	

	manager.draw_UI_textures();


	// Clear the contents of the window. This call will clear
	// both the color and depth buffers. 
	gl::clear(Color::gray(0.1f));

	

	manager.draw_textures(m_shader_prog);
	
	
	
	
	
	// Set the current draw color to orange by setting values for
	// red, green and blue directly. Values range from 0 to 1.
	// See also: gl::ScopedColor
	gl::color(1.0f, 0.5f, 0.25f);

	//gl::clear(Color(0.01f * cosmos_counter, 0.01f * cosmos_counter,0.01f * cosmos_counter));
	manager.draw();
}

// This line tells Cinder to actually create and run the application.
CINDER_APP(CosmosApp, RendererGl, prepareSettings)
// imGUIexample.cpp : Defines the entry point for the application.
//

#include "UseImGui.h"
#include "glad/glad.h"
#include "GLFW/glfw3.h"

using namespace std;

int W = 0;
int H = 0;

class Boids
{

	float x, y;							// Coordinates
	float velocity_x, velocity_y;		// Velocity
	float direction;					// Direction of boid
	float radius;						// Radius of vision
	float angle;						// Angle of vision

	// void change(boid)
	// {
	// 	for i in boids_massive:
	//		switch:
	//			if boid.x > 29.9 || boid.x < 0.1:
	//				velocity_x = -velocity_x;
	//			if boid.y > 29.9 || boid.y < 0.1:
	//				velocity_y = -velocity_y;
	//
	// 		if (((boid.x - i.x) ** 2 + (boid.y - i.y) ** 2) ** 0.5) <= boid.radius:
	// 			velocity_x...
	// }

}

class CustomImGui : public UseImGui {
public:
	virtual void Update() override {

		// render your GUI
		static float f = 0.0f;
		static int counter = 0;

		ImGui::Begin("CustomImGui Hello, world!");              // Create a window called "Hello, world!" and append into it.

		ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)

		ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f

		bool clear_color_changed = ImGui::ColorEdit3("clear color", (float*)clear_color); // Edit 3 floats representing a color

		if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
			counter++;
		ImGui::SameLine();
		ImGui::Text("counter = %d", counter);

		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

		ImGui::Button("Rhythm1");
		ImGui::Text("display size: %d, %d", H, W);

        static int e = 0;
		ImGui::RadioButton("Привет!", &e, 0); // ImGui::SameLine();
		//ImGui::RadioButton(u8"Привет!", &e, 1); // ImGui::SameLine();
		ImGui::RadioButton("radio c", &e, 2);

		if (ImGui::TreeNode("Дерево"))
		{
			// Using the generic BeginListBox() API, you have full control over how to display the combo contents.
			// (your selection data could be an index, a pointer to the object, an id for the object, a flag intrusively
			// stored in the object itself, etc.)
			const char* items[] = { "AAAA", "BBBB", "CCCC", "DDDD", "EEEE", "FFFF", "GGGG", "HHHH", "IIII", "JJJJ", "KKKK", "LLLLLLL", "MMMM", "OOOOOOO" };
			static int item_current_idx = 0; // Here we store our selection data as an index.
			if (ImGui::BeginListBox("listbox 1"))
			{
				for (int n = 0; n < IM_ARRAYSIZE(items); n++)
				{
					const bool is_selected = (item_current_idx == n);
					if (ImGui::Selectable(items[n], is_selected))
						item_current_idx = n;

					// Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
					if (is_selected)
						ImGui::SetItemDefaultFocus();
				}
				ImGui::EndListBox();
			}

			// Custom size: use all width, 5 items tall
			ImGui::Text("Full-width:");
			if (ImGui::BeginListBox("##listbox 2", ImVec2(-FLT_MIN, 5 * ImGui::GetTextLineHeightWithSpacing())))
			{
				for (int n = 0; n < IM_ARRAYSIZE(items); n++)
				{
					const bool is_selected = (item_current_idx == n);
					if (ImGui::Selectable(items[n], is_selected))
						item_current_idx = n;

					// Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
					if (is_selected)
						ImGui::SetItemDefaultFocus();
				}
				ImGui::EndListBox();
			}

			ImGui::TreePop();
		}

		ImGui::End();


		if (clear_color_changed) {
			change_clear_color(clear_color[0], clear_color[1], clear_color[2]);
		}

	}

private:
	float clear_color[3] = { .0f, .0f, .0f };
	void change_clear_color(float r, float g, float b) {
		glClearColor(r, g, b, 1.00f);
	}
};



int main()
{
	// Setup window
	if (!glfwInit())
		return 1;

	// GL 3.0 + GLSL 130
	const char *glsl_version = "#version 130";
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
	//glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // 3.0+ only

	// Create window with graphics context
	GLFWwindow *window = glfwCreateWindow(1280, 720, "Dear ImGui - Example", NULL, NULL);
	if (window == NULL)
		return 1;
	glfwMakeContextCurrent(window);
	glfwSwapInterval(1); // Enable vsync

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))  // tie window context to glad's opengl funcs
		throw("Unable to context to OpenGL");

	int screen_width, screen_height;
	glfwGetFramebufferSize(window, &screen_width, &screen_height);
	glViewport(0, 0, screen_width, screen_height);

	auto* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
	W = mode->width;
	H = mode->height;

	CustomImGui myimgui;
	myimgui.Init(window, glsl_version);
	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();



		glClear(GL_COLOR_BUFFER_BIT);
		myimgui.NewFrame();
		myimgui.Update();
		myimgui.Render();
		glfwSwapBuffers(window);

	}
	myimgui.Shutdown();

	return 0;
}

#include "config.h"
#include "dispatcher.h"
#include "elevator.h"
#include "panel.h"
#include <cstdio>
#include <memory>
#include <vector>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <GLFW/glfw3.h>

void RenderElevatorPanel(std::vector<std::unique_ptr<Elevator>> &elevators) {
    // Setup GLFW window
    glfwInit();
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "ELEVATOR SIMULATOR", NULL, NULL);
    glfwMakeContextCurrent(window);

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 130");

    // Main rendering loop
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        // Start the ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // Set the inner window attributes
        ImGui::SetNextWindowPos(ImVec2(0.0f, 0.0f));
        ImGui::SetNextWindowSize(ImVec2(float(WINDOW_WIDTH), float(WINDOW_HEIGHT)));
        ImGuiWindowFlags flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse;
        ImGui::Begin("Elevator Panel", nullptr, flags);

        // Show the current status of the elevators
        if (ImGui::BeginTable("ElevatorDisplays", ELEVATOR_COUNT)) {
            for (int i = 0; i < ELEVATOR_COUNT; i++) {
                ImGui::TableNextColumn();
                ImGui::Text("ELEVATOR %d", i + 1);
                ImGui::Separator();
                
                ImGui::SetWindowFontScale(2.0f);
                ImGui::Text("[ %d ]", elevators[i] -> get_current_floor());
                ImGui::SameLine();
                if (elevators[i] -> get_direction() == 1) {
                    ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.0f, 1.0f), "^");
                } else if (elevators[i] -> get_direction() == -1) {
                    ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "v");
                } else {
                    ImGui::Text(" ");
                }
                ImGui::SetWindowFontScale(1.0f);
            }
            ImGui::EndTable();
        }

        ImGui::Spacing();
        ImGui::Separator();
        ImGui::Spacing();

        // Select the floor of the panel
        static int panel_floor = 1;
        ImGui::SetWindowFontScale(1.5f);
        ImVec2 button_size = ImVec2(30.0f, 30.0f);
        if (ImGui::Button("<##Decrease", button_size)) {
            if (panel_floor > 1) {
                panel_floor--;
            }
        }

        ImGui::SameLine();
        ImGui::Text("Current Floor: %dF", panel_floor); 
        ImGui::SameLine();

        if (ImGui::Button(">##Increase", button_size)) {
            if (panel_floor < 10) {
                panel_floor++;
            }
        }
        ImGui::SetWindowFontScale(1.0f);

        ImGui::Spacing();
        ImGui::Separator();
        ImGui::Spacing();

        // Show the floor buttons
        ImGui::SetWindowFontScale(3.0f);
        ImVec2 grid_button_size = ImVec2(75.0f, 75.0f);
        if (ImGui::BeginTable("FloorButtonsGrid", 5, ImGuiTableFlags_SizingFixedSame)) {
            for (int i = 1; i <= 10; i++) {
                if (i <= 5) {
                    ImGui::Dummy(ImVec2(0.0f, 1.5f));
                }
                ImGui::TableNextColumn();
                char buf[16];
                sprintf(buf, "%d##FloorBtn", i);
                if (ImGui::Button(buf, grid_button_size) && i != panel_floor) {
                    DispatchElevators(elevators, panel_floor, i);
                }
            }
            ImGui::EndTable();
        }
        ImGui::SetWindowFontScale(1.0f);

        ImGui::End();

        // Rendering
        ImGui::Render();
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        glfwSwapBuffers(window);
    }

    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwDestroyWindow(window);
    glfwTerminate();
}
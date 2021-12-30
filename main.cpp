#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdlib.h>
#include <algorithm>
#include <math.h>
#include <time.h>
#include <iostream>
#include <vector>
#include<assert.h>
#include <random>

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <stb/stb_image.h>

#include "TextureClass.h"
#include "ShaderClass.h"
#include "VAO.h"
#include "VBO.h"
#include "EBO.h"
#include "CameraClass.h"

// Resolution
const GLsizei width = 1280; 
const GLsizei height = 720;
const GLint resolution[] = { width, height };

// Number of points
int points = 300;
// Vector of vertice
std::vector<GLfloat> vertices;

// Default palette
int palette = 1;
// Default mode
int mode = 8;
// Enable/disable points by input at the beginning
bool pointsByInput = false;

// Setting a good random number
std::random_device rd; // obtain a random number from hardware
std::mt19937 gen(rd()); // seed the generator
std::uniform_real_distribution<> distr(0, 1); // define the range

// Keyboard input 
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) 
{
    if (action == GLFW_PRESS)
    {
        switch (key)
        {
            // Add and remove point
            case GLFW_KEY_UP:
                if (points == 500) {
                    std::cout << "Too much points!!!" << std::endl;
                    break;
                }
                else {
                    points++;
                    vertices.push_back(distr(gen));
                    std::cout << "One more! :O" << std::endl;
                    break;
                }
            
            // Switch palette
            case GLFW_KEY_DOWN:
                points--;
                vertices.pop_back();
                std::cout << "There it goes :C" << std::endl;
                break;

            case GLFW_KEY_1:
                palette = 1;
                std::cout << "Black and white" << std::endl;
                break;
            case GLFW_KEY_2:
                palette = 2;
                std::cout << "Red and blue" << std::endl;
                break;
            case GLFW_KEY_3:
                palette = 3;
                std::cout << "UwU" << std::endl;
                break;

            // Switch mode
            case GLFW_KEY_Q:
                mode = 8;
                std::cout << "Normal" << std::endl;
                break;
            case GLFW_KEY_W:
                mode = mode != 9 ? 9 : 8;
                std::cout << "Isolines" << std::endl;
                break;
            case GLFW_KEY_E:
                mode = mode != 0 ? 0 : 8;
                std::cout << "Softness... just in B&W... without isolines..." << std::endl;
        }
    }    
}

int main() {
    // Number of points input
    if (pointsByInput)
    {
        while (true)
        {
            std::cout << "Number of points (limit: 500): ";
            std::cin >> points;
            std::cout << std::endl;
            if (points <= 500)
            {
                break;
            }
            else
            {
                std::cout << "Too much points!!!" << std::endl;
            }
        }
    }

    assert(points <= 500);

    // Initialize GLFW
    glfwInit();
    // Set glfw for OpenGl 3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    // CORE profile, modern functions only
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    // Check window creation
    GLFWwindow* window = glfwCreateWindow(width, height, "OwO", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed creating glfw window" << std::endl;
        glfwTerminate();
        return -1;
    }
    // Keyboard inputs

    glfwSetKeyCallback(window, keyCallback);

    // Telling GLFW the window
    glfwMakeContextCurrent(window);

    // Load glad to config openGL
    gladLoadGL();
    // Viewport of OpenGL in the Window
    glViewport(0, 0, width, height);

    // Vertices coordinates just for vertex fragment
    GLfloat screenVertices[] =
    { //     COORDINATES     /        COLORS      /   TexCoord  //
        0.0f, 0.0f,  1.0f,     0.83f, 0.70f, 0.44f,	0.0f, 0.0f,
        0.0f, 0.0f, 0.0f,     0.83f, 0.70f, 0.44f,	5.0f, 0.0f,
         1.0f, 0.0f, 0.0f,     0.83f, 0.70f, 0.44f,	0.0f, 0.0f,
         1.0f, 0.0f,  1.0f,     0.83f, 0.70f, 0.44f,	5.0f, 0.0f,
         0.5f, 0.8f,  0.5f,     0.92f, 0.86f, 0.76f,	2.5f, 5.0f
    };

    // Indices for vertices order
    GLuint indices[] =
    {
        0, 1, 2,
        0, 2, 3,
        0, 1, 4,
        1, 2, 4,
        2, 3, 4,
        3, 0, 4

    };

    // Vertices coordinates
    GLfloat screenVertices2[] =
    { //     COORDINATES     /        COLORS      /   TexCoord  //
        -0.5f, 0.0f,  0.5f,     0.83f, 0.70f, 0.44f,	0.0f, 0.0f,
        -0.5f, 0.0f, -0.5f,     0.83f, 0.70f, 0.44f,	5.0f, 0.0f,
         0.5f, 0.0f, -0.5f,     0.83f, 0.70f, 0.44f,	0.0f, 0.0f,
         0.5f, 0.0f,  0.5f,     0.83f, 0.70f, 0.44f,	5.0f, 0.0f,
         0.0f, 0.8f,  0.0f,     0.92f, 0.86f, 0.76f,	2.5f, 5.0f
    };

    // Indices for vertices order
    GLuint indices2[] =
    {
        0, 1, 2,
        0, 2, 3,
        0, 1, 4,
        1, 2, 4,
        2, 3, 4,
        3, 0, 4

    };

    // Random points generator
    for (int i = 0; i < points * 2; i++)
    {
        vertices.push_back(distr(gen));
    }


    // Create shader program, vertex shader doesn't do anything
    Shader shaderProgram("shader.vs", "shader.fs");

    // Create VAO
    VAO VAO1;
    // Bind VAO as the current one
    VAO1.Bind();

    // Create VBO and EBO
    VBO VBO1(screenVertices, sizeof(screenVertices));
    EBO EBO1(indices, sizeof(indices));

    // Link VBO to VAO
    VAO1.LinkAttrib(VBO1, 0, 3, GL_FLOAT, 8 * sizeof(float), (void*)0);
    VAO1.LinkAttrib(VBO1, 1, 3, GL_FLOAT, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    VAO1.LinkAttrib(VBO1, 2, 2, GL_FLOAT, 8 * sizeof(float), (void*)(6 * sizeof(float)));

    // Unbind VAO, VBO, EBO 
    VAO1.Unbind();
    VBO1.Unbind();
    EBO1.Unbind();

    // Create VAO
    VAO VAO2;
    // Bind VAO as the current one
    VAO2.Bind();

    // Create VBO and EBO
    VBO VBO2(screenVertices2, sizeof(screenVertices2));
    EBO EBO2(indices2, sizeof(indices2));

    // Link VBO to VAO
    VAO2.LinkAttrib(VBO2, 0, 3, GL_FLOAT, 8 * sizeof(float), (void*)0);
    VAO2.LinkAttrib(VBO2, 1, 3, GL_FLOAT, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    VAO2.LinkAttrib(VBO2, 2, 2, GL_FLOAT, 8 * sizeof(float), (void*)(6 * sizeof(float)));

    // Unbind VAO, VBO, EBO 
    VAO2.Unbind();
    VBO2.Unbind();
    EBO2.Unbind();

    
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");


    // Linking uniforms
    GLfloat uniPoints = glGetUniformLocation(shaderProgram.ID, "points");
    GLuint uniLenght = glGetUniformLocation(shaderProgram.ID, "lenght");
    GLuint uniResolution = glGetUniformLocation(shaderProgram.ID, "resolution");

    // Creating and setting texture
    Texture owo("bed.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);
    owo.textUnit(shaderProgram, "tex0", 0);

    // Enable depth
    glEnable(GL_DEPTH_TEST);

    Camera camera(width, height, glm::vec3(0.0f, 0.0f, 2.0f));

    bool drawTriangle = true;
    float size = 0;

    // Main loop
    while (!glfwWindowShouldClose(window))
    {
        // Color Background
        glClearColor(0.102f, 0.0f, 0.051f, 1.0f);
        // Clean buffer and change the color
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        if (!io.WantCaptureMouse)
        {

        }
        

        // Tell OpenGL to use shader program
        shaderProgram.Activate();

        camera.Inputs(window);
        camera.Matrix(45.0f, 0.1f, 100.0f, shaderProgram, "camMatrix");


        // Pass uniform values
        glUniform1fv(uniPoints, points*2+1, &vertices[0]);
        glUniform1iv(uniResolution, 2, &resolution[0]);
        glUniform1i(uniLenght, points);
        glUniform1i(glGetUniformLocation(shaderProgram.ID, "palette"), palette);
        glUniform1i(glGetUniformLocation(shaderProgram.ID, "mode"), mode);
        glUniform1f(glGetUniformLocation(shaderProgram.ID, "scale"), size);

        // Use texture
        owo.Bind();

        // Tell OpenGL to use VAO
        VAO1.Bind();

        if (drawTriangle)
        {
            // Draw primitives
            glDrawElements(GL_TRIANGLES, sizeof(indices) / sizeof(int), GL_UNSIGNED_INT, 0);
        }

        VAO1.Unbind();

        VAO2.Bind();

        if (drawTriangle)
        {
            // Draw primitives
            glDrawElements(GL_TRIANGLES, sizeof(indices2) / sizeof(int), GL_UNSIGNED_INT, 0);
        }


        ImGui::Begin("ImGUI window");
        ImGui::Text("aiuda");
        ImGui::Checkbox("Draw Triangle", &drawTriangle);
        ImGui::SliderFloat("Size", &size, 0.5f, 2.0f);
        ImGui::End();

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());


        // Swap buffers 
        glfwSwapBuffers(window);

        // GLFW events
        glfwPollEvents();
    }

    // Delete VAO, VBO and EBO
    VAO1.Delete();
    VBO1.Delete();
    EBO1.Delete();

    // Delete VAO, VBO and EBO
    VAO2.Delete();
    VBO2.Delete();
    EBO2.Delete();

    // Delete shader program
    shaderProgram.Delete();


    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();


    // Delete texture
    owo.Delete();

    // Delete window
    glfwDestroyWindow(window);
    // Terminate GLFW
    glfwTerminate();
    return 0;
}


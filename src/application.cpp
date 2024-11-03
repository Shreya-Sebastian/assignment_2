//#include "Image.h"
#include "mesh.h"
#include "texture.h"
// Always include window first (because it includes glfw, which includes GL which needs to be included AFTER glew).
// Can't wait for modules to fix this stuff...
#include <framework/disable_all_warnings.h>
DISABLE_WARNINGS_PUSH()
#include <glad/glad.h>
// Include glad before glfw3
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/mat4x4.hpp>
#include <imgui/imgui.h>
DISABLE_WARNINGS_POP()
#include <framework/shader.h>
#include <framework/window.h>
#include <framework/image.h>
#include <functional>
#include <iostream>
#include <vector>


/*Mesh createSquareMesh() {
    Mesh squareMesh;

    // Define vertices with positions, normals, and texture coordinates.
    squareMesh.vertices = {
        {{-0.5f, -0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f}}, // Bottom-left
        {{ 0.5f, -0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}, {1.0f, 0.0f}}, // Bottom-right
        {{ 0.5f,  0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f}}, // Top-right
        {{-0.5f,  0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 1.0f}}  // Top-left
    };

    // Define two triangles to form the square.
    squareMesh.triangles = {
        {0, 1, 2}, // First triangle
        {0, 2, 3}  // Second triangle
    };

    return squareMesh;
}*/

Mesh createCubeMesh() {
    Mesh cubeMesh;

    // Define vertices for a cube with normals and texture coordinates.
    cubeMesh.vertices = {
        // Front face
        {{-0.5f, -0.5f,  0.5f}, {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f}}, // Bottom-left
        {{ 0.5f, -0.5f,  0.5f}, {0.0f, 0.0f, 1.0f}, {1.0f, 0.0f}}, // Bottom-right
        {{ 0.5f,  0.5f,  0.5f}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f}}, // Top-right
        {{-0.5f,  0.5f,  0.5f}, {0.0f, 0.0f, 1.0f}, {0.0f, 1.0f}}, // Top-left

        // Back face
        {{-0.5f, -0.5f, -0.5f}, {0.0f, 0.0f, -1.0f}, {0.0f, 0.0f}}, // Bottom-left
        {{ 0.5f, -0.5f, -0.5f}, {0.0f, 0.0f, -1.0f}, {1.0f, 0.0f}}, // Bottom-right
        {{ 0.5f,  0.5f, -0.5f}, {0.0f, 0.0f, -1.0f}, {1.0f, 1.0f}}, // Top-right
        {{-0.5f,  0.5f, -0.5f}, {0.0f, 0.0f, -1.0f}, {0.0f, 1.0f}}, // Top-left

        // Left face
        {{-0.5f, -0.5f, -0.5f}, {-1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}}, // Bottom-left
        {{-0.5f,  0.5f, -0.5f}, {-1.0f, 0.0f, 0.0f}, {1.0f, 0.0f}}, // Bottom-right
        {{-0.5f,  0.5f,  0.5f}, {-1.0f, 0.0f, 0.0f}, {1.0f, 1.0f}}, // Top-right
        {{-0.5f, -0.5f,  0.5f}, {-1.0f, 0.0f, 0.0f}, {0.0f, 1.0f}}, // Top-left

        // Right face
        {{ 0.5f, -0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}}, // Bottom-left
        {{ 0.5f,  0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}, {1.0f, 0.0f}}, // Bottom-right
        {{ 0.5f,  0.5f,  0.5f}, {1.0f, 0.0f, 0.0f}, {1.0f, 1.0f}}, // Top-right
        {{ 0.5f, -0.5f,  0.5f}, {1.0f, 0.0f, 0.0f}, {0.0f, 1.0f}}, // Top-left

        // Top face
        {{-0.5f,  0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f}}, // Bottom-left
        {{ 0.5f,  0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f}}, // Bottom-right
        {{ 0.5f,  0.5f,  0.5f}, {0.0f, 1.0f, 0.0f}, {1.0f, 1.0f}}, // Top-right
        {{-0.5f,  0.5f,  0.5f}, {0.0f, 1.0f, 0.0f}, {0.0f, 1.0f}}, // Top-left

        // Bottom face
        {{-0.5f, -0.5f, -0.5f}, {0.0f, -1.0f, 0.0f}, {0.0f, 0.0f}}, // Bottom-left
        {{ 0.5f, -0.5f, -0.5f}, {0.0f, -1.0f, 0.0f}, {1.0f, 0.0f}}, // Bottom-right
        {{ 0.5f, -0.5f,  0.5f}, {0.0f, -1.0f, 0.0f}, {1.0f, 1.0f}}, // Top-right
        {{-0.5f, -0.5f,  0.5f}, {0.0f, -1.0f, 0.0f}, {0.0f, 1.0f}}, // Top-left
    };

    // Define triangles using indices to the vertices above
    cubeMesh.triangles = {
        // Front face
        {0, 1, 2}, {0, 2, 3},
        // Back face
        {4, 5, 6}, {4, 6, 7},
        // Left face
        {8, 9, 10}, {8, 10, 11},
        // Right face
        {12, 13, 14}, {12, 14, 15},
        // Top face
        {16, 17, 18}, {16, 18, 19},
        // Bottom face
        {20, 21, 22}, {20, 22, 23}
    };

    return cubeMesh;
}

unsigned int loadTexture(const std::filesystem::path& path) {
    // Load the image
    Image image(path);

    unsigned int textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    // Set texture wrapping and filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Determine the image format based on channels
    GLenum format = (image.channels == 4) ? GL_RGBA : GL_RGB;

    // Load image data into the texture
    glTexImage2D(GL_TEXTURE_2D, 0, format, image.width, image.height, 0, format, GL_UNSIGNED_BYTE, image.get_data());
    glGenerateMipmap(GL_TEXTURE_2D);

    return textureID;
}

class Application {
public:
    Application()
        : m_window("Final Project", glm::ivec2(1024, 1024), OpenGLVersion::GL41)
        , m_texture(RESOURCE_ROOT "resources/checkerboard.png")
    {
        m_window.registerKeyCallback([this](int key, int scancode, int action, int mods) {
            if (action == GLFW_PRESS)
                onKeyPressed(key, mods);
            else if (action == GLFW_RELEASE)
                onKeyReleased(key, mods);
            });
        m_window.registerMouseMoveCallback(std::bind(&Application::onMouseMove, this, std::placeholders::_1));
        m_window.registerMouseButtonCallback([this](int button, int action, int mods) {
            if (action == GLFW_PRESS)
                onMouseClicked(button, mods);
            else if (action == GLFW_RELEASE)
                onMouseReleased(button, mods);
            });

        //m_meshes = GPUMesh::loadMeshGPU(RESOURCE_ROOT "resources/dragon.obj");

        // Create a square mesh and convert it to a GPUMesh
        /*Mesh cpuSquareMesh = createSquareMesh();
        GPUMesh gpuSquareMesh(cpuSquareMesh);

        // Add the square mesh to the list of meshes
        m_meshes.push_back(std::move(gpuSquareMesh));*/

        // Load the mars.png texture
        unsigned int m_textureID = loadTexture("resources/mars.png");
        if (m_textureID == 0) {
            std::cerr << "Failed to load texture!" << std::endl;
        }
        else {
            std::cout << "Texture loaded with ID: " << m_textureID << std::endl;
        }

        // Create a cube mesh and convert it to a GPUMesh
        /*Mesh cpuCubeMesh = createCubeMesh();
        GPUMesh gpuCubeMesh(cpuCubeMesh);

        // Add the cube mesh to the list of meshes
        m_meshes.push_back(std::move(gpuCubeMesh));*/

        GPUMesh cubeMesh1 = GPUMesh(createCubeMesh());  // First cube mesh
        GPUMesh cubeMesh2 = GPUMesh(createCubeMesh());  // Second cube mesh
        m_meshes.emplace_back(createCubeMesh());  // Construct first cube mesh in-place
        m_meshes.emplace_back(createCubeMesh());

        /*GPUMesh cubeMesh1 = GPUMesh(createCubeMesh());  // Create the cube mesh
        m_meshes.push_back(cubeMesh1);                  // Add first instance of the cube
        m_meshes.push_back(cubeMesh1);*/

        try {
            ShaderBuilder defaultBuilder;
            defaultBuilder.addStage(GL_VERTEX_SHADER, RESOURCE_ROOT "shaders/shader_vert.glsl");
            defaultBuilder.addStage(GL_FRAGMENT_SHADER, RESOURCE_ROOT "shaders/shader_frag.glsl");
            m_defaultShader = defaultBuilder.build();

            ShaderBuilder shadowBuilder;
            shadowBuilder.addStage(GL_VERTEX_SHADER, RESOURCE_ROOT "shaders/shadow_vert.glsl");
            shadowBuilder.addStage(GL_FRAGMENT_SHADER, RESOURCE_ROOT "Shaders/shadow_frag.glsl");
            m_shadowShader = shadowBuilder.build();

            // Any new shaders can be added below in similar fashion.
            // ==> Don't forget to reconfigure CMake when you do!
            //     Visual Studio: PROJECT => Generate Cache for ComputerGraphics
            //     VS Code: ctrl + shift + p => CMake: Configure => enter
            // ....
        }
        catch (ShaderLoadingException e) {
            std::cerr << e.what() << std::endl;
        }
    }

    void update()
    {
        float angle1 = 0.0f;
        float angle2 = 0.0f;
        float parentYPosition = 0.0f;
        float direction = 1.0f;
        int dummyInteger = 0; // Initialized to 0
        glm::vec3 parentColor (1.0, 1.0, 0.0);
        glm::vec3 childColor(0.0, 0.4, 1.0);

        while (!m_window.shouldClose()) {
            // This is your game loop
            // Put your real-time logic and rendering in here
            m_window.updateInput();

            angle1 += 0.001f;
            angle2 += 0.0025f;

            parentYPosition += 0.005f * direction;
            if (parentYPosition > 0.5f || parentYPosition < -0.5f) {
                direction *= -1.0f; // Reverse direction when it reaches a certain height
            }

            // Use ImGui for easy input/output of ints, floats, strings, etc...
            ImGui::Begin("Window");
            ImGui::InputInt("This is an integer input", &dummyInteger); // Use ImGui::DragInt or ImGui::DragFloat for larger range of numbers.
            ImGui::Text("Value is: %i", dummyInteger); // Use C printf formatting rules (%i is a signed integer)
            ImGui::Checkbox("Use material if no texture", &m_useMaterial);
            ImGui::End();

            // Clear the screen
            glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            // ...
            glEnable(GL_DEPTH_TEST);

            m_defaultShader.bind();

            // Parent Cube Transformation (Bigger Cube)
            glm::mat4 modelMatrixParent = glm::translate(glm::mat4(1.0f), glm::vec3(-0.3f, parentYPosition, 0.0f));
            modelMatrixParent = glm::scale(modelMatrixParent, glm::vec3(0.2f)); // Scale down the parent cube
            modelMatrixParent = glm::rotate(modelMatrixParent, angle1, glm::vec3(0.0f, 1.0f, 0.0f));

            glm::mat4 mvpMatrixParent = m_projectionMatrix * m_viewMatrix * modelMatrixParent;
            glm::mat3 normalModelMatrixParent = glm::inverseTranspose(glm::mat3(modelMatrixParent));

            // Pass uniforms for the parent cube to the shader
            glUniformMatrix4fv(m_defaultShader.getUniformLocation("mvpMatrix"), 1, GL_FALSE, glm::value_ptr(mvpMatrixParent));
            glUniformMatrix3fv(m_defaultShader.getUniformLocation("normalModelMatrix"), 1, GL_FALSE, glm::value_ptr(normalModelMatrixParent));
            glUniform3fv(m_defaultShader.getUniformLocation("color"), 1, glm::value_ptr(parentColor));

            // Bind texture if necessary
            if (m_meshes[0].hasTextureCoords()) {
                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_2D, m_textureID);
                glUniform1i(m_defaultShader.getUniformLocation("colorMap"), 0);
                glUniform1i(m_defaultShader.getUniformLocation("hasTexCoords"), GL_TRUE);
                glUniform1i(m_defaultShader.getUniformLocation("useMaterial"), GL_FALSE);
            }
            else {
                glUniform1i(m_defaultShader.getUniformLocation("hasTexCoords"), GL_FALSE);
                glUniform1i(m_defaultShader.getUniformLocation("useMaterial"), m_useMaterial);
            }

            // Draw the first cube
            m_meshes[0].draw(m_defaultShader);

            // Child Cube Transformation
            // Start with the parent transformation and add a relative transformation for the child
            glm::mat4 modelMatrixChild = modelMatrixParent; // Inherit the parent's transformation
            modelMatrixChild = glm::translate(modelMatrixChild, glm::vec3(1.5f, 0.0f, 0.0f)); // Offset to the right of the parent
            modelMatrixChild = glm::scale(modelMatrixChild, glm::vec3(0.2f)); // Scale down the child cube
            modelMatrixChild = glm::rotate(modelMatrixChild, angle2, glm::vec3(0.0f, 1.0f, 0.0f));

            glm::mat4 mvpMatrixChild = m_projectionMatrix * m_viewMatrix * modelMatrixChild;
            glm::mat3 normalModelMatrixChild = glm::inverseTranspose(glm::mat3(modelMatrixChild));

            // Pass uniforms for the child cube to the shader
            glUniformMatrix4fv(m_defaultShader.getUniformLocation("mvpMatrix"), 1, GL_FALSE, glm::value_ptr(mvpMatrixChild));
            glUniformMatrix3fv(m_defaultShader.getUniformLocation("normalModelMatrix"), 1, GL_FALSE, glm::value_ptr(normalModelMatrixChild));
            glUniform3fv(m_defaultShader.getUniformLocation("color"), 1, glm::value_ptr(childColor));

            // Bind texture if necessary
            if (m_meshes[1].hasTextureCoords()) {
                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_2D, m_textureID);
                glUniform1i(m_defaultShader.getUniformLocation("colorMap"), 0);
                glUniform1i(m_defaultShader.getUniformLocation("hasTexCoords"), GL_TRUE);
                glUniform1i(m_defaultShader.getUniformLocation("useMaterial"), GL_FALSE);
            }
            else {
                glUniform1i(m_defaultShader.getUniformLocation("hasTexCoords"), GL_FALSE);
                glUniform1i(m_defaultShader.getUniformLocation("useMaterial"), m_useMaterial);
            }

            // Draw the second cube
            m_meshes[1].draw(m_defaultShader);

            // Processes input and swaps the window buffer
            m_window.swapBuffers();
        }
    }

    // In here you can handle key presses
    // key - Integer that corresponds to numbers in https://www.glfw.org/docs/latest/group__keys.html
    // mods - Any modifier keys pressed, like shift or control
    void onKeyPressed(int key, int mods)
    {
        std::cout << "Key pressed: " << key << std::endl;
    }

    // In here you can handle key releases
    // key - Integer that corresponds to numbers in https://www.glfw.org/docs/latest/group__keys.html
    // mods - Any modifier keys pressed, like shift or control
    void onKeyReleased(int key, int mods)
    {
        std::cout << "Key released: " << key << std::endl;
    }

    // If the mouse is moved this function will be called with the x, y screen-coordinates of the mouse
    void onMouseMove(const glm::dvec2& cursorPos)
    {
        std::cout << "Mouse at position: " << cursorPos.x << " " << cursorPos.y << std::endl;
    }

    // If one of the mouse buttons is pressed this function will be called
    // button - Integer that corresponds to numbers in https://www.glfw.org/docs/latest/group__buttons.html
    // mods - Any modifier buttons pressed
    void onMouseClicked(int button, int mods)
    {
        std::cout << "Pressed mouse button: " << button << std::endl;
    }

    // If one of the mouse buttons is released this function will be called
    // button - Integer that corresponds to numbers in https://www.glfw.org/docs/latest/group__buttons.html
    // mods - Any modifier buttons pressed
    void onMouseReleased(int button, int mods)
    {
        std::cout << "Released mouse button: " << button << std::endl;
    }

private:
    Window m_window;

    //GPUMesh m_squareMesh;
    unsigned int m_textureID;


    // Shader for default rendering and for depth rendering
    Shader m_defaultShader;
    Shader m_shadowShader;

    std::vector<GPUMesh> m_meshes;
    Texture m_texture;
    bool m_useMaterial{ true };

    // Projection and view matrices for you to fill in and use
    glm::mat4 m_projectionMatrix = glm::perspective(glm::radians(80.0f), 1.0f, 0.1f, 30.0f);
    glm::mat4 m_viewMatrix = glm::lookAt(glm::vec3(-1, 1, -1), glm::vec3(0), glm::vec3(0, 1, 0));
    glm::mat4 m_modelMatrix{ 1.0f };
};

int main()
{
    Application app;
    app.update();

    return 0;
}

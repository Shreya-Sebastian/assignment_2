//#include "Image.h"
#include "mesh.h"
#include "framework/mesh.h"
#include "texture.h"
#include "camera.h"
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

/*Mesh createSkyboxMesh() {
    Mesh skyboxMesh;

    skyboxMesh.vertices = {
        // Back face
        {{-1.0f,  1.0f, -1.0f}, {}, {}}, // Top-left
        {{-1.0f, -1.0f, -1.0f}, {}, {}}, // Bottom-left
        {{ 1.0f, -1.0f, -1.0f}, {}, {}}, // Bottom-right
        {{ 1.0f,  1.0f, -1.0f}, {}, {}}, // Top-right

        // Front face
        {{-1.0f, -1.0f,  1.0f}, {}, {}}, // Bottom-left
        {{-1.0f,  1.0f,  1.0f}, {}, {}}, // Top-left
        {{ 1.0f,  1.0f,  1.0f}, {}, {}}, // Top-right
        {{ 1.0f, -1.0f,  1.0f}, {}, {}}, // Bottom-right

        // Left face
        {{-1.0f,  1.0f,  1.0f}, {}, {}}, // Top-left
        {{-1.0f,  1.0f, -1.0f}, {}, {}}, // Top-right
        {{-1.0f, -1.0f, -1.0f}, {}, {}}, // Bottom-right
        {{-1.0f, -1.0f,  1.0f}, {}, {}}, // Bottom-left

        // Right face
        {{ 1.0f,  1.0f,  1.0f}, {}, {}}, // Top-left
        {{ 1.0f, -1.0f, -1.0f}, {}, {}}, // Bottom-right
        {{ 1.0f,  1.0f, -1.0f}, {}, {}}, // Top-right
        {{ 1.0f, -1.0f,  1.0f}, {}, {}}, // Bottom-left

        // Top face
        {{-1.0f,  1.0f, -1.0f}, {}, {}}, // Top-left
        {{ 1.0f,  1.0f, -1.0f}, {}, {}}, // Top-right
        {{ 1.0f,  1.0f,  1.0f}, {}, {}}, // Bottom-right
        {{-1.0f,  1.0f,  1.0f}, {}, {}}, // Bottom-left

        // Bottom face
        {{-1.0f, -1.0f, -1.0f}, {}, {}}, // Bottom-left
        {{ 1.0f, -1.0f,  1.0f}, {}, {}}, // Bottom-right
        {{ 1.0f, -1.0f,  1.0f}, {}, {}}, // Top-right
        {{-1.0f, -1.0f,  1.0f}, {}, {}}, // Top-left
    };

    skyboxMesh.triangles = {
        // Back face
        {0, 1, 2}, {0, 2, 3},
        // Front face
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

    return skyboxMesh;
}*/

float skyboxVertices[] = {
    // Positions          
    -1.0f,  1.0f, -1.0f,  // 0: Top-left-front
    -1.0f, -1.0f, -1.0f,  // 1: Bottom-left-front
     1.0f, -1.0f, -1.0f,  // 2: Bottom-right-front
     1.0f,  1.0f, -1.0f,  // 3: Top-right-front
    -1.0f,  1.0f,  1.0f,  // 4: Top-left-back
    -1.0f, -1.0f,  1.0f,  // 5: Bottom-left-back
     1.0f, -1.0f,  1.0f,  // 6: Bottom-right-back
     1.0f,  1.0f,  1.0f   // 7: Top-right-back
};

unsigned int skyboxIndices[] = {
    // Front face
    0, 1, 2,
    2, 3, 0,
    // Back face
    4, 5, 6,
    6, 7, 4,
    // Left face
    4, 5, 1,
    1, 0, 4,
    // Right face
    3, 2, 6,
    6, 7, 3,
    // Top face
    4, 0, 3,
    3, 7, 4,
    // Bottom face
    1, 5, 6,
    6, 2, 1
};



class Application {
public:
    Application()
        : m_window("Final Project", glm::ivec2(1024, 1024), OpenGLVersion::GL41)
        , m_texture(RESOURCE_ROOT "resources/textures/wall.jpeg")
        , m_normalMap(RESOURCE_ROOT "resources/textures/normalmap.png")
         
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



        std::vector<std::string> faces = {
            "resources/px.png", "resources/nx.png", "resources/py.png", "resources/ny.png", "resources/pz.png", "resources/nz.png"
        };
        unsigned int cubemapTexture = loadCubemap(faces); // Function to load cubemap texture


        // Initialize your other resources (e.g., cube mesh)

        try {


            ShaderBuilder defaultBuilder;
            defaultBuilder.addStage(GL_VERTEX_SHADER, RESOURCE_ROOT "shaders/shader_vert.glsl");
            defaultBuilder.addStage(GL_FRAGMENT_SHADER, RESOURCE_ROOT "shaders/shader_frag.glsl");
            m_defaultShader = defaultBuilder.build();

            ShaderBuilder shadowBuilder;
            shadowBuilder.addStage(GL_VERTEX_SHADER, RESOURCE_ROOT "shaders/shadow_vert.glsl");
            shadowBuilder.addStage(GL_FRAGMENT_SHADER, RESOURCE_ROOT "Shaders/shadow_frag.glsl");
            m_shadowShader = shadowBuilder.build();

            // Load shaders
            ShaderBuilder reflectionBuilder;
            reflectionBuilder.addStage(GL_VERTEX_SHADER, RESOURCE_ROOT "shaders/reflection_vert.glsl");
            reflectionBuilder.addStage(GL_FRAGMENT_SHADER, RESOURCE_ROOT "shaders/reflection_frag.glsl");
            m_reflectionShader = reflectionBuilder.build();

            ShaderBuilder skyboxBuilder;
            skyboxBuilder.addStage(GL_VERTEX_SHADER, RESOURCE_ROOT "shaders/skybox_vert.glsl");
            skyboxBuilder.addStage(GL_FRAGMENT_SHADER, RESOURCE_ROOT "Shaders/skybox_frag.glsl");
            m_skyboxShader = skyboxBuilder.build();

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
        glm::vec3 childColor(0.0, 0.4, 1.0);
        glm::vec3 parentColor(1.0, 1.0, 0.0);

        // Set up the skybox VAO, VBO, and EBO
        unsigned int skyboxVAO, skyboxVBO, skyboxEBO;
        glGenVertexArrays(1, &skyboxVAO);
        glGenBuffers(1, &skyboxVBO);
        glGenBuffers(1, &skyboxEBO);

        glBindVertexArray(skyboxVAO);
        glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), skyboxVertices, GL_STATIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, skyboxEBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(skyboxIndices), skyboxIndices, GL_STATIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

        while (!m_window.shouldClose()) {
            // Update input and animations
            m_window.updateInput();

            //updateCameraPosition();

            angle1 += 0.001f;
            angle2 += 0.0025f;

            parentYPosition += 0.005f * direction;
            if (parentYPosition > 0.5f || parentYPosition < -0.5f) {
                direction *= -1.0f; // Reverse direction when it reaches a certain height
            }

            // ImGui controls
            ImGui::Begin("Window");
            ImGui::InputInt("This is an integer input", &dummyInteger);
            ImGui::Text("Value is: %i", dummyInteger);
            ImGui::Checkbox("Use material if no texture", &m_useMaterial);
            ImGui::End();


            m_timer += 0.1f;
            glm::mat4 Modelmat(1.f);
            m_modelMatrix = glm::rotate(Modelmat, glm::radians(m_timer), glm::vec3(1.f, 1.f, 0.f));


            // Clear the screen
            glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            glEnable(GL_DEPTH_TEST);
            //glDisable(GL_BLEND);

            m_defaultShader.bind();  // Use default shader for the child cube

            // Parent Cube Transformation (environment-mapped reflective cube)
            glm::mat4 modelMatrixParent = glm::translate(glm::mat4(1.0f), glm::vec3(-0.3f, parentYPosition, 0.0f));
            modelMatrixParent = glm::scale(modelMatrixParent, glm::vec3(0.2f));
            modelMatrixParent = glm::rotate(modelMatrixParent, angle1, glm::vec3(0.0f, 1.0f, 0.0f));

            glm::mat4 mvpMatrixParent = m_projectionMatrix * m_viewMatrix * modelMatrixParent;
            glm::mat3 normalModelMatrixParent = glm::inverseTranspose(glm::mat3(modelMatrixParent));

            // Set uniforms for child cube
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

            m_meshes[0].draw(m_defaultShader);

            //m_defaultShader.bind();

            glm::mat4 modelMatrixChild = modelMatrixParent; // Start with the parent transformation
            modelMatrixChild = glm::translate(modelMatrixChild, glm::vec3(1.5f, 0.0f, 0.0f)); // Offset to the right of the parent
            modelMatrixChild = glm::scale(modelMatrixChild, glm::vec3(0.2f));
            modelMatrixChild = glm::rotate(modelMatrixChild, angle2, glm::vec3(0.0f, 1.0f, 0.0f));

            glm::mat4 mvpMatrixChild = m_projectionMatrix * m_viewMatrix * modelMatrixChild;
            glm::mat3 normalModelMatrixChild = glm::inverseTranspose(glm::mat3(modelMatrixChild));

            // Set uniforms for child cube
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

            m_meshes[1].draw(m_defaultShader);

            glDepthFunc(GL_LEQUAL); // Ensure skybox depth is always 1.0

            // Use skybox shader
            m_skyboxShader.bind();

            // Modify the view matrix to ignore translation (keep rotation only)
            glm::mat4 viewMatrix = glm::mat4(glm::mat3(m_viewMatrix)); // Remove translation
            glUniformMatrix4fv(m_skyboxShader.getUniformLocation("view"), 1, GL_FALSE, glm::value_ptr(viewMatrix));
            glUniformMatrix4fv(m_skyboxShader.getUniformLocation("projection"), 1, GL_FALSE, glm::value_ptr(m_projectionMatrix));
            glUniformMatrix4fv(m_skyboxShader.getUniformLocation("model"), 1, GL_FALSE, glm::value_ptr(m_modelMatrix));

            // Bind the cubemap texture
            glBindVertexArray(skyboxVAO);
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);

            // Draw the skybox
            glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
            glBindVertexArray(0);

            // Reset depth function to default
            glDepthFunc(GL_LESS);


            m_window.swapBuffers();
        }
    }


    // In here you can handle key presses
    // key - Integer that corresponds to numbers in https://www.glfw.org/docs/latest/group__keys.html
    // mods - Any modifier keys pressed, like shift or control
    void onKeyPressed(int key, int mods)
    {
        std::cout << "Key pressed: " << key << std::endl;
        float cameraSpeed = 0.1f;

        if (key == GLFW_KEY_W) {
            camera.moveForward();
            m_modelMatrix = glm::translate(m_modelMatrix, glm::vec3(0.0f, 0.0f, cameraSpeed)); //moves object
        }


        if (key == GLFW_KEY_S) {
            camera.moveBack();
            m_modelMatrix = glm::translate(m_modelMatrix, glm::vec3(0.0f, 0.0f, -cameraSpeed)); //moves object
        }// Move backward
        if (key == GLFW_KEY_A) {
            camera.moveLeft();

            m_modelMatrix = glm::translate(m_modelMatrix, glm::vec3(-cameraSpeed, 0.0f, 0.0f)); //moves object
        }
        if (key == GLFW_KEY_D) {
            camera.moveRight();
            m_modelMatrix = glm::translate(m_modelMatrix, glm::vec3(cameraSpeed, 0.0f, 0.0f)); //moves object
        }

        if (key == GLFW_KEY_LEFT) {
            camera.rotate(glm::vec3(m_modelMatrix[3]), -10.0f, false);
        }
        if (key == GLFW_KEY_RIGHT) {
            camera.rotate(glm::vec3(m_modelMatrix[3]), 10.0f, false); // Rotate right
        }
        if (key == GLFW_KEY_UP) {
            camera.rotate(glm::vec3(m_modelMatrix[3]), 10.0f, true);
        }
        if (key == GLFW_KEY_DOWN) {
            camera.rotate(glm::vec3(m_modelMatrix[3]), -10.0f, true);

        }

        if (key == GLFW_KEY_L) { //top view
            camera.setTopView();
        }

        m_viewMatrix = camera.viewMatrix();
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

    unsigned int cubemapTexture;
    //GPUMesh m_squareMesh;
    unsigned int m_textureID;

    //GPUMesh m_skyboxMesh;
    //unsigned int m_cubemapTextureID;



    // Shader for default rendering and for depth rendering
    Shader m_defaultShader;
    Shader m_shadowShader;
    Shader m_reflectionShader;
    Shader m_skyboxShader;

    std::vector<GPUMesh> m_meshes;
    Texture m_texture;
    Texture m_normalMap;
    bool m_useMaterial{ true };

    glm::vec3 cameraTarget = glm::vec3(0);
    glm::vec3 cameraPos = glm::vec3(-1, 1, -1);
    Camera camera{ &m_window, cameraPos, cameraTarget };

    // Projection and view matrices for you to fill in and use
    glm::mat4 m_projectionMatrix = glm::perspective(glm::radians(80.0f), 1.0f, 0.1f, 30.0f);
    glm::mat4 m_viewMatrix = camera.viewMatrix();
    glm::mat4 m_modelMatrix{ 1.0f };


    glm::vec3 cameraPosition;

    unsigned int loadCubemap(const std::vector<std::string>& faces) {
        unsigned int textureID;
        glGenTextures(1, &textureID);
        glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

        int width, height, channels;
        for (unsigned int i = 0; i < faces.size(); i++) {
            Image image(faces[i]);
            if (!image.getPixels().empty()) {
                glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB,
                    image.width, image.height, 0, GL_RGB, GL_UNSIGNED_BYTE, image.getPixels().data());
            }
            else {
                std::cerr << "Failed to load cubemap texture at path: " << faces[i] << std::endl;
                throw std::runtime_error("Cubemap texture load failed");
            }
        }

        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

        return textureID;
    }

    void updateCameraPosition() {
        float cameraSpeed = 0.05f;  // Adjust the speed as needed

        glm::vec3 forward(0.0f, 0.0f, -1.0f);  // Forward direction
        glm::vec3 right(1.0f, 0.0f, 0.0f);     // Right direction
        glm::vec3 up(0.0f, 1.0f, 0.0f);        // Up direction

        if (m_window.isKeyPressed(GLFW_KEY_W)) {
            cameraPosition += cameraSpeed * glm::vec3(0.0f, 0.0f, -1.0f);  // Move forward
        }
        if (m_window.isKeyPressed(GLFW_KEY_S)) {
            cameraPosition += cameraSpeed * glm::vec3(0.0f, 0.0f, 1.0f);   // Move backward
        }
        if (m_window.isKeyPressed(GLFW_KEY_A)) {
            cameraPosition += cameraSpeed * glm::vec3(-1.0f, 0.0f, 0.0f);  // Move left
        }
        if (m_window.isKeyPressed(GLFW_KEY_D)) {
            cameraPosition += cameraSpeed * glm::vec3(1.0f, 0.0f, 0.0f);   // Move right
        }
        if (m_window.isKeyPressed(GLFW_KEY_SPACE)) {
            cameraPosition += cameraSpeed * glm::vec3(0.0f, 1.0f, 0.0f);   // Move up
        }
        if (m_window.isKeyPressed(GLFW_KEY_LEFT_SHIFT)) {
            cameraPosition += cameraSpeed * glm::vec3(0.0f, -1.0f, 0.0f);  // Move down
        }

        // Update view matrix
        m_viewMatrix = glm::lookAt(cameraPosition, cameraPosition + glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    }

    //TODO: remove scuffed timer
    float m_timer{ 0.f };
};

int main()
{
    Application app;
    app.update();

    return 0;
}
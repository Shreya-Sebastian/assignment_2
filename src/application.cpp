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
        , m_normalMap(RESOURCE_ROOT "resources/textures/fur_normalmap.jpg")
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
            std::cerr << "Failed to load mars texture!" << std::endl;
        }
        else {
            std::cout << "Mars texture loaded with ID: " << m_textureID << std::endl;
        }

        // Create a cube mesh and convert it to a GPUMesh
        /*Mesh cpuCubeMesh = createCubeMesh();
        GPUMesh gpuCubeMesh(cpuCubeMesh);

        // Add the cube mesh to the list of meshes
        m_meshes.push_back(std::move(gpuCubeMesh));*/

        GPUMesh cubeMesh1 = GPUMesh(createCubeMesh());  // First cube mesh
        GPUMesh cubeMesh2 = GPUMesh(createCubeMesh());  // Second cube mesh
        wolfMeshes = GPUMesh::loadMeshGPU(RESOURCE_ROOT "resources/wolf/Wolf_obj.obj");
        //for (auto& wolf : wolfMeshes) wolf.setNormalMap(RESOURCE_ROOT "resources/textures/fur_normalmap.jpg");

        m_meshes.emplace_back(createCubeMesh());  // Construct first cube mesh in-place
        m_meshes.emplace_back(createCubeMesh());

        /*GPUMesh cubeMesh1 = GPUMesh(createCubeMesh());  // Create the cube mesh
        m_meshes.push_back(cubeMesh1);                  // Add first instance of the cube
        m_meshes.push_back(cubeMesh1);*/


        // Defining the faces of the skybox
        std::vector<std::string> faces = {
            "resources/px.png", "resources/nx.png", "resources/py.png", "resources/ny.png", "resources/pz.png", "resources/nz.png"
        };
        unsigned int cubemapTexture = loadCubemap(faces); // Function to load cubemap texture

        // Defining the bezier curves
        arcLengths.resize(NUM_SAMPLES);
        bezierPoints.resize(NUM_SAMPLES);
        std::vector<glm::vec3> controlPoints = {
            startPoint1, controlPoint1, controlPoint2, endPoint1, // First curve
            startPoint2, controlPoint3, controlPoint4, endPoint2, // Second curve
            startPoint3, controlPoint5, controlPoint6, endPoint3, // Third curve
            startPoint4, controlPoint7, controlPoint8, endPoint4  // Fourth curve
        };
        generateArcLengthTable(controlPoints);


        // Load textures for animation
        std::vector<std::string> animationFrames = {
            "resources/aerial_grass_rock_diff_2k.jpg",
            "resources/aerial_rocks_02_diff_2k.jpg",
            "resources/rocky_terrain_diff_2k.jpg",
            "resources/rocky_terrain_02_diff_2k.jpg"
        };
        loadAnimationTextures(animationFrames);

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

            ShaderBuilder normalBuilder;
            normalBuilder.addStage(GL_VERTEX_SHADER, RESOURCE_ROOT "shaders/normal_vert.glsl");
            normalBuilder.addStage(GL_FRAGMENT_SHADER, RESOURCE_ROOT "shaders/normal_frag.glsl");
            m_normalShader = normalBuilder.build();

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
        m_modelMatrix_wolf_2 = glm::translate(m_modelMatrix_wolf_2, glm::vec3(0.5f, 0.0f, 0.5f));
        //glm::mat4 rotationMatrix = glm::rotate(m_modelMatrix_wolf, 5.0f, glm::vec3(0.0f,1.0f,0.0f));
        //glm::mat4 rotationMatrix = glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        //m_modelMatrix_wolf = rotationMatrix * m_modelMatrix_wolf;

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


        float lastFrameTime = static_cast<float>(glfwGetTime());
        const float positionTolerance = 0.01f;  // Tolerance for reaching the ends
        //float deltaTime = currentFrameTime - lastFrameTime;
        //lastFrameTime = currentFrameTime;
        //time += speed * deltaTime;  // deltaTime is the frame time
        //if (time > 1.0f) time -= 1.0f;


        while (!m_window.shouldClose()) {
            // Update input and animations
            //m_window.updateInput();

            //updateCameraPosition();

            // Time logic for the bezier curve moves
            float currentFrameTime = static_cast<float>(glfwGetTime());
            float deltaTime = currentFrameTime - lastFrameTime;
            lastFrameTime = currentFrameTime;
            time += speed * deltaTime;  // deltaTime is the frame time
            time = glm::mod(time, 1.0f);  // Keep time within [0, 1]

            // Time logic for the multiple texture loading
            m_timeAccumulator += deltaTime;
            if (m_timeAccumulator >= m_animationSpeed) {
                m_timeAccumulator = 0.0f;
                m_currentFrame = (m_currentFrame + 1) % m_animationTextures.size();
            }

            glm::vec3 bezierPosition = getConstantSpeedPosition(time);

            m_window.updateInput();
            angle1 += 0.001f;
            angle2 += 0.0025f;

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

            
            m_defaultShader.bind();
            glm::vec3 lightPos = glm::vec3(1.0f);

            
            for (GPUMesh& mesh : wolfMeshes) {
                const glm::mat4 mvpMatrix = m_projectionMatrix * m_viewMatrix * m_modelMatrix_wolf;
                glUniformMatrix4fv(m_defaultShader.getUniformLocation("mvpMatrix"), 1, GL_FALSE, glm::value_ptr(mvpMatrix));
                const glm::mat3 normalModelMatrix = glm::inverseTranspose(glm::mat3(m_modelMatrix_wolf));
                glUniformMatrix3fv(m_defaultShader.getUniformLocation("normalModelMatrix"), 1, GL_FALSE, glm::value_ptr(normalModelMatrix));
                glUniform3fv(m_defaultShader.getUniformLocation("color"), 1, glm::value_ptr(parentColor));
                glUniform3fv(m_defaultShader.getUniformLocation("lightPos"), 1, glm::value_ptr(lightPos));
                glUniform3fv(m_defaultShader.getUniformLocation("cameraPos"), 1, glm::value_ptr(camera.cameraPos()));
                glUniform1i(m_defaultShader.getUniformLocation("wolf"), true);
                glUniform1f(m_defaultShader.getUniformLocation("metallic"), 0.9);
                glUniform1f(m_defaultShader.getUniformLocation("roughness"), 0.6);

                mesh.draw(m_defaultShader);
            }

            m_normalShader.bind();

            for (GPUMesh& mesh : wolfMeshes) {
                const glm::mat4 mvpMatrix = m_projectionMatrix * m_viewMatrix * m_modelMatrix_wolf_2;
                glUniformMatrix4fv(m_normalShader.getUniformLocation("mvpMatrix"), 1, GL_FALSE, glm::value_ptr(mvpMatrix));
                const glm::mat3 normalModelMatrix = glm::inverseTranspose(glm::mat3(m_modelMatrix_wolf_2));
                glUniformMatrix3fv(m_normalShader.getUniformLocation("normalModelMatrix"), 1, GL_FALSE, glm::value_ptr(normalModelMatrix));
                glUniform3fv(m_normalShader.getUniformLocation("color"), 1, glm::value_ptr(glm::vec3(1.f, 1.0f, 1.0f)));
                glUniform3fv(m_normalShader.getUniformLocation("lightPos"), 1, glm::value_ptr(lightPos));
                glUniform3fv(m_normalShader.getUniformLocation("cameraPos"), 1, glm::value_ptr(camera.cameraPos()));
                glUniform1i(m_normalShader.getUniformLocation("wolf"), true);
                glUniform1f(m_normalShader.getUniformLocation("metallic"), 0.3);
                glUniform1f(m_normalShader.getUniformLocation("roughness"), 0.1);
                m_normalMap.bind(GL_TEXTURE1);
                glUniform1i(m_normalShader.getUniformLocation("normalMap"), 1);
                mesh.draw(m_normalShader);
            }


            // Print out the value of hasTexCoords for debugging
            std::cout << "hasTexCoords value: " << m_meshes[0].hasTextureCoords() << std::endl;

            // Parent Cube Transformation (environment-mapped reflective cube)
            glm::mat4 modelMatrixParent = glm::translate(glm::mat4(1.0f), bezierPosition);
            modelMatrixParent = glm::scale(modelMatrixParent, glm::vec3(0.2f));
            modelMatrixParent = glm::rotate(modelMatrixParent, angle1, glm::vec3(0.0f, 1.0f, 0.0f));

            // Switch to the reflection shader for the reflective cube
            m_reflectionShader.bind();


            // Set the MVP matrices
            glUniformMatrix4fv(m_reflectionShader.getUniformLocation("model"), 1, GL_FALSE, glm::value_ptr(modelMatrixParent));
            glUniformMatrix4fv(m_reflectionShader.getUniformLocation("view"), 1, GL_FALSE, glm::value_ptr(m_viewMatrix));
            glUniformMatrix4fv(m_reflectionShader.getUniformLocation("projection"), 1, GL_FALSE, glm::value_ptr(m_projectionMatrix));

            // Set the camera position for accurate reflection direction calculation
            glUniform3fv(m_reflectionShader.getUniformLocation("cameraPosition"), 1, glm::value_ptr(cameraPos));

            // Bind the cubemap texture to texture unit 0
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
            glUniform1i(m_reflectionShader.getUniformLocation("skybox"), 0); // Set the cubemap sampler uniform

            // Draw the parent (larger) cube with reflection
            m_meshes[0].draw(m_reflectionShader);

            m_defaultShader.bind();

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
            glUniform1i(m_defaultShader.getUniformLocation("wolf"), false);

            // Bind texture if necessary
            if (m_meshes[1].hasTextureCoords()) {
                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_2D, m_textureID);
                glUniform1i(m_defaultShader.getUniformLocation("colorMap"), 0);
                glUniform1i(m_defaultShader.getUniformLocation("hasTexCoords"), GL_TRUE);
                //glUniform1i(m_defaultShader.getUniformLocation("useMaterial"), GL_FALSE);
            }
            else {
                glUniform1i(m_defaultShader.getUniformLocation("hasTexCoords"), GL_FALSE);
                //glUniform1i(m_defaultShader.getUniformLocation("useMaterial"), m_useMaterial);
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
            m_modelMatrix_wolf = glm::translate(m_modelMatrix_wolf, camera.forward() * cameraSpeed); //moves object
        }
        if (key == GLFW_KEY_S) {
            camera.moveBack();
            m_modelMatrix_wolf = glm::translate(m_modelMatrix_wolf, -camera.forward() * cameraSpeed); //moves object
        }// Move backward
        if (key == GLFW_KEY_A) {
            camera.moveLeft();
            glm::vec3 right = glm::normalize(glm::cross(camera.forward(), glm::vec3(0,1,0) ));
            m_modelMatrix_wolf = glm::translate(m_modelMatrix_wolf, -right*cameraSpeed); //moves object
        }
        if (key == GLFW_KEY_D) {
            camera.moveRight();
            glm::vec3 right = glm::normalize(glm::cross(camera.forward(), glm::vec3(0, 1, 0)));
            m_modelMatrix_wolf = glm::translate(m_modelMatrix_wolf, right*cameraSpeed); //moves object
        }

        if (key == GLFW_KEY_LEFT) {
            camera.rotate(glm::vec3(m_modelMatrix[3]), -10.0f, false);
        }
        if (key == GLFW_KEY_RIGHT) {
            camera.rotate(glm::vec3(m_modelMatrix[3]), 10.0f, false); 
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
        
        if (mousePressed) {
            printf("Let's rotate!");
            glm::vec2 delta = 0.5f * glm::vec2(cursorPos - m_prevCursorPos);

            camera.rotate(glm::vec3(m_modelMatrix[3]), delta.x, false);
            camera.rotate(glm::vec3(m_modelMatrix[3]), delta.y, true);
            m_viewMatrix = camera.viewMatrix();
        }
        std::cout << "Mouse at position: " << cursorPos.x << " " << cursorPos.y << std::endl;
        m_prevCursorPos = cursorPos;
    }

    // If one of the mouse buttons is pressed this function will be called
    // button - Integer that corresponds to numbers in https://www.glfw.org/docs/latest/group__buttons.html
    // mods - Any modifier buttons pressed
    void onMouseClicked(int button, int mods)
    {

        if (button == GLFW_MOUSE_BUTTON_LEFT) {
            std::cout << "Pressed mouse button: " << button << std::endl;
            
            mousePressed = true;
            
        }
        
    }

    // If one of the mouse buttons is released this function will be called
    // button - Integer that corresponds to numbers in https://www.glfw.org/docs/latest/group__buttons.html
    // mods - Any modifier buttons pressed
    void onMouseReleased(int button, int mods)
    {
        std::cout << "Released mouse button: " << button << std::endl;
        mousePressed = false;
        
    }

    void generateArcLengthTable(const std::vector<glm::vec3>& controlPoints) {
        int curveCount = controlPoints.size() / 4; // Each curve has 4 control points
        float totalLength = 0.0f;

        // Resize arcLengths and bezierPoints to hold points for all curves
        arcLengths.resize(curveCount * NUM_SAMPLES);
        bezierPoints.resize(curveCount * NUM_SAMPLES);

        for (int curveIndex = 0; curveIndex < curveCount; ++curveIndex) {
            // Get control points for the current curve
            const glm::vec3& p0 = controlPoints[curveIndex * 4];
            const glm::vec3& p1 = controlPoints[curveIndex * 4 + 1];
            const glm::vec3& p2 = controlPoints[curveIndex * 4 + 2];
            const glm::vec3& p3 = controlPoints[curveIndex * 4 + 3];

            // Generate points and arc lengths for this curve
            arcLengths[curveIndex * NUM_SAMPLES] = totalLength;
            bezierPoints[curveIndex * NUM_SAMPLES] = p0;
            for (int i = 1; i < NUM_SAMPLES; i++) {
                float t = static_cast<float>(i) / (NUM_SAMPLES - 1);
                glm::vec3 point = evaluateBezier(p0, p1, p2, p3, t);
                bezierPoints[curveIndex * NUM_SAMPLES + i] = point;

                // Calculate segment length and update total length
                float segmentLength = glm::length(point - bezierPoints[curveIndex * NUM_SAMPLES + i - 1]);
                totalLength += segmentLength;
                arcLengths[curveIndex * NUM_SAMPLES + i] = totalLength;
            }
        }

        // Normalize arc length values to [0, 1] across all curves
        for (float& length : arcLengths) {
            length /= totalLength;
        }
    }


    glm::vec3 getConstantSpeedPosition(float t) {
        // Find the segment where this t corresponds in arc length space
        int segment = 0;
        while (segment < arcLengths.size() - 1 && arcLengths[segment] < t) {
            segment++;
        }

        // Boundary cases: return start or end position if at extremes
        if (segment == 0) {
            return bezierPoints[0];
        }
        else if (segment >= arcLengths.size()) {
            return bezierPoints[arcLengths.size() - 1];
        }

        // Calculate local t within the segment
        float segmentT = (t - arcLengths[segment - 1]) / (arcLengths[segment] - arcLengths[segment - 1]);
        return glm::mix(bezierPoints[segment - 1], bezierPoints[segment], segmentT);
    }

    glm::vec3 evaluateBezier(const glm::vec3& p0, const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& p3, float t) {
        float u = 1.0f - t;
        return u * u * u * p0 + 3 * u * u * t * p1 + 3 * u * t * t * p2 + t * t * t * p3;
    }

    // Function to load animation textures
    void loadAnimationTextures(const std::vector<std::string>& framePaths) {
        for (const auto& path : framePaths) {
            unsigned int textureID = loadTexture(path);
            if (textureID != 0) {
                m_animationTextures.push_back(textureID);
                std::cout << "Loaded texture: " << path << " with ID: " << textureID << std::endl;
            }
            else {
                std::cerr << "Failed to load texture: " << path << std::endl;
            }
        }
    }

    // Function to load a texture with ID
    unsigned int loadTexture(const std::filesystem::path& path) {
        Image image(path);
        unsigned int textureID;
        glGenTextures(1, &textureID);
        glBindTexture(GL_TEXTURE_2D, textureID);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        GLenum format = (image.channels == 4) ? GL_RGBA : GL_RGB;
        glTexImage2D(GL_TEXTURE_2D, 0, format, image.width, image.height, 0, format, GL_UNSIGNED_BYTE, image.get_data());
        glGenerateMipmap(GL_TEXTURE_2D);

        return textureID;
    }

private:
    Window m_window;

    unsigned int cubemapTexture;
    //GPUMesh m_squareMesh;
    unsigned int m_textureID;

    //GPUMesh m_skyboxMesh;
    //unsigned int m_cubemapTextureID;

    const int NUM_SAMPLES = 100;

    // Shader for default rendering and for depth rendering
    Shader m_defaultShader;
    Shader m_shadowShader;
    Shader m_reflectionShader;
    Shader m_skyboxShader;
    Shader m_normalShader;

    std::vector<GPUMesh> m_meshes;
    std::vector<GPUMesh> wolfMeshes;
    Texture m_texture;
    Texture m_normalMap;
    bool m_useMaterial{ true };
    glm::dvec2 m_prevCursorPos;



    // Projection and view matrices for you to fill in and use
    glm::mat4 m_projectionMatrix = glm::perspective(glm::radians(80.0f), 1.0f, 0.1f, 30.0f);
    glm::mat4 m_modelMatrix{ 1.0f };
    glm::mat4 m_modelMatrix_wolf{ 1.0f };
    glm::mat4 m_modelMatrix_wolf_2{ 1.0f };

    glm::vec3 cameraTarget = glm::vec3(0.0f);
    glm::vec3 cameraPos = glm::vec3(-1, 1, -1);
    Camera camera{ &m_window, cameraPos, cameraTarget };
    glm::mat4 m_viewMatrix = camera.viewMatrix();
    bool mousePressed = false;


    glm::vec3 cameraPosition;

    // Points for the bezier curve
    // Side 1: From left to top
    glm::vec3 startPoint1{ -1.0f, 0.0f, -1.0f };
    glm::vec3 controlPoint1{ -1.0f, 1.0f, -1.0f };
    glm::vec3 controlPoint2{ 0.0f, 1.0f, -1.0f };
    glm::vec3 endPoint1{ 1.0f, 0.0f, -1.0f };

    // Side 2: From top to right
    glm::vec3 startPoint2{ 1.0f, 0.0f, -1.0f };
    glm::vec3 controlPoint3{ 1.0f, 1.0f, 0.0f };
    glm::vec3 controlPoint4{ 1.0f, 1.0f, 1.0f };
    glm::vec3 endPoint2{ 1.0f, 0.0f, 1.0f };

    // Side 3: From right to bottom
    glm::vec3 startPoint3{ 1.0f, 0.0f, 1.0f };
    glm::vec3 controlPoint5{ 1.0f, -1.0f, 1.0f };
    glm::vec3 controlPoint6{ 0.0f, -1.0f, 1.0f };
    glm::vec3 endPoint3{ -1.0f, 0.0f, 1.0f };

    // Side 4: From bottom to left
    glm::vec3 startPoint4{ -1.0f, 0.0f, 1.0f };
    glm::vec3 controlPoint7{ -1.0f, -1.0f, 0.0f };
    glm::vec3 controlPoint8{ -1.0f, -1.0f, -1.0f };
    glm::vec3 endPoint4{ -1.0f, 0.0f, -1.0f };  // Same as the original startPoint to close the loop


    // For the constant move of the cube through the bezier curve
    float time = 0.0f;
    float speed = 0.1f;  // Adjust for desired speed
    float lastFrameTime = 0.0f;

    std::vector<float> arcLengths;
    std::vector<glm::vec3> bezierPoints;

    std::vector<unsigned int> m_animationTextures;

    // Animation parameters
    float m_animationSpeed;    // Time per frame
    int m_currentFrame;        // Current frame index
    float m_timeAccumulator;   // Time accumulator for frame switching

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
    float m_timer{ 0.f };
};

int main()
{
    Application app;
    app.update();

    return 0;
}
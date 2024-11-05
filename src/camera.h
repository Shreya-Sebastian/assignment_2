// Suppress warnings in third-party code.
#include <framework/disable_all_warnings.h>
DISABLE_WARNINGS_PUSH()
#include <glm/mat4x4.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
DISABLE_WARNINGS_POP()
#include <framework/window.h>

class Camera {
public:

    Camera(Window* pWindow, const glm::vec3& position, const glm::vec3& forward);

    //movement
    void moveForward();
    void moveLeft();
    void moveRight();
    void moveBack();
    void rotate(const glm::vec3& objectPosition, float degrees, bool up);

    //change settings
    void setFollowCharacter();
    void setTopView();
    void setUserInteraction(bool enabled);

    //getters
    glm::vec3 cameraPos() const;
    glm::mat4 viewMatrix() const;

private:
    void rotateX(float angle);
    void rotateY(float angle);

private:
    static constexpr glm::vec3 s_yAxis{ 0, 1, 0 };
    glm::vec3 m_position{ 0 };
    glm::vec3 m_target{ 1 };
    glm::vec3 m_up{ 0, 1, 0 };

    const Window* m_pWindow;
    bool m_userInteraction{ true };
    bool m_topView{ false };
    bool m_followCharacter{ true };
    glm::dvec2 m_prevCursorPos{ 0 };
    float cameraSpeed{ 0.1f };
};

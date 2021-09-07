#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <learnopengl/filesystem.h>
#include <learnopengl/shader_m.h>
#include <learnopengl/camera.h>

#include <iostream>

//#include "vertices.h"
#include "my_shaders.h"


void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);
int glfw_init_all(GLFWwindow*& window);
void set_point_light(Shader& shader, int i, glm::vec3 position, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular,
    float constant, float linear, float quadratic);
unsigned int config_depth_map();
void rend_scene(glm::vec3 cam_pos, glm::mat4 model, glm::mat4 projection, glm::mat4 view, std::vector<glm::vec3>Positions);
/*template <typename T>

char* formal_str(char* form, T i) {
    char str3[1024];
    sprintf(str3, "pointLights[%d].position", i);
    return str3;
}
*/

// settings
const unsigned int SCR_WIDTH = 1200;
const unsigned int SCR_HEIGHT = 800;

// camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

// lighting
glm::vec3 lightPos(1.2f, 1.0f, 2.0f);

SphereRender sphere_render;
CubeRender cube_render;
CylinderRender cylinder_render;
CubeRender_tex cube_tex_render;
QuadRender quad_render;
SkyboxRender skybox_render;
SphereRender glass_ball_render;
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
void processInput(GLFWwindow *window);
int glfw_init_all(GLFWwindow* &window);
void set_point_light(Shader& shader, int i, glm::vec3 position, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular,
    float constant, float linear, float quadratic);
void config_depth_map(unsigned int& depthMapFBO,unsigned int& depthMap);

void rend_scene();
void rend_scene(Shader new_shader);
/*template <typename T>

char* formal_str(char* form, T i) {
    char str3[1024];
    sprintf(str3, "pointLights[%d].position", i);
    return str3;
}
*/
typedef GLbyte* bytePt;

// settings
const unsigned int SCR_WIDTH = 1024;
const unsigned int SCR_HEIGHT = 1024;

// camera
Camera camera(glm::vec3(0.0f, 5.0f, 3.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

// lighting
#define NR_POINT_LIGHTS 2
unsigned int shadowmap_id[2] = { 5,6 };
glm::vec3 pointLightPositions[] = {
         glm::vec3(2.3f, 7.3f, -3.0f),
       // glm::vec3(2.3f, 7.3f, -3.0f),
                glm::vec3(0.7f,  8.2f,  2.0f),

};

glm::vec3 Positions[] = {
    glm::vec3(0.0f,  2.0f,  0.0f),
    glm::vec3(3.0f,  3.0f, 2.0f),
    glm::vec3(-1.5f, 0.51f, -2.5f),   //cylinder
    glm::vec3(2.0f,  2.0f,  1.0f),
    glm::vec3(-1.0f,  -2.5f, -1.0f),   
    glm::vec3(-1.5f, 1.2f, 1.5f),   // cone 
};
glm::vec3 GlassBallPositions[] = {
        glm::vec3(0.0f,  4.0f,  1.0f),
};
glm::mat4 projection;
glm::mat4 view;





void set_depth_shader(glm::vec3 &LightPos, Shader &simpleDepthShader) {
    glm::mat4 lightProjection, lightView;
    glm::mat4 lightSpaceMatrix;
    float near_plane = 1.0f, far_plane = 7.5f;
    //lightProjection = glm::perspective(glm::radians(45.0f), (GLfloat)SHADOW_WIDTH / (GLfloat)SHADOW_HEIGHT, near_plane, far_plane); // note that if you use a perspective projection matrix you'll have to change the light position as the current light position isn't enough to reflect the whole scene
    lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);
    lightView = glm::lookAt(LightPos, glm::vec3(0.0f), glm::vec3(0.0, 1.0, 0.0));
    lightSpaceMatrix = lightProjection * lightView;
    simpleDepthShader.setMat4("lightSpaceMatrix", lightSpaceMatrix);
}

int arrLen = SCR_WIDTH * SCR_HEIGHT * 3;
GLbyte* colorArr = new GLbyte[arrLen];
void saveColorData(bytePt& _pt, std::string& _str) {
    FILE* pFile = NULL;
    pFile = fopen(_str.c_str(), "wt");
    if (!pFile) { fprintf(stderr, "error \n"); exit(-1); }

    for (int i = 0; i < SCR_WIDTH * SCR_HEIGHT * 3; i++) {
        if (colorArr[i] == -1) { colorArr[i] = 255; }
    }
    for (int i = 0; i < SCR_WIDTH * SCR_HEIGHT * 3; i++) {
        fprintf(pFile, "%d\n", colorArr[i]);
    }
    fclose(pFile);
    printf("color data saved! \n");
}

void config_depth_map(unsigned int&depthMapFBO, unsigned int&depthMap) {
    const unsigned int SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;

    glGenFramebuffers(1, &depthMapFBO);
    // create depth texture
    glGenTextures(1, &depthMap);
    glBindTexture(GL_TEXTURE_2D, depthMap);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    float borderColor[] = { 1.0, 1.0, 1.0, 1.0 };
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
    // attach depth texture as FBO's depth buffer
    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void config_scene_cube_map(unsigned int& sceneMapFBO, unsigned int& sceneCubeMap) {
    const unsigned int Scene_WIDTH = 1024, Scene_HEIGHT = 1024;

    glGenFramebuffers(1, &sceneMapFBO);
    // create depth texture
    glGenTextures(1, &sceneCubeMap);
    glBindTexture(GL_TEXTURE_CUBE_MAP, sceneCubeMap);
    for (GLuint i = 0; i < 6; ++i) {
        //glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB,
            SCR_WIDTH, SCR_HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    }

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);    float borderColor[] = { 1.0, 1.0, 1.0, 1.0 };
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
    // attach depth texture as FBO's depth buffer
    glBindFramebuffer(GL_FRAMEBUFFER, sceneMapFBO);
    glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, sceneCubeMap, 0);
   // glDrawBuffer(GL_NONE);
     glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}


void renderQuad();
int main()
{
    GLFWwindow* window;
    int rf = glfw_init_all(window);
    if (rf == -1) return rf;
    else { std::cout << "Init glfw ok!"; }
    // configure global opengl state
    // -----------------------------
    glEnable(GL_DEPTH_TEST);

    // build and compile our shader zprogram


    // set up vertex data (and buffer(s)) and configure vertex attributes
    
    const unsigned int SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;
    unsigned int depthMapFBO[NR_POINT_LIGHTS];
    unsigned int depthMap[NR_POINT_LIGHTS];

    unsigned int sceneCubemap, sceneCubeFBO;

    config_scene_cube_map(sceneCubeFBO,  sceneCubemap);

    config_depth_map(depthMapFBO[0], depthMap[0]);
    config_depth_map(depthMapFBO[1], depthMap[1]);



    //printf("Config depth map ok with %d, %d\n ", depthMap, depthMapFBO);
    SphereRender sphere_render("basic_with_color.vs", "light_objs_with_color.fs");
    CubeRender cube_render("basic_with_color.vs", "light_objs_with_color.fs");
    CylinderRender cylinder_render("basic_with_color.vs", "light_objs_with_color.fs");
    ConeRender cone_render("basic_with_color.vs", "light_objs_with_color.fs");
    CubeRender_tex cube_tex_render("basic_with_texture.vs", "light_objs_with_texture.fs");
    QuadRender quad_render("basic_with_texture.vs", "light_objs_with_texture.fs");
    LightRender light_render("sphere");
    SkyboxRender skybox_render("skybox.vs", "skybox.fs");
    GlassSphereRender glass_render("basic_with_color.vs", "light_objs_color_ref.fs");

    Shader simpleDepthShader("depth_shader.vs", "depth_shader.fs");
  //  Shader debugDepthQuad("3.1.3.debug_quad.vs", "3.1.3.debug_quad_depth.fs");




    //
    sphere_render.build_vertices(glm::vec4(0.5f, 1.0f, 1.0f, 1.0f));
    sphere_render.set_shininess(32.0f);
    sphere_render.set_shadowmap(shadowmap_id, depthMap);

    cylinder_render.build_vertices(glm::vec4(1.0f, 0.2f, 1.0f, 1.0f));
    cylinder_render.set_shininess(32.0f);
    cylinder_render.set_shadowmap(shadowmap_id, depthMap);

    cube_render.build_vertices(glm::vec4(1.0f, 1.0f, 0.5f, 1.0f));
    cube_render.set_shininess(32.0f);
    cube_render.set_shadowmap(shadowmap_id, depthMap);

    cone_render.build_vertices(glm::vec4(0.3f, 0.9f, 0.5f, 1.0f));
    cone_render.set_shininess(32.0f);
    cone_render.set_shadowmap(shadowmap_id, depthMap);

    cube_tex_render.build_vertices();
    cube_tex_render.set_shininess(32.0f);
    cube_tex_render.set_diffuseMap("resources/textures/container2.png", 3);
    cube_tex_render.set_specularMap("resources/textures/container2_specular.png", 4);
    cube_tex_render.set_shadowmap(shadowmap_id,  depthMap);

    quad_render.build_vertices();
    quad_render.set_shininess(32.0f);
    quad_render.set_diffuseMap("resources/textures/wood.png", 1);
    quad_render.set_specularMap("resources/textures/wood.png", 1);
    quad_render.set_shadowmap(shadowmap_id, depthMap);

    skybox_render.build_vertices();
    skybox_render.set_cubeMap("resources/textures/skybox", 0);


    glass_render.build_vertices(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
    glass_render.set_shininess(32.0f);
    glass_render.set_shadowmap(shadowmap_id, depthMap);
    glass_render.shader.use();
    glass_render.shader.setInt("cubeTexture", 7);
    glActiveTexture(GL_TEXTURE7);
    glBindTexture(GL_TEXTURE_CUBE_MAP, sceneCubemap);


    light_render.build_vertices(glm::vec4(1.0, 1.0, 1.0, 1.0));

    //render_init();
    printf("render init ok\n");
    //cube_render.set_shininess(32.0f);
    //CubeRender cube_render("basic_with_color.vs", "light_objs_with_color.fs");
    float constant = 1.0f;
    float linear = 0.09;
    float quadratic = 0.032;
    glm::vec3 ambient(0.1f, 0.1f, 0.1f);
    glm::vec3 diffuse(0.8f, 0.8f, 0.8f);
    glm::vec3 specular(1.0f, 1.0f, 1.0f);
    for (int i = 0; i < NR_POINT_LIGHTS; i++) {
        set_point_light(sphere_render.shader, i, pointLightPositions[i], ambient, diffuse, specular,
            constant, linear, quadratic);
        set_point_light(cube_render.shader, i, pointLightPositions[i], ambient, diffuse, specular,
           constant, linear, quadratic);
        set_point_light(cone_render.shader, i, pointLightPositions[i], ambient, diffuse, specular,
            constant, linear, quadratic);
        set_point_light(cylinder_render.shader, i, pointLightPositions[i], ambient, diffuse, specular,
            constant, linear, quadratic);
        set_point_light(glass_render.shader, i, pointLightPositions[i], ambient, diffuse, specular,
            constant, linear, quadratic);
        set_point_light(quad_render.shader, i, pointLightPositions[i], ambient, diffuse, specular,
            constant, linear, quadratic);
        set_point_light(cube_tex_render.shader, i, pointLightPositions[i], ambient, diffuse, specular,
            constant, linear, quadratic);
    } 
    //为glass ball生成六个视角矩阵
    float near_plane = 1.0f;
    float far_plane = 25.0f;
    glm::mat4 sceneProj = glm::perspective(glm::radians(90.0f), (float)SHADOW_WIDTH / (float)SHADOW_HEIGHT, near_plane, far_plane);
    std::vector<glm::mat4> sceneTransforms;
    sceneTransforms.push_back(glm::lookAt(GlassBallPositions[0], GlassBallPositions[0] + glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
    sceneTransforms.push_back(glm::lookAt(GlassBallPositions[0], GlassBallPositions[0] + glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
    sceneTransforms.push_back(glm::lookAt(GlassBallPositions[0], GlassBallPositions[0] + glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f)));
    sceneTransforms.push_back(glm::lookAt(GlassBallPositions[0], GlassBallPositions[0] + glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f)));
    sceneTransforms.push_back(glm::lookAt(GlassBallPositions[0], GlassBallPositions[0] + glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
    sceneTransforms.push_back(glm::lookAt(GlassBallPositions[0], GlassBallPositions[0] + glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, -1.0f, 0.0f)));

    // 1. render scene to glass cubemap   //由于是静态场景我们不更新texture
// --------------------------------
    glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
    glBindFramebuffer(GL_FRAMEBUFFER, sceneCubeFBO);
    glClear(GL_DEPTH_BUFFER_BIT);


    glm::mat4 lightProjection, lightView;
    glm::mat4 lightSpaceMatrix;
    glm::mat4 model;

    //先算一遍阴影
    for (int dx = 0; dx < NR_POINT_LIGHTS; dx++) {
        lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);
        lightProjection = glm::perspective(glm::radians(90.0f), (GLfloat)SHADOW_WIDTH / (GLfloat)SHADOW_HEIGHT, near_plane, far_plane); // note that if you use a perspective projection matrix you'll have to change the light position as the current light position isn't enough to reflect the whole scene
        lightView = glm::lookAt(pointLightPositions[dx], glm::vec3(0.0f), glm::vec3(0.0, 1.0, 0.0));
        lightSpaceMatrix = lightProjection * lightView;
        simpleDepthShader.use();
        simpleDepthShader.setMat4("lightSpaceMatrix", lightSpaceMatrix);

        glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
        glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO[dx]);
        glClear(GL_DEPTH_BUFFER_BIT);
        //glCullFace(GL_FRONT);
        for (unsigned int i = 1; i < 2; i++) {
            model = glm::mat4(1.0f);
            model = glm::translate(model, Positions[i]);
            sphere_render.render2(simpleDepthShader, camera.Position, model, projection, view);
        }
        for (unsigned int i = 0; i < 1; i++) {
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, Positions[i]);
            cube_render.render2(simpleDepthShader, camera.Position, model, projection, view);
        }
        for (unsigned int i = 2; i < 3; i++) {
            model = glm::mat4(1.0f);
            model = glm::translate(model, Positions[i]);
            cylinder_render.render2(simpleDepthShader, camera.Position, model, projection, view);
        }
        model = glm::mat4(1.0f);
        model = glm::translate(model, Positions[5]);
        cone_render.render2(simpleDepthShader, camera.Position, model, projection, view);

        model = glm::mat4(1.0f);
        model = glm::translate(model, Positions[3]);
        cube_tex_render.render2(simpleDepthShader, camera.Position, model, projection, view);

        model = glm::mat4(1.0f);
        model = glm::translate(model, Positions[4]);
        quad_render.render2(simpleDepthShader, camera.Position, model, projection, view);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    //再正常渲染一次生成一次cube map给glass ball
    for (int i = 0; i < 6; i++)
    {
        projection = sceneProj;
        view = sceneTransforms[i];
        GLuint face = GL_TEXTURE_CUBE_MAP_POSITIVE_X + i;
        glBindFramebuffer(GL_FRAMEBUFFER, sceneCubeFBO);

        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, face, sceneCubemap, 0);

        glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        for (unsigned int i = 1; i < 2; i++) {
            model = glm::mat4(1.0f);
            model = glm::translate(model, Positions[i]);
            sphere_render.render(GlassBallPositions[0], model, projection, view);
        }
        for (unsigned int i = 0; i < 1; i++) {
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, Positions[i]);
            cube_render.render(GlassBallPositions[0], model, projection, view);
        }
        for (unsigned int i = 2; i < 3; i++) {
            model = glm::mat4(1.0f);
            model = glm::translate(model, Positions[i]);
            cylinder_render.render(GlassBallPositions[0], model, projection, view);
        }
        model = glm::mat4(1.0f);
        model = glm::translate(model, Positions[3]);
        cube_tex_render.render(GlassBallPositions[0], model, projection, view);

        model = glm::mat4(1.0f);
        model = glm::translate(model, Positions[5]);
        cone_render.render(GlassBallPositions[0], model, projection, view);

        model = glm::mat4(1.0f);
        //model = glm::translate(model, Positions[4]);
        quad_render.render(GlassBallPositions[0], model, projection, view);
        for (int i = 0; i < NR_POINT_LIGHTS; i++) {
            model = glm::mat4(1.0f);
            model = glm::translate(model, pointLightPositions[i]);
            light_render.render(camera.Position, model, projection, view);
        }

        model = glm::mat4(1.0f);
        model = glm::scale(model, glm::vec3(1.0f)); // Make it a smaller cube
        //skybox_render.render(model, projection, view);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        // BindViewMatrix(lightViewMatrices[i]);
        //RenderScene();
    }



    std::cout << "set point light ok" << std::endl;
    //debugDepthQuad.use();
  //debugDepthQuad.setInt("depthMap", 0);
    //render loop
    while(!glfwWindowShouldClose(window))
    {
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // input
        // -----
        processInput(window);

        // render
        // ------
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glm::mat4 model;
        projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        view = camera.GetViewMatrix();

        
        
        for (int i = 0; i < NR_POINT_LIGHTS; i++){
            lightProjection = glm::perspective(glm::radians(90.0f), (GLfloat)SHADOW_WIDTH / (GLfloat)SHADOW_HEIGHT, near_plane, far_plane); // note that if you use a perspective projection matrix you'll have to change the light position as the current light position isn't enough to reflect the whole scene
            //lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);
            lightView = glm::lookAt(pointLightPositions[i], glm::vec3(0.0f), glm::vec3(0.0, 1.0, 0.0));
            lightSpaceMatrix = lightProjection * lightView;
            sphere_render.set_light_matrix(lightSpaceMatrix, i);
            cube_render.set_light_matrix(lightSpaceMatrix, i);
            cylinder_render.set_light_matrix(lightSpaceMatrix, i);
            cube_tex_render.set_light_matrix(lightSpaceMatrix, i);
            quad_render.set_light_matrix(lightSpaceMatrix, i);
            glass_render.set_light_matrix(lightSpaceMatrix, i);
            cone_render.set_light_matrix(lightSpaceMatrix, i);

        }

        //set_depth_shader(pointLightPositions[0], simpleDepthShader);
        //glUniformMatrix4fv(lightSpaceMatrixLocation, 1, GL_FALSE, glm::value_ptr(lightSpaceMatrix));
        for (int dx = 0; dx < NR_POINT_LIGHTS; dx++) {
            lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);
            lightProjection = glm::perspective(glm::radians(90.0f), (GLfloat)SHADOW_WIDTH / (GLfloat)SHADOW_HEIGHT, near_plane, far_plane); // note that if you use a perspective projection matrix you'll have to change the light position as the current light position isn't enough to reflect the whole scene
            lightView = glm::lookAt(pointLightPositions[dx], glm::vec3(0.0f), glm::vec3(0.0, 1.0, 0.0));
            lightSpaceMatrix = lightProjection * lightView;
            simpleDepthShader.use();
            simpleDepthShader.setMat4("lightSpaceMatrix", lightSpaceMatrix);

            glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
            glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO[dx]);
            glClear(GL_DEPTH_BUFFER_BIT);
            //glCullFace(GL_FRONT);
            model = glm::mat4(1.0f);
            model = glm::translate(model, GlassBallPositions[0]);
            glass_render.render2(simpleDepthShader, camera.Position, model, projection, view);

            for (unsigned int i = 1; i < 2; i++) {
                model = glm::mat4(1.0f);
                model = glm::translate(model, Positions[i]);
                sphere_render.render2(simpleDepthShader, camera.Position, model, projection, view);
            }
            for (unsigned int i = 0; i < 1; i++) {
                glm::mat4 model = glm::mat4(1.0f);
                model = glm::translate(model, Positions[i]);
                model = glm::rotate(model, glm::radians(60.0f), glm::normalize(glm::vec3(1.0, 0.0, 1.0)));
                cube_render.render2(simpleDepthShader, camera.Position, model, projection, view);
            }
            for (unsigned int i = 2; i < 3; i++) {
                model = glm::mat4(1.0f);
                model = glm::translate(model, Positions[i]);
                cylinder_render.render2(simpleDepthShader, camera.Position, model, projection, view);
            }
            model = glm::mat4(1.0f);
            model = glm::translate(model, Positions[5]);
            cone_render.render2(simpleDepthShader, camera.Position, model, projection, view);

            model = glm::mat4(1.0f);
            model = glm::translate(model, Positions[3]);
            cube_tex_render.render2(simpleDepthShader, camera.Position, model, projection, view);

            model = glm::mat4(1.0f);
            model = glm::translate(model, Positions[4]);
            quad_render.render2(simpleDepthShader, camera.Position, model, projection, view);

           
            glBindFramebuffer(GL_FRAMEBUFFER, 0);

        }
        //rend_scene(simpleDepthShader);

        glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        for (unsigned int i = 1; i < 2; i++) {
            model = glm::mat4(1.0f);
            model = glm::translate(model, Positions[i]);
            sphere_render.render(camera.Position, model, projection, view);
        }
        for (unsigned int i = 0; i < 1; i++) {
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, Positions[i]);
            model = glm::rotate(model, glm::radians(60.0f), glm::normalize(glm::vec3(1.0, 0.0, 1.0)));
            cube_render.render(camera.Position, model, projection, view);
        }
        for (unsigned int i = 2; i < 3; i++) {
            model = glm::mat4(1.0f);
            model = glm::translate(model, Positions[i]);
            cylinder_render.render(camera.Position, model, projection, view);
        }
        model = glm::mat4(1.0f);
        model = glm::translate(model, Positions[5]);
        cone_render.render(camera.Position, model, projection, view);

        model = glm::mat4(1.0f);
        model = glm::translate(model, Positions[3]);
        cube_tex_render.render(camera.Position, model, projection, view);

        model = glm::mat4(1.0f);
        //model = glm::translate(model, Positions[4]);
        quad_render.render(camera.Position, model, projection, view);
        for (int i = 0; i < NR_POINT_LIGHTS; i++) {
            model = glm::mat4(1.0f);
            model = glm::translate(model, pointLightPositions[i]);
            light_render.render(camera.Position, model, projection, view);
        }

        model = glm::mat4(1.0f);
        model = glm::scale(model, glm::vec3(1.0f)); // Make it a smaller cube
        skybox_render.render(model, projection, view);
        
        model = glm::mat4(1.0f);
        model = glm::translate(model, GlassBallPositions[0]);
        glass_render.render(camera.Position, model, projection, view);
        //rend_scene(); 
        /*debugDepthQuad.setFloat("near_plane", near_plane);
        debugDepthQuad.setFloat("far_plane", far_plane);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, depthMap); */

            // renderQuad();

        // world transformation
       //draw sphere
        

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();

    }

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}




void set_point_light(Shader& shader, int i, glm::vec3 position, glm::vec3 ambient, 
                        glm::vec3 diffuse, glm::vec3 specular,
                        float constant, float linear, float quadratic) 
{
    shader.use();

    char str3[128];
    sprintf(str3, "pointLights[%d].position", i); 
    shader.setVec3(str3, position.x, position.y, position.z);
    sprintf(str3, "pointLights[%d].ambient", i);
    shader.setVec3(str3, ambient.x, ambient.y, ambient.z);
    sprintf(str3, "pointLights[%d].diffuse", i);
    shader.setVec3(str3, diffuse.x, diffuse.y, diffuse.z);
    sprintf(str3, "pointLights[%d].specular", i);
    shader.setVec3(str3, specular.x, specular.y, specular.z);
    sprintf(str3, "pointLights[%d].constant", i);
    shader.setFloat(str3, constant);
    sprintf(str3, "pointLights[%d].linear", i);
    shader.setFloat(str3, linear);
    sprintf(str3, "pointLights[%d].quadratic", i);
    shader.setFloat(str3, quadratic);
    std::cout << "str3: " << str3 << std::endl;
}
int glfw_init_all(GLFWwindow *& window) {
    // glfw: initialize and configure
// ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
    // glfw window creation
    // --------------------
    window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // tell GLFW to capture our mouse
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
    return 0;
}
// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
GLint viewPort[4] = { 0 };

void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS) {
        glGetIntegerv(GL_VIEWPORT, viewPort);
        glReadPixels(viewPort[0], viewPort[1], viewPort[2], viewPort[3], GL_RGB, GL_UNSIGNED_BYTE, colorArr);
        printf("color data read !\n");
        saveColorData(colorArr, (std::string)"tmpcolor.txt");
    }
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(yoffset);
}


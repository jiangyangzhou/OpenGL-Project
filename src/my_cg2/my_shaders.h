#ifndef  _MY_SHADER_FILE_H  
#define  _MY_SHADER_FILE_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <learnopengl/filesystem.h>
#include <learnopengl/shader_m.h>
#include <learnopengl/camera.h>
#include <string>

#include "utils.h"
#define NR_POINT_LIGHTS 2


std::vector<unsigned short> GL_TEXTURE_vector = { GL_TEXTURE0, GL_TEXTURE1, GL_TEXTURE2, GL_TEXTURE3, GL_TEXTURE4, GL_TEXTURE5,
GL_TEXTURE6, GL_TEXTURE7, GL_TEXTURE8, GL_TEXTURE9, GL_TEXTURE10 };
void build_gl_VAO(unsigned int* VAO, unsigned int* VBO, int v_size, float* vertices, std::vector<int> frags) {
    int len_v = 0;
    for (int i = 0; i < frags.size(); i++) len_v += frags[i];

    glGenVertexArrays(1, VAO);
    glGenBuffers(1, VBO);
    glBindBuffer(GL_ARRAY_BUFFER, *VBO);
    glBufferData(GL_ARRAY_BUFFER, v_size * sizeof(float), vertices, GL_STATIC_DRAW);

    glBindVertexArray(*VAO);
    int acc = 0;
    for (int j = 0; j < frags.size(); j++) {
        std::cout << frags[j] << acc << len_v << std::endl;
        glVertexAttribPointer(j, frags[j], GL_FLOAT, GL_FALSE, len_v * sizeof(float), (void*)(acc* sizeof(float)));
        glEnableVertexAttribArray(j);
        acc += frags[j];
    }
    glBindVertexArray(0);
}


void build_gl_VAO_with_color(unsigned int *VAO, unsigned int *VBO, int v_size, float *vertices){
    glGenVertexArrays(1, VAO);
    glGenBuffers(1, VBO);    


    glBindBuffer(GL_ARRAY_BUFFER, *VBO);
    glBufferData(GL_ARRAY_BUFFER, v_size * sizeof(float), vertices, GL_STATIC_DRAW);

    glBindVertexArray(*VAO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 10 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 10 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, 10 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);
    glBindVertexArray(0);
}
void build_gl_VAO_with_tex(unsigned int* VAO, unsigned int* VBO, int v_size, float* vertices) {
    glGenVertexArrays(1, VAO);
    glGenBuffers(1, VBO);

    glBindBuffer(GL_ARRAY_BUFFER, *VBO);
    glBufferData(GL_ARRAY_BUFFER, v_size * sizeof(float), vertices, GL_STATIC_DRAW);

    glBindVertexArray(*VAO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);
}


struct point {
    glm::vec3 pos;
    glm::vec3 norm;
    glm::vec4 color = glm::vec4(1.0, 1.0, 1.0, 1.0);
    //default point norm is sphere-like
    point(glm::vec3 a) :pos(a), norm(a * 2.0f) {};
    point(glm::vec3 a, glm::vec4 col) :pos(a), norm(a * 2.0f), color(col) {};
    point(glm::vec3 a, glm::vec3 n, glm::vec4 col) :pos(a), norm(n), color(col) {};
    point(float x, float y, float z) :
        pos(glm::vec3(x, y, z)), norm(glm::vec3(x, y, z) * 2.0f){};
    point(float x, float y, float z, float r, float g, float b, float alp ):
        pos(glm::vec3(x,y,z)), norm(glm::vec3(x, y, z)*2.0f), color(glm::vec4(r,g,b,alp)) {};
    point(float x, float y, float z, float nx, float ny, float nz, float r, float g, float b, float alp) :
        pos(glm::vec3(x, y, z)), norm(glm::vec3(nx, ny, nz)), color(glm::vec4(r, g, b, alp)) {};
    point() {};
};
struct triangle {
    point p1;
    point p2;
    point p3;
    triangle(point a, point b, point c) :
        p1(a), p2(b), p3(c)
    {};
    triangle(glm::vec3 a, glm::vec3 b, glm::vec3 c) :
        p1(point(a)),p2(point(b)),p3(point(c))
    {};
    triangle() {};
};

struct point_tex {
    glm::vec3 pos;
    glm::vec3 norm;
    glm::vec2 coord;
    //default point norm is sphere-like
    point_tex(glm::vec3 a) :pos(a), norm(a * 2.0f) {};
    point_tex(glm::vec3 a, glm::vec2 co) :pos(a), norm(a * 2.0f), coord(co) {};
    point_tex(glm::vec3 a, glm::vec3 n, glm::vec2 co) :pos(a), norm(n), coord(co) {};

    point_tex(float x, float y, float z) :
        pos(glm::vec3(x, y, z)), norm(glm::vec3(x, y, z) * 2.0f) {};
    point_tex(float x, float y, float z, float c1, float c2) :
        pos(glm::vec3(x, y, z)), norm(glm::vec3(x, y, z) * 2.0f), coord(glm::vec2(c1, c2)) {};
    point_tex(float x, float y, float z, float nx, float ny, float nz, float c1, float c2) :
        pos(glm::vec3(x, y, z)), norm(glm::vec3(nx, ny, nz)), coord(glm::vec2(c1,c2)) {};
    point_tex() {};
};

struct triangle_tex {
    point_tex p1;
    point_tex p2;
    point_tex p3;
    triangle_tex() {};
    triangle_tex(point_tex a, point_tex b, point_tex c) :
        p1(a), p2(b), p3(c)
    {};
    triangle_tex(glm::vec3 a, glm::vec3 b, glm::vec3 c) :
        p1(point_tex(a)), p2(point_tex(b)), p3(point_tex(c))
    {};
};
void build_triangle_for_rectangle(std::vector<glm::vec3> face_vec, triangle& tri1, triangle& tri2, const glm::vec4 color)
{
    // input squeeze:  leftdown, rightdown, leftup, rightup  
    //23
    //01
    glm::vec3 norm =  glm::cross(face_vec[1] - face_vec[0], face_vec[2] - face_vec[0]);
    if (glm::dot(norm,face_vec[0]) < 0)
        norm = -norm;
    tri1 = triangle(point(face_vec[0], norm, color), point(face_vec[1], norm, color), point(face_vec[2], norm, color));
    tri2 = triangle(point(face_vec[2], norm, color), point(face_vec[1], norm, color), point(face_vec[3], norm, color));
}

void build_triangle_for_rectangle_tex(std::vector<glm::vec3> face_vec, triangle_tex& tri1, triangle_tex& tri2)
{
    // input squeeze:  leftdown, rightdown, leftup, rightup
    glm::vec3 norm = glm::cross(face_vec[1] - face_vec[0], face_vec[2] - face_vec[0]);
    if (glm::dot(norm, face_vec[0]) < 0)
        norm = -norm;
    std::cout << "face_vec len" << face_vec.size() << std::endl;
    tri1 = triangle_tex(point_tex(face_vec[0], norm, glm::vec2(0.0, 0.0)),
        point_tex(face_vec[1], norm, glm::vec2(1.0, 0.0)),
        point_tex(face_vec[2], norm, glm::vec2(0.0, 1.0)));
    tri2 = triangle_tex(point_tex(face_vec[2], norm, glm::vec2(0.0, 1.0)),
        point_tex(face_vec[1], norm, glm::vec2(1.0, 0.0)),
        point_tex(face_vec[3], norm, glm::vec2(1.0, 1.0)));
}

int get_sphere_vertices(float*& vertices, glm::vec4 color, int depth=4) {
    std::vector<triangle> vec;
    point front(0., 0., 0.5, color.r, color.g, color.b, color.a);
    point back(0., 0., -0.5, color.r, color.g, color.b, color.a);
    point left(-0.5, 0., 0., color.r, color.g, color.b, color.a);
    point right(0.5, 0., 0., color.r, color.g, color.b, color.a);
    point up(0., 0.5, 0., color.r, color.g, color.b, color.a);
    point down(0., -0.5, 0., color.r, color.g, color.b, color.a);

    vec.push_back(triangle(up, front, right));
    vec.push_back(triangle(up, right, back));
    vec.push_back(triangle(up, back, left));
    vec.push_back(triangle(up, left, front));
    vec.push_back(triangle(front, down, right));
    vec.push_back(triangle(right, down, back));
    vec.push_back(triangle(back, down, left));
    vec.push_back(triangle(left, down, front));
    for (int j = 0; j < depth - 1; j++) {
        std::vector<triangle> new_vec;
        triangle tri;
        while (!vec.empty()) {
            tri = vec.back();
            vec.pop_back();
            glm::vec3 new_pos1 = glm::normalize(tri.p1.pos + tri.p2.pos) * 0.5f;
            glm::vec3 new_pos2 = glm::normalize(tri.p1.pos + tri.p3.pos) * 0.5f;
            glm::vec3 new_pos3 = glm::normalize(tri.p2.pos + tri.p3.pos) * 0.5f;
            glm::vec4 new_color1 = (tri.p1.color + tri.p2.color) * 0.5f;
            glm::vec4 new_color2 = (tri.p1.color + tri.p3.color) * 0.5f;
            glm::vec4 new_color3 = (tri.p2.color + tri.p3.color) * 0.5f;
            point new1 = point(new_pos1, new_color1);
            point new2 = point(new_pos2, new_color2);
            point new3 = point(new_pos3, new_color3);
            new_vec.push_back(triangle(tri.p1, new1, new2));   //                    
            new_vec.push_back(triangle(new1, tri.p2, new3));  //                   
            new_vec.push_back(triangle(new2, new3, tri.p3));  //                   
            new_vec.push_back(triangle(new1, new3, new2));
        }
        vec = new_vec;
    }
    int tri_num = vec.size();
    vertices = new float[tri_num * 3 * 10];
    memcpy(vertices, &vec[0], tri_num * 3 * 10 * sizeof(float));
    return 3 * 10 * tri_num;
}

int get_cylinder_vertices(float*& vertices, const glm::vec4 color, int segs = 4) {
    std::vector<triangle> vec;
    float PI = glm::pi<float>();
    float theta = 2 * PI / segs;
    float x;
    float z;
    float last_x = cos(-theta);
    float last_z = sin(-theta);
    for (float th = 0; th <= 2*PI; th += theta) {
        x = cos(th);
        z = sin(th);
        triangle tri1, tri2;
        // 圆柱侧面的矩形
        std::vector<glm::vec3> face_vec;
        for (float y = -0.5; y < 1.0; y += 1.0) {
            face_vec.push_back(glm::vec3(last_x, y, last_z));
            face_vec.push_back(glm::vec3(x, y, z));
            glm::vec3 norm(0, y*2.0, 0);
            point p1(glm::vec3(last_x, y, last_z), norm, color);
            point p2(glm::vec3(x, y, z), norm, color);
            point p3(glm::vec3(0, y, 0), norm,  color);
            vec.push_back(triangle(p1,p2,p3));  //底面
        }
        build_triangle_for_rectangle(face_vec, tri1, tri2, color);
        vec.push_back(tri1);
        vec.push_back(tri2);  //侧面
        last_x = x;
        last_z = z;
    }
    int tri_num = vec.size();
    vertices = new float[tri_num * 3 * 10];
    memcpy(vertices, &vec[0], tri_num * 3 * 10 * sizeof(float));
    return 3 * 10 * tri_num;
}

int get_cone_vertices(float*& vertices, const glm::vec4 color, int segs = 4) {
    std::vector<triangle> vec;
    float PI = glm::pi<float>();
    float theta = 2 * PI / segs;
    float x;
    float z;
    float y1 = -0.5;
    float y2 = 1.5;
    float last_x = cos(-theta);
    float last_z = sin(-theta);
    glm::vec3 v1, v2, v3;
    for (float th = 0; th <= 2 * PI; th += theta) {
        x = cos(th);
        z = sin(th);

        v1 = glm::vec3(last_x, y1, last_z);
        v2 = glm::vec3(x, y1, z);
        glm::vec3 norm(0, y1 * 2.0, 0);
        point p1(v1, norm, color);
        point p2(v2, norm, color);
        point p3(glm::vec3(0, y1, 0), norm, color);
        vec.push_back(triangle(p1, p2, p3));  //底面

        v3 = glm::vec3(0, y2, 0);
        glm::vec3 norm2 = glm::cross(v2-v1, v3 - v1);
        point p4(v1, norm2, color);
        point p5(v2, norm2, color);
        point p6(v3, norm2, color);
        vec.push_back(triangle(p4, p5, p6));  //底面
        last_x = x;
        last_z = z;
    }
    int tri_num = vec.size();
    vertices = new float[tri_num * 3 * 10];
    memcpy(vertices, &vec[0], tri_num * 3 * 10 * sizeof(float));
    return 3 * 10 * tri_num;
}




int get_cube_vertices(float*& vertices, std::vector<glm::vec4>& color_vec) {
    glm::vec4 color = glm::vec4(1.0, 1.0, 1.0, 1.0);;
    std::vector<triangle> tri_vec;
    for (float i = -0.5; i < 1.0; i++) {
        std::vector<std::vector<glm::vec3>> face_vecs = { 
            std::vector<glm::vec3>(),std::vector<glm::vec3>(),std::vector<glm::vec3>()
        };
        for (float j = -0.5; j < 1.0; j++) {
            for (float k = -0.5; k < 1.0; k++) {
                face_vecs[0].push_back(glm::vec3(i, j, k));
                face_vecs[1].push_back(glm::vec3(j, k, i));
                face_vecs[2].push_back(glm::vec3(k, i, j));
            }
        }
        triangle tri1, tri2;
        for (int m = 0; m < 3; m++) {
            if (color_vec.size() == 0)
                color = glm::vec4(1.0, 1.0, 1.0, 1.0);
            else if (color_vec.size() == 1) color = color_vec[0];
            //else if (color_vec.size() == 6) color = color_vec[3 * ((int)(i + 0.51)) + m];
            //std::cout << "face_vecss.size()" << face_vecs.size()<<std::endl;
            build_triangle_for_rectangle(face_vecs[m], tri1, tri2, color);
            tri_vec.push_back(tri1);
            tri_vec.push_back(tri2);
        }
    }
    int tri_num = tri_vec.size();
    //std::cout << "tri num: in cube_tex" << tri_num << std::endl;
    vertices = new float[tri_num * 3 * 10];
    memcpy(vertices, &tri_vec[0], tri_num * 3 * 10 * sizeof(float));
    return 3 * 10 * tri_num;
}

int get_cube_vertices_tex(float*& vertices) {
    std::vector<triangle_tex> tri_vec;
    for (float i = -0.5; i < 1.0; i++) {
        std::vector<std::vector<glm::vec3>> face_vecs = {
            std::vector<glm::vec3>(), std::vector<glm::vec3>(), std::vector<glm::vec3>()
        };        
        for (float j = -0.5; j < 1.0; j++) {
            for (float k = -0.5; k < 1.0; k++) {
                face_vecs[0].push_back(glm::vec3(i, j, k));
                face_vecs[1].push_back(glm::vec3(j, k, i));
                face_vecs[2].push_back(glm::vec3(k, i, j));
            }
        }
        triangle_tex tri1, tri2;
        for (int m = 0; m < 3; m++) {
            build_triangle_for_rectangle_tex(face_vecs[m], tri1, tri2);
            tri_vec.push_back(tri1);
            tri_vec.push_back(tri2);
        }
    }
    int tri_num = tri_vec.size();
    std::cout << "trinum of cube_tex" << tri_num << std::endl;
    vertices = new float[tri_num * 3 * 8];
    memcpy(vertices, &tri_vec[0], tri_num * 3 * 8 * sizeof(float));
    return 3 * 8 * tri_num;
}

int get_skybox_vertices(float*& vertices) {
    float skyboxVertices[] = {
        // positions          
        -1.0f,  1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

        -1.0f,  1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f,  1.0f
    };
    vertices = new float[12 * 3 * 3];
    memcpy(vertices, &skyboxVertices[0], 36 * 3 * sizeof(float));
    return 36 * 3;
}

class Render {
public:
    Shader shader;
    float* vertices;
    int v_size;
    int p_size = 10;
    unsigned int VBO, VAO;
    unsigned int shadowMap[2];
    unsigned int shadow_map_id[2];
    bool enable_shadow= true;
    Render() {};
    Render(char* vs_file, char* fs_file) {
        this->shader = Shader(vs_file, fs_file);
    };
    void set_viewPos(glm::vec3 pos) {
        shader.use();
        shader.setVec3("viewPos", pos); };
    void set_shininess(float shininess) {
        shader.use();
        shader.setFloat("material.shininess", shininess); 
    };
    void set_shadowmap(unsigned int shadowmap_id[], unsigned int shadowMap[]) {
        shader.use();
        this->shadow_map_id[0] = shadowmap_id[0];
        this->shadow_map_id[1] = shadowmap_id[1];
        this->shadowMap[0] = shadowMap[0];
        this->shadowMap[1] = shadowMap[1];
        shader.setInt("shadowMap[0]", shadowmap_id[0]);
        shader.setInt("shadowMap[1]", shadowmap_id[1]);

    };
    void set_light_matrix(glm::mat4 &light_matrix, int i) {
        this->shader.use();
        char str3[128];
        sprintf(str3, "lightSpaceMatrix[%d]", i);
        shader.setMat4(str3, light_matrix);
    }
    void bind_vertices() {
        build_gl_VAO_with_color(&VAO, &VBO, v_size, vertices);
    };
    Shader get_shader() { return shader; };
    void render(glm::vec3 cam_pos, glm::mat4 model, glm::mat4 projection, glm::mat4 view);
    void render2(Shader &new_shader, glm::vec3 cam_pos, glm::mat4 model, glm::mat4 projection, glm::mat4 view);
    
    
    ~Render() {
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
        delete& shader;
    };

};
void Render::render2(Shader &new_shader, glm::vec3 cam_pos, glm::mat4 model, glm::mat4 projection, glm::mat4 view) {
    new_shader.use();
    glBindVertexArray(VAO);
    new_shader.setMat4("model", model);
    glDrawArrays(GL_TRIANGLES, 0, v_size / p_size);
    glBindVertexArray(0);
}
void Render::render(glm::vec3 cam_pos, glm::mat4 model, glm::mat4 projection,
      glm::mat4 view) {
    this->shader.use();
    glBindVertexArray(VAO);
    this->set_viewPos(cam_pos);
    shader.setMat4("projection", projection);
    shader.setMat4("view", view);
    shader.setMat4("model", model);
    if (enable_shadow) {
        for (int i = 0; i < NR_POINT_LIGHTS; i++) {
            glActiveTexture(GL_TEXTURE_vector[shadow_map_id[i]]);
            glBindTexture(GL_TEXTURE_2D, shadowMap[i]);
        }
    }
    glDrawArrays(GL_TRIANGLES, 0, v_size / p_size);
    glBindVertexArray(0);
}


class SkyboxRender:public Render {
public:
    int cubeMap = -1;
    int cube_tex_id = 0;
    SkyboxRender() {};
    SkyboxRender(char* vs_file, char* fs_file) :Render(vs_file, fs_file) { p_size = 3; };
    void set_cubeMap(char* cube_dir, int tex_id) {
        std::vector<std::string> faces
        {
            std::string(cube_dir) + std::string("/right.jpg"),
            std::string(cube_dir) + std::string("/left.jpg"),
            std::string(cube_dir) + std::string("/top.jpg"),
            std::string(cube_dir) + std::string("/bottom.jpg"),
            std::string(cube_dir) + std::string("/front.jpg"),
            std::string(cube_dir) + std::string("/back.jpg")
        };
        cube_tex_id = tex_id;
        cubeMap = loadCubemap(faces);
        this->shader.use();
        shader.setInt("cubeTexture", cube_tex_id);
    };
    void build_vertices() {
        this->v_size = get_skybox_vertices(this->vertices);
        std::vector<int> fracs = { 3 };
        build_gl_VAO(&VAO, &VBO, v_size, vertices, fracs);
        this->shader.use();
    };
    void render(glm::mat4 model, glm::mat4 projection, glm::mat4 view) {
        shader.use();
        glDepthFunc(GL_LEQUAL);  // change depth function so depth test passes when values are equal to depth buffer's content
        glBindVertexArray(VAO);
        glActiveTexture(GL_TEXTURE_vector[cube_tex_id]);
        glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMap);
        shader.setMat4("projection", projection);
        glm::mat4 view_mat = view;
        view_mat[3][0] = 0.0f;
        view_mat[3][1] = 0.0f;
        view_mat[3][2] = 0.0f;
        shader.setMat4("view", view_mat);
        shader.setMat4("model", model);
        //std::cout << "cubeMap" << cubeMap;
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);
        glDepthFunc(GL_LESS); // set depth function back to default
    };

};
class QuadRender : public Render {
public:
    int diffuseMap = -1;
    int diff_tex_id = 0;
    int specularMap = -1;
    int specular_tex_id = 0;
    QuadRender() {};
    QuadRender(char* vs_file, char* fs_file) :Render(vs_file, fs_file) { p_size = 8; };
    ~QuadRender() {};
    void set_diffuseMap(char* diffuse_file, int tex_id) {
        shader.use();
        diffuseMap = loadTexture(diffuse_file);
        diff_tex_id = tex_id;
        shader.setInt("material.diffuse", diff_tex_id);
    };
    void set_specularMap(char* specular_file, int tex_id) {
        shader.use();
        specularMap = loadTexture(specular_file);
        specular_tex_id = tex_id;
        shader.setInt("material.specular", specular_tex_id);
    };

    void build_vertices() {
        float planeVertices[] = {
            // positions            // normals         // texcoords
             25.0f, -0.5f,  25.0f,  0.0f, 1.0f, 0.0f,  25.0f,  0.0f,
            -25.0f, -0.5f,  25.0f,  0.0f, 1.0f, 0.0f,   0.0f,  0.0f,
            -25.0f, -0.5f, -25.0f,  0.0f, 1.0f, 0.0f,   0.0f, 25.0f,

             25.0f, -0.5f,  25.0f,  0.0f, 1.0f, 0.0f,  25.0f,  0.0f,
            -25.0f, -0.5f, -25.0f,  0.0f, 1.0f, 0.0f,   0.0f, 25.0f,
             25.0f, -0.5f, -25.0f,  0.0f, 1.0f, 0.0f,  25.0f, 25.0f
        };
        std::vector<int> fracs = { 3, 3, 2 };
        vertices = &planeVertices[0];
        v_size = 48;
        build_gl_VAO_with_tex(&VAO, &VBO, v_size, vertices);
    }
    void  render(glm::vec3 cam_pos, glm::mat4 model, glm::mat4 projection, glm::mat4 view) {
        shader.use();
        glBindVertexArray(VAO);
        this->set_viewPos(cam_pos);
        shader.setMat4("projection", projection);
        shader.setMat4("view", view);
        shader.setMat4("model", model);
        if (diffuseMap != -1) {
            glActiveTexture(GL_TEXTURE_vector[diff_tex_id]);
            glBindTexture(GL_TEXTURE_2D, diffuseMap);
        }
        if (specularMap != -1) {
            glActiveTexture(GL_TEXTURE_vector[specular_tex_id]);
            glBindTexture(GL_TEXTURE_2D, specularMap);
        }
        if (enable_shadow) {
            for (int i = 0; i < NR_POINT_LIGHTS; i++) {
                glActiveTexture(GL_TEXTURE_vector[shadow_map_id[i]]);
                glBindTexture(GL_TEXTURE_2D, shadowMap[i]);
            }
        }
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glBindVertexArray(0);
    }
};

class CubeRender_tex: public Render {
public:
    int diffuseMap = -1;
    int diff_tex_id = -1;
    int specularMap = -1;
    int specular_tex_id = -1;

    CubeRender_tex() {};
    CubeRender_tex(char* vs_file, char* fs_file) :Render(vs_file, fs_file) { p_size = 8; };
    ~CubeRender_tex() {};
    void set_diffuseMap(char* diffuse_file, int tex_id) {
        shader.use();
        diffuseMap = loadTexture(diffuse_file);
        diff_tex_id = tex_id;
        shader.setInt("material.diffuse", diff_tex_id);
        printf("diffuse map: %d", diffuseMap);
    };
    void set_specularMap(char* specular_file, int tex_id) {
        shader.use();
        if (!(tex_id == diff_tex_id && tex_id>=0))
            specularMap = loadTexture(specular_file);
        specular_tex_id = tex_id;
        shader.setInt("material.specular", specular_tex_id);
    };
    void flip_norm() {
        for (int i = 0; i < v_size; i+=8) {
            vertices[i + 3] = -vertices[i + 3];
            vertices[i + 4] = -vertices[i + 4];
            vertices[i + 5] = -vertices[i + 5];
        }
    }
    void build_vertices();
    void render(glm::vec3 cam_pos, glm::mat4 model, glm::mat4 projection, glm::mat4 view) ;
};

void CubeRender_tex::build_vertices() {
    this->v_size = get_cube_vertices_tex(this->vertices);
    build_gl_VAO_with_tex(&VAO, &VBO, v_size, vertices);
    this->shader.use();
}
void  CubeRender_tex::render(glm::vec3 cam_pos, glm::mat4 model, glm::mat4 projection, glm::mat4 view) {
    this->shader.use();
    glBindVertexArray(VAO);
    this->set_viewPos(cam_pos);
    shader.setMat4("projection", projection);
    shader.setMat4("view", view);
    shader.setMat4("model", model);
    glBindVertexArray(VAO);
    if (1 || diffuseMap != -1) {
        glActiveTexture(GL_TEXTURE_vector[diff_tex_id]);
        glBindTexture(GL_TEXTURE_2D, diffuseMap);
    }
    if (1|| specularMap != -1) {
        glActiveTexture(GL_TEXTURE_vector[specular_tex_id]);
        glBindTexture(GL_TEXTURE_2D, specularMap);
    }
    if (enable_shadow) {
        for (int i = 0; i < NR_POINT_LIGHTS; i++) {
            glActiveTexture(GL_TEXTURE_vector[shadow_map_id[i]]);
            glBindTexture(GL_TEXTURE_2D, shadowMap[i]);
        }
    }
    //Render::render(cam_pos, model, projection, view);
    //glDrawArrays(GL_TRIANGLES, 0, v_size / p_size);
    glDrawArrays(GL_TRIANGLES, 0, v_size / p_size);

    glBindVertexArray(0);
}

//被灯照的cube_colored
class CubeRender : public Render {
public:
    CubeRender() {};
    CubeRender(char* vs_file, char* fs_file) :Render(vs_file, fs_file) {};
    ~CubeRender() { };
    void build_vertices(glm::vec4 color);
    // void render(glm::vec3 cam_pos, glm::mat4 model, glm::mat4 projection, glm::mat4 view);
};


void CubeRender::build_vertices(glm::vec4 color) {
    std::vector<glm::vec4> color_vec = {color};
    this->v_size = get_cylinder_vertices(this->vertices, color_vec[0], 4);
    //for (int i = 0; i < 12; i++) std::cout << vertices[i] << " ";
    build_gl_VAO_with_color(&VAO, &VBO, v_size, vertices);
    std::cout << "VAO" << VAO << VBO << std::endl;
    this->shader.use();
}

//被灯照的sphere
class SphereRender: public Render {
public:
    SphereRender() {};
    SphereRender(char* vs_file, char* fs_file) :Render(vs_file, fs_file) {};  
    ~SphereRender() { };
    void build_vertices(glm::vec4 color);
   // void render(glm::vec3 cam_pos, glm::mat4 model, glm::mat4 projection, glm::mat4 view);
};




void SphereRender::build_vertices(glm::vec4 color) {
    this->v_size = get_sphere_vertices(this->vertices, color, 8);
    for (int i = 0; i < 240; i++) std::cout << vertices[i] << " ";
    build_gl_VAO_with_color(&VAO, &VBO, v_size, vertices);
    std::cout << "VAO" << VAO << VBO << std::endl;
    this->shader.use();
}

//被灯照的玻璃sphere
class GlassSphereRender : public SphereRender {
public:
    GlassSphereRender() {};
    GlassSphereRender(char* vs_file, char* fs_file) :SphereRender(vs_file, fs_file)
    {
        enable_shadow = true;
    };
    ~GlassSphereRender() { };
    void set_cube_map() {};
    //void build_vertices(glm::vec4 color);
    // void render(glm::vec3 cam_pos, glm::mat4 model, glm::mat4 projection, glm::mat4 view);
};



// 灯自己的Render
class LightRender: public Render {
public:
    std::string shape;
    LightRender(std::string shape);
    ~LightRender() {};
    void build_vertices(glm::vec4 color);
};
LightRender::LightRender(std::string shape) {
    this->shape = shape;
    p_size = 10;
    enable_shadow = 0;
    shader = Shader("basic_with_color.vs", "basic_light.fs");
}

void LightRender::build_vertices(glm::vec4 color) {
    if(this->shape==std::string("sphere"))
        this->v_size = get_sphere_vertices(this->vertices, color, 4);
    if (this->shape == std::string("cube")) {
        std::vector<glm::vec4> color_vec = {color};
        this->v_size = get_cube_vertices(this->vertices, color_vec);
    }
    build_gl_VAO_with_color(&VAO, &VBO, v_size, vertices);
}



class CylinderRender: public Render {
public:
    CylinderRender() {};
    CylinderRender(char* vs_file, char* fs_file) :Render(vs_file, fs_file) {};
    ~CylinderRender() {};
    void build_vertices(glm::vec4 color);
    //void render(glm::vec3 cam_pos, glm::mat4 model, glm::mat4 projection, glm::mat4 view);
};

void CylinderRender::build_vertices(glm::vec4 color) {
    this->v_size = get_cylinder_vertices(vertices, color, 180);
    build_gl_VAO_with_color(&VAO, &VBO, v_size, vertices);
}


class ConeRender : public Render {
public:
    ConeRender() {};
    ConeRender(char* vs_file, char* fs_file) :Render(vs_file, fs_file) {};
    ~ConeRender() {};
    void build_vertices(glm::vec4 color);
    //void render(glm::vec3 cam_pos, glm::mat4 model, glm::mat4 projection, glm::mat4 view);
};

void ConeRender::build_vertices(glm::vec4 color) {
    this->v_size = get_cone_vertices(vertices, color, 180);
    build_gl_VAO_with_color(&VAO, &VBO, v_size, vertices);
}


#endif
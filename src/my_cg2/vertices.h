#ifndef  _PRJ_DIR_FILE_H  //必须确保header guard宏名永不重名
#define  _PRJ_DIR_FILE_H
#include <vector>
#include <glm/glm.hpp>

int box_vertices(float *& vertices){
    vertices = new float[288];
    float init_vertices[] = {
        // positions          // normals           // texture coords
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,

        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f
    };
    for (int i = 0; i < 288; i++)
        vertices[i] = init_vertices[i];
    return 6 * 8 * 6;
}
struct triangle {
    glm::vec3 first;
    glm::vec3 norm1;
    glm::vec2 coord1 = glm::vec2(0.,0.);
    glm::vec3 second;
    glm::vec3 norm2;
    glm::vec2 coord2 = glm::vec2(0.,0.);
    glm::vec3 third;
    glm::vec3 norm3;
    glm::vec2 coord3 = glm::vec2(0., 0.);
    triangle(glm::vec3 a, glm::vec3 b, glm::vec3 c) : first(a), second(b), third(c), norm1(a*2.0f), norm2(b*2.0f), norm3(c*2.0f){};
    triangle() {};
};

int sphere_vertices(float*& vertices, int depth) {
    std::vector<triangle> vec;
    glm::vec3 front(0., 0., 0.5);
    glm::vec3 back(0., 0., -0.5);
    glm::vec3 left(-0.5, 0., 0.);
    glm::vec3 right(0.5, 0., 0.);
    glm::vec3 up(0., 0.5, 0.);
    glm::vec3 down(0., -0.5, 0.);

    vec.push_back(triangle(up, front, right));
    vec.push_back(triangle(up, right, back));
    vec.push_back(triangle(up, back, left));
    vec.push_back(triangle(up, left, front));
    vec.push_back(triangle(front, down, right));
    vec.push_back(triangle(right, down, back));
    vec.push_back(triangle(back, down, left));
    vec.push_back(triangle(left, down, front));
    std::cout << "push back tri1 ok";
    for (int j = 0; j < depth - 1; j++) {
        std::vector<triangle> new_vec;
        triangle tri;
        while (!vec.empty()) {
            tri = vec.back();
            vec.pop_back();
            glm::vec3 new1 = glm::normalize(tri.first + tri.second) * 0.5f;
            glm::vec3 new2 = glm::normalize(tri.first + tri.third) * 0.5f;
            glm::vec3 new3 = glm::normalize(tri.second + tri.third) * 0.5f;    \
                std::cout << new1.x << new1.y << new1.z<<std::endl;
            std::cout << new2.x << new2.y << new2.z << std::endl;
            std::cout << new3.x << new3.y << new3.z << std::endl;
            new_vec.push_back(triangle(tri.first, new1, new2));   //                    
            new_vec.push_back(triangle(new1, tri.second, new3));  //                   
            new_vec.push_back(triangle(new2, new3, tri.third));  //                   
            new_vec.push_back(triangle(new1, new3, new2));
        }
        vec = new_vec;
    }
    int tri_num = vec.size();
    vertices = new float[tri_num * 3 * 8];
    memcpy(vertices, &vec[0], tri_num * 3 * 8 * sizeof(float));
    return 3 * 8 * tri_num;
}



#endif
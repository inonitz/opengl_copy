#include "bhdr.h"
#include "Shader.h"
#include "Buffers.h"
#include "Camera.h"
#include "Window.h"
//#include "glm.hpp"
#include <gtx/transform.hpp>
//#include <gtx/string_cast.hpp>
//#include <gtc/type_ptr.hpp>

int i = 0;

void update(Window& win, Camera& camera, mlib::mat4& model)
{

         if (win.keyPressed(GLFW_KEY_ESCAPE)) win.close();
    else if (win.keyPressed(GLFW_KEY_W)) camera.update(win.dt, MOVEMENT_KEY::W);
    else if (win.keyPressed(GLFW_KEY_S)) camera.update(win.dt, MOVEMENT_KEY::S);
    else if (win.keyPressed(GLFW_KEY_A)) camera.update(win.dt, MOVEMENT_KEY::A);
    else if (win.keyPressed(GLFW_KEY_D)) camera.update(win.dt, MOVEMENT_KEY::D);
    else if (win.keyPressed(GLFW_KEY_V)) camera.update(win.dt, MOVEMENT_KEY::V);
    else if (win.keyPressed(GLFW_KEY_C)) camera.update(win.dt, MOVEMENT_KEY::C);
    win.update();
    float d0 = mlib::PIf / 2 * win.dt;
    if (i % 100 == 0)
    {
        win.clearConsole(); std::cout << model.str();
        camera.debug();
    }
    i++;
    //else if (glfwGetKey(win, GLFW_KEY_1) == GLFW_PRESS) translate.x += dx;
    //else if (glfwGetKey(win, GLFW_KEY_2) == GLFW_PRESS) translate.x -= dx;
    //else if (glfwGetKey(win, GLFW_KEY_3) == GLFW_PRESS) translate.y += dx;
    //else if (glfwGetKey(win, GLFW_KEY_4) == GLFW_PRESS) translate.y -= dx;
    //else if (glfwGetKey(win, GLFW_KEY_5) == GLFW_PRESS) translate.z += dx;
    //else if (glfwGetKey(win, GLFW_KEY_6) == GLFW_PRESS) translate.z -= dx;
    //else if (glfwGetKey(win, GLFW_KEY_7) == GLFW_PRESS) rot.x += d0;
    //else if (glfwGetKey(win, GLFW_KEY_8) == GLFW_PRESS) rot.x -= d0;
    //else if (glfwGetKey(win, GLFW_KEY_9) == GLFW_PRESS) rot.y += d0;
    //else if (glfwGetKey(win, GLFW_KEY_0) == GLFW_PRESS) rot.y -= d0;
    //else if (glfwGetKey(win, GLFW_KEY_MINUS) == GLFW_PRESS) rot.z += d0;
    //else if (glfwGetKey(win, GLFW_KEY_EQUAL) == GLFW_PRESS) rot.z -= d0;
}

Array<float> vertices =
{
    // pos           // col
    // pos           // col
   -1.0, -1.0, 1.0, 1.0f, 0.0f, 0.0f,
    1.0, -1.0, 1.0, 0.0f, 1.0f, 0.0f,
    1.0,  1.0, 1.0, 0.0f, 0.0f, 1.0f,
   -1.0,  1.0, 1.0, 1.0f, 0.0f, 0.0f,

   -1.0, -1.0, -1.0, 1.0f, 0.0f, 0.0f,
    1.0, -1.0, -1.0, 0.0f, 1.0f, 0.0f,
    1.0,  1.0, -1.0, 0.0f, 0.0f, 1.0f,
   -1.0,  1.0, -1.0, 1.0f, 0.0f, 0.0f
};

Array<unsigned int> indices =
{
    0, 1, 2,
    2, 3, 0,

    1, 5, 6,
    6, 2, 1,

    7, 6, 5,
    5, 4, 7,

    4, 0, 3,
    3, 7, 4,

    4, 5, 1,
    1, 0, 4,

    3, 2, 6,
    6, 7, 3
};

int main()
{
    mlib::setVectorOrder(COLUMN_MAJOR);
    mlib::vec3 position(0.0f, 0.0f, 3.0f), front(0.0f, 0.0f, -1.0f), up(0.0f, 1.0f, 0.0f);
    int32_t w = 800, h = 600;
    float_t n = 0.1f, f = 1000.0f, aspect = (float_t)w / h, fov = mlib::PIf / 4;

    Window win(w, h, true);
    Camera camera = Camera(position, front + position, up);

    mlib::vec3 trs(0.0f), rot(2 * (float)mlib::PI), scl(0.5f, 0.5f, 0.5f);
    mlib::mat4 model = mlib::scale(mlib::vec3(0.5f));
    mlib::mat4 view = camera.getView();
    mlib::mat4 projection = mlib::perspective(n, f, aspect, fov);

    Shader shader("Util/Shaders/mvp.shader");
    Array<Vertex> attribs =
    {
        Vertex({vtype::vec3, dtype::Float, 0,  true}),
        Vertex({vtype::vec3, dtype::Float, 12, true}),
    };
    VBO vertexBuffer(vertices, attribs, dataUsage::STATIC);
    IBO indexBuffer(indices, dataUsage::STATIC);
    shader.link();
    shader.use();
    vertexBuffer.bind();
    indexBuffer.bind();

    model = mlib::identity();
    mlib::mat4 mvp = mlib::mvp(projection, view, model);
    while (!win.shouldClose())
    {
        glClearColor(0.7f, 0.7f, 0.7f, 0.7f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        shader.setUniformMat4("u_model", model);
        shader.setUniformMat4("u_view",  view);
        shader.setUniformMat4("u_projection", projection);


        glDrawElements(GL_TRIANGLES, indexBuffer.m_count, GL_UNSIGNED_INT, 0);

        view = camera.getView();
        mlib::mat4 mvp = mlib::mvp(projection, view, model);
        update(win, camera, mvp);
    }

    win.close();
}
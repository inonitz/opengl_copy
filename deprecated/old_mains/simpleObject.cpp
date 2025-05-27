#include "Shader.h"
#include "Buffers.h"
#include "Window.h"

Array<float> vertices =
{
    // pos             // col            // tex coords
   -1.0, -1.0, 1.0,   1.0f, 0.0f, 0.0f,  0.0f, 0.0f,
    1.0, -1.0, 1.0,   0.0f, 1.0f, 0.0f,  1.0f, 0.0f,
    1.0,  1.0, 1.0,   0.0f, 0.0f, 1.0f,  1.0f, 1.0f,
   -1.0,  1.0, 1.0,   1.0f, 0.0f, 0.0f,  0.0f, 1.0f,

   -1.0, -1.0, 0.0,  1.0f, 0.0f, 0.0f,  0.0f, 0.0f,
    1.0, -1.0, 0.0,  0.0f, 1.0f, 0.0f,  1.0f, 0.0f,
    1.0,  1.0, 0.0,  0.0f, 0.0f, 1.0f,  1.0f, 1.0f,
   -1.0,  1.0, 0.0,  1.0f, 0.0f, 0.0f,  0.0f, 1.0f
};

Array<unsigned int> indices =
{
    0, 1, 2,
    2, 3, 0,

    4, 5, 6,
    6, 7, 4,

    4, 0, 3,
    3, 7, 4,

    1, 5, 6,
    6, 2, 1,

    0, 1, 5,
    5, 4, 0,

    3, 2, 6,
    6, 7, 3
};

//Array<float> vertices =
//{
//    // pos             // col            // tex coords
//   -1.0, -1.0, 1.0,   1.0f, 0.0f, 0.0f,  0.0f, 0.0f,
//    1.0, -1.0, 1.0,   0.0f, 1.0f, 0.0f,  1.0f, 0.0f,
//    1.0,  1.0, 1.0,   0.0f, 0.0f, 1.0f,  1.0f, 1.0f,
//   -1.0,  1.0, 1.0,   1.0f, 0.0f, 0.0f,  0.0f, 1.0f
//};
//Array<unsigned int> indices =
//{
//    0, 1, 2,
//    2, 3, 0
//};

int main()
{
    vec3 position(0.0f, 0.0f, 3.0f), front(0.0f, 0.0f, -1.0f), up(0.0f, 1.0f, 0.0f), at{ 0.0f }; add(&front, &position, &at);
    vec3 trs = { 0.0f, 0.0f, 0.0f }, rot = vec3(0.0f), scl = { 1.0f, 1.0f, 0.2f };
    int32_t w = 800, h = 600;
    float_t n = 10.0f, f = 1000.0f, aspect = (float_t)w / h, fov = rad(45);
    Window window(w, h, true);

    mat4 model, view, proj, tmp;
    translate(trs, &tmp);
    scale(scl, &model);
    Matrix4x4Multiply(&model, &tmp, &model);

    lookAt(position, at, up, &view);
    perspective(n, f, aspect, fov, &proj);

    Shader shader("Util/Shaders/texture2D.shader");
    Array<Vertex> attribs =
    {
        Vertex({vtype::vec3, dtype::Float, 0,  true}),
        Vertex({vtype::vec3, dtype::Float, 12, true}),
        Vertex({vtype::vec2, dtype::Float, 24, true})
    };
    VBO vertexBuffer(vertices, attribs, USAGE::STATIC);
    IBO indexBuffer(indices, USAGE::STATIC);

    texFormat<2> t0Format = texFormat<2>({ 
        texarg::MIPMAP_NEAREST_GL_NEAREST,
        texarg::MIPMAP_NEAREST_GL_NEAREST,
        texarg::CLAMP_EDGE, 
        texarg::CLAMP_EDGE
        });
    Texture2D tex0 = Texture2D("Assets/wall.jpg", t0Format);

    shader.link();
    shader.use();
    vertexBuffer.bind();
    indexBuffer.bind();
    tex0.bind();

    shader.setUniform1i("u_texture", 0);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LESS);
    int i = 0;
    while (!window.shouldClose())
    {
        glClearColor(0.7f, 0.7f, 0.7f, 0.7f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        shader.setUniformMat4("projection", proj);
        shader.setUniformMat4("view", view);
        shader.setUniformMat4("model", model);


        if (window.keyPressed(GLFW_KEY_W)) { position.z -= window.dt; }
        else if (window.keyPressed(GLFW_KEY_S)) { position.z += window.dt; }
        else if (window.keyPressed(GLFW_KEY_D)) { position.x += window.dt; }
        else if (window.keyPressed(GLFW_KEY_A)) { position.x -= window.dt; }
        else if (window.keyPressed(GLFW_KEY_UP))   { position.y += window.dt; }
        else if (window.keyPressed(GLFW_KEY_DOWN)) { position.y -= window.dt; }
        else if (window.keyPressed(GLFW_KEY_R)) { rot.x += window.dt; }
        else if (window.keyPressed(GLFW_KEY_T)) { rot.y += window.dt; }
        else if (window.keyPressed(GLFW_KEY_Y)) { rot.z += window.dt; }
        else if (window.keyPressed(GLFW_KEY_9)) { fov   += window.dt; }
        else if (window.keyPressed(GLFW_KEY_0)) { fov   -= window.dt; }


        add(&position, &front, &at);
        scale(scl, &model);

        translate(trs, &tmp);
        scale(scl, &model);
        Matrix4x4Multiply(&model, &tmp, &model);

        lookAt(position, at, up, &view);
        if (i % 35 == 0) 
        {
            window.clearConsole();
            std::cout << window.dt;
        }
        glDrawElements(GL_TRIANGLES, indexBuffer.m_count, GL_UNSIGNED_INT, 0);
        window.update();
        i++;
    }


	
};
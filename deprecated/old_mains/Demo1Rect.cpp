#include <glad/glad.h>
#include <glfw3.h>
#include "math_3d.h"
#include "Buffers.h"
#include "Shader.h"
//#include "ResourceManager.h"

// Rectangle
Array<float> vertices = {
    1.0f,  1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
    1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
   -1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
   -1.0f,  1.0f, 0.0f, 0.0f, 0.0f, 0.0f
};

Array<GLuint> indices = {
    0, 1, 3,
    1, 2, 3
};

static unsigned int w = 800, h = 600;
static float near = 10, far = 1000, aspect_ratio = (float)w / (float)h, fov = 1000;
static float t0 = (float)glfwGetTime(), t1 = 0, dt = t1 - t0;

//static m3d::vec4 scrn_col = m3d::random();
static m3d::vec3 scl(0.5f), trs(0.0f), rot(0.0f), eye(0.0f, 0.0f, 3.0f), at(0.0f, 0.0f, -1.0f), up(0, 1, 0);

m3d::mat4x4 model = m3d::scale(scl).matmul(m3d::translate(trs));
m3d::mat4x4 view = m3d::lookAt(eye, at, up);
m3d::mat4x4 persp = m3d::perspective(near, far, aspect_ratio, fov);
m3d::mat4x4 mvp = persp.matmul(view.matmul(model));


void framebuffer_size_callback(GLFWwindow* window, int width, int height) { glViewport(0, 0, width, height); }
void processInput(GLFWwindow* window, m3d::vec3& trs, m3d::vec3& rot) {
    t1 = (float)glfwGetTime();
    dt = t1 - t0;
    float dx = (float)dt * 1.5f;
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) glfwSetWindowShouldClose(window, true);
    else if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) trs.x += dx;
    else if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)  trs.x -= dx;
    else if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)    trs.y += dx;
    else if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)  trs.y -= dx;
    else if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)     rot.x += (float)m3d::PI * dt;
    else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)     rot.x -= (float)m3d::PI * dt;
    else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)     rot.z += (float)m3d::PI * dt;
    else if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)     rot.z -= (float)m3d::PI * dt;
    else if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)     rot.y += (float)m3d::PI * dt;
    else if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)     rot.y -= (float)m3d::PI * dt;
    t0 = t1;
}
void rainbow(m3d::vec3& v) { Array<float> r = m3d::random(3); v = m3d::vec3(r[0], r[1], r[2]); }

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    GLFWwindow* window = glfwCreateWindow(w, h, "your mom", NULL, NULL);

    glfwMakeContextCurrent(window); glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) { std::cout << "Failed to initialize GLAD" << std::endl; return -1; }

    Shader shader = Shader("Util/Shaders/mvp.shader"); // uses default shader path on default constructor.
    shader.link();

    VertexBuffer vb = VertexBuffer(vertices.get_c(), (u_int)vertices.size_bytes());
    IndexBuffer ib = IndexBuffer(indices);
    VertexArray va = VertexArray(false);
    va.push<float>(4, 3, false);
    va.push<u_int>(4, 3, false);

    m3d::vec3 color_change(0.0f);
    bool big = false;

    va.gl_activateAttribs();
    shader.use();
    //glEnable(GL_DEPTH_TEST);
    //glDepthFunc(GL_LESS);
    while (!glfwWindowShouldClose(window)) {
        processInput(window, trs, rot);
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        if (big) {
            color_change -= m3d::vec3(0, dt, dt);
            if (color_change.z <= 0) big = false;
        }
        else {
            color_change += m3d::vec3(0.0f, dt, dt);
            if (color_change.z >= 1) big = true;
        }

        m3d::mat4x4 model = m3d::model(trs, rot, scl);
        mvp = m3d::mvp(persp, view, model);
        shader.use();
        shader.setUniformMat4("u_MVP", model);
        shader.setUniform3f("u_color", color_change);
        va.bind();
        ib.bind();
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);


        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
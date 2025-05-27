#include "bhdr.h"
#include "Shader.h"
#include "Buffers.h"
#include "Camera.h"
#include <Windows.h>


/*
    TODO: 
    Add Texture struct/class
    Add Camera  struct/class
    Add Lighting
*/
void clearConsole();


const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 800;
const float near_ = 1;
const float far_ = 1000;
const float asp = SCR_WIDTH / SCR_HEIGHT;
const float fov = 60;
unsigned int i = 0;
float t0 = (float)glfwGetTime(), t1 = 0, dt = 0;
m3d::mat4x4 model, view, perspective, modelViewProj;
m3d::vec3 translate(0.0f, 0.0f, -10.0f), rot(m3d::PI * 30.0f/180.0f);
m3d::vec3 position(0.0f, 0.0f, 3.0f), front(0.0f, 0.0f, -1.0f), up(0.0f, 1.0f, 0.0f);

Camera camera = Camera(position, front + position, up);

//Array<float> vertices =
//{
//    // pos           // col
//    -1.0f, -1.0f, 0, 1.0f, 0.0f, 0.0f,
//     1.0f, -1.0f, 0, 0.0f, 1.0f, 0.0f,
//     1.0f,  1.0f, 0, 0.0f, 0.0f, 1.0f,
//    -1.0f,  1.0f, 0, 1.0f, 0.0f, 0.0f
//};
Array<float> vertices =
{
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

void framebuffer_size_callback(GLFWwindow* window, int width, int height) { glViewport(0, 0, width, height); }
void update(GLFWwindow* window)
{
    t1 = (float)glfwGetTime();
    dt = t1 - t0;
    t0 = t1;
    float dx = (float)dt * 2.5f;
    float d0 = (float)m3d::PI / 2 * dt;
    
         if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) glfwSetWindowShouldClose(window, true);
    else if (glfwGetKey(window, GLFW_KEY_1     ) == GLFW_PRESS) translate.x += dx;
    else if (glfwGetKey(window, GLFW_KEY_2     ) == GLFW_PRESS) translate.x -= dx;
    else if (glfwGetKey(window, GLFW_KEY_3     ) == GLFW_PRESS) translate.y += dx;
    else if (glfwGetKey(window, GLFW_KEY_4     ) == GLFW_PRESS) translate.y -= dx;
    else if (glfwGetKey(window, GLFW_KEY_5     ) == GLFW_PRESS) translate.z += dx;
    else if (glfwGetKey(window, GLFW_KEY_6     ) == GLFW_PRESS) translate.z -= dx;
    else if (glfwGetKey(window, GLFW_KEY_7     ) == GLFW_PRESS) rot.x += d0;
    else if (glfwGetKey(window, GLFW_KEY_8     ) == GLFW_PRESS) rot.x -= d0;
    else if (glfwGetKey(window, GLFW_KEY_9     ) == GLFW_PRESS) rot.y += d0;
    else if (glfwGetKey(window, GLFW_KEY_0     ) == GLFW_PRESS) rot.y -= d0;
    else if (glfwGetKey(window, GLFW_KEY_MINUS ) == GLFW_PRESS) rot.z += d0;
    else if (glfwGetKey(window, GLFW_KEY_EQUAL ) == GLFW_PRESS) rot.z -= d0;
    else if (glfwGetKey(window, GLFW_KEY_W     ) == GLFW_PRESS) camera.update(dt, MOVEMENT_KEY::W);
    else if (glfwGetKey(window, GLFW_KEY_S     ) == GLFW_PRESS) camera.update(dt, MOVEMENT_KEY::S);
    else if (glfwGetKey(window, GLFW_KEY_A     ) == GLFW_PRESS) camera.update(dt, MOVEMENT_KEY::A);
    else if (glfwGetKey(window, GLFW_KEY_D     ) == GLFW_PRESS) camera.update(dt, MOVEMENT_KEY::D);

    if (i % 35 == 0)
    {
        clearConsole();
        std::cout << "dx: " << dx << "\nd0: " << d0;
        std::cout << "\nrotation vec: " << rot.toString();
        std::cout << "\ntrs      vec: " << translate.toString();
        std::cout << "\nmvp: " << modelViewProj.toString();
        camera.debug();

    }
    perspective = m3d::perspective(near_, far_, asp, fov);
    view = camera.getView();
    model = m3d::rotate(rot).matmul(m3d::translate(translate));
    modelViewProj = perspective.matmul(view.matmul(model));
}


int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "fuck you", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    Shader shader("Util/Shaders/mvp.shader");
    shader.link();
    shader.use();
   
    Array<Vertex> attribs =
    {
        Vertex({vtype::vec3, dtype::Float, 0,  true}),
        Vertex({vtype::vec3, dtype::Float, 12, true}),
    };
    vertices *= 0.5f;
    /*vertices += 0.5f;*/

    VBO vertexBuffer(vertices, attribs, dataUsage::STATIC);
    IBO indexBuffer(indices, dataUsage::STATIC);


    shader.use();
    vertexBuffer.bind();
    indexBuffer.bind();
    glEnable(GL_DEPTH_TEST);
    while (!glfwWindowShouldClose(window))
    {
        glClearColor(0.7f, 0.7f, 0.7f, 0.7f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        update(window);

        shader.setUniformMat4("u_mvp", modelViewProj);
        i++;

        glDrawElements(GL_TRIANGLES, indexBuffer.m_count, GL_UNSIGNED_INT, 0);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}




void clearConsole() {
    HANDLE                     hStdOut;
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    DWORD                      count;
    DWORD                      cellCount;
    COORD                      homeCoords = { 0, 0 };

    hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hStdOut == INVALID_HANDLE_VALUE) return;

    /* Get the number of cells in the current buffer */
    if (!GetConsoleScreenBufferInfo(hStdOut, &csbi)) return;
    cellCount = csbi.dwSize.X * csbi.dwSize.Y;

    /* Fill the entire buffer with spaces */
    if (!FillConsoleOutputCharacter(
        hStdOut,
        (TCHAR)' ',
        cellCount,
        homeCoords,
        &count
    )) return;

    /* Fill the entire buffer with the current colors and attributes */
    if (!FillConsoleOutputAttribute(
        hStdOut,
        csbi.wAttributes,
        cellCount,
        homeCoords,
        &count
    )) return;
    SetConsoleCursorPosition(hStdOut, homeCoords);
}
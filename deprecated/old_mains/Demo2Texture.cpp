#include <glad/glad.h>
#include <glfw3.h>
#include "Shader.h"
#include "Texture.h"
#include <Windows.h>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
void ClearScreen();

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;
float t0 = (float)glfwGetTime(), t1 = 0, dt = t1 - t0;
int i = 0;
m3d::vec3 translate(0.0f), rotate({0.0f, 90.0f, 0.0f}), scale(0.7f);
m3d::mat4x4 model = m3d::scale(scale).matmul(m3d::translate(translate));
/*
    Why you don't see the textured quad at first [for m3d::vec3 rotate(0.0f)]:
    The m3d::rotate cancels out the x component of the matrix.
    Then, what happens is that the vector becomes (0, y, z) which doesn't let you see it.
    when you rotate around the y axis, it starts showing the vector because its x applies to:
    -1 < x < 1
*/

int main()
{

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
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

    Shader shader("Util/Shaders/texture2D.shader");
    shader.link();
    shader.use();

    float AAvertices[] =
    {
        // pos           // col            // tex coords
        -1.0f, -1.0f, 0, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
         1.0f, -1.0f, 0, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
         1.0f,  1.0f, 0, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
        -1.0f,  1.0f, 0, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f
    };

    unsigned int AAindices[] =
    {
        0, 1, 2,
        2, 3, 0
    };
    unsigned int VBO, VAO, EBO, TEX;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(AAvertices), AAvertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(AAindices), AAindices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3*sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    // loading texture
    int x, y, channels, mipmapLevel = 0;
    GLenum gpuFormat = GL_RGBA8, channelsFormat = GL_RGBA;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* data = stbi_load("Assets/wall.jpg", &x, &y, &channels, 0);
    glGenTextures(1, &TEX);
    glBindTexture(GL_TEXTURE_2D, TEX);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    if (channels == 3) { gpuFormat = GL_RGB8; channelsFormat = GL_RGB; }
    mipmapLevel = (int)log2(x);

    for(int i = 0; i<mipmapLevel; i++)
        glTexImage2D(GL_TEXTURE_2D, i, gpuFormat, x, y, 0, channelsFormat, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
    stbi_image_free(data);


    shader.setUniform1i("u_texture", 0);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, TEX);


    float a = 256;
    bool sub = false;
    while (!glfwWindowShouldClose(window))
    {
        processInput(window);
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        if (a >= 256) sub = true;
        else if (a <= -256) sub = false;

        if(!sub) a += 0.1f;
        if (sub) a -= 0.1f;

        shader.use();
        shader.setUniformMat4("u_mvp", model);
        shader.setUniform3f("u_color", {0.0f, a/256, 0.0f});
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glfwTerminate();
    return 0;
}

void processInput(GLFWwindow* window) {
    t1 = (float)glfwGetTime();
    dt = t1 - t0;
    float dx = (float)dt * 1.5f;
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) glfwSetWindowShouldClose(window, true);
    else if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) translate.x += dx;
    else if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)  translate.x -= dx;
    else if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)    translate.y += dx;
    else if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)  translate.y -= dx;
    else if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)     rotate.x += (float)m3d::PI * dt;
    else if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)     rotate.x -= (float)m3d::PI * dt;
    else if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS)     rotate.y += (float)m3d::PI * dt;
    else if (glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS)     rotate.y -= (float)m3d::PI * dt;
    else if (glfwGetKey(window, GLFW_KEY_5) == GLFW_PRESS)     rotate.z += (float)m3d::PI * dt;
    else if (glfwGetKey(window, GLFW_KEY_6) == GLFW_PRESS)     rotate.z -= (float)m3d::PI * dt;
    t0 = t1;
    model = m3d::scale(scale).matmul(m3d::translate(translate).matmul(m3d::rotate(rotate.x, rotate.y, rotate.z)));
    std::string msg = model.toString();
    if (i % 20 == 0) {
        ClearScreen();
        std::cout << msg;
    }
    i++;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void ClearScreen() {
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
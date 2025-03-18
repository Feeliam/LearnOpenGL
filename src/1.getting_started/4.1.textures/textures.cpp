#include <glad/glad.h>    // OpenGL函数加载器
#include <GLFW/glfw3.h>   // OpenGL窗口和上下文管理
#include <stb_image.h>    // 图片加载库

#include <learnopengl/filesystem.h>  // 文件系统工具
#include <learnopengl/shader_s.h>    // 着色器类

#include <iostream>

// 函数声明
// 窗口大小改变时的回调函数
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
// 处理输入的函数
void processInput(GLFWwindow *window);

// 窗口设置
const unsigned int SCR_WIDTH = 800;   // 窗口宽度
const unsigned int SCR_HEIGHT = 600;  // 窗口高度

int main()
{
    // GLFW初始化和配置
    // ------------------------------
    glfwInit();
    // 设置OpenGL版本为3.3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    // 设置OpenGL核心模式
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // 创建GLFW窗口
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // 加载所有OpenGL函数指针
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // 构建和编译着色器程序
    // ------------------------------------
    Shader ourShader("bin/1.getting_started/4.1.texture.vs", "bin/1.getting_started/4.1.texture.fs"); 

    // 设置顶点数据（和缓冲区）并配置顶点属性
    // ------------------------------------------------------------------
    float vertices[] = {
        // 位置              // 颜色             // 纹理坐标
         0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // 右上
         0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // 右下
        -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // 左下
        -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // 左上 
    };
    unsigned int indices[] = {  
        0, 1, 3, // 第一个三角形
        1, 2, 3  // 第二个三角形
    };

    // 创建和配置VAO、VBO和EBO
    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);    // 生成顶点数组对象
    glGenBuffers(1, &VBO);         // 生成顶点缓冲对象
    glGenBuffers(1, &EBO);         // 生成索引缓冲对象

    // 绑定VAO
    glBindVertexArray(VAO);

    // 绑定并填充VBO
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // 绑定并填充EBO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // 设置顶点属性
    // 位置属性
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // 颜色属性
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // 纹理坐标属性
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    // 加载和创建纹理 
    // -------------------------
    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture); // 所有GL_TEXTURE_2D操作将影响此纹理对象
    // 设置纹理环绕方式
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// 设置纹理环绕方式为GL_REPEAT（默认方式）
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // 设置纹理过滤方式
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // 加载图片，创建纹理并生成mipmap
    int width, height, nrChannels;
    // FileSystem::getPath(...)是GitHub仓库的一部分，可以在任何IDE/平台上找到文件；替换为你自己的图片路径。
    unsigned char *data = stbi_load(FileSystem::getPath("resources/textures/container.jpg").c_str(), &width, &height, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);

    // 渲染循环
    while (!glfwWindowShouldClose(window))
    {
        // 处理输入
        processInput(window);

        // 渲染指令
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);  // 设置清屏颜色
        glClear(GL_COLOR_BUFFER_BIT);           // 清除颜色缓冲

        // 绑定纹理
        // glActiveTexture(GL_TEXTURE0); // 在绑定纹理之前先激活纹理单元
        glBindTexture(GL_TEXTURE_2D, texture);

        // 激活着色器程序
        ourShader.use();
        // 绑定VAO并绘制
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        // 交换缓冲并查询IO事件
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // 可选：在所有资源释放后进行清理
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);

    // 终止GLFW，清理所有之前分配的GLFW资源
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

/**
 * 处理所有输入：查询当前帧是否有按键被按下/释放，并作出相应反应
 * @param window GLFW窗口指针
 */
void processInput(GLFWwindow *window)
{
    // 按ESC键关闭窗口
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

/**
 * GLFW回调函数：当窗口大小改变时(由OS或用户调整)该回调函数就会执行
 * @param window GLFW窗口指针
 * @param width  新的窗口宽度
 * @param height 新的窗口高度
 */
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // 确保视口与新的窗口尺寸匹配
    // 注意：在视网膜显示器上，宽度和高度会明显大于指定的尺寸
    glViewport(0, 0, width, height);
}

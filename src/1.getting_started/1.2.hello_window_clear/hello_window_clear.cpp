// 包含必要的头文件
#include <glad/glad.h>     // OpenGL函数加载器
#include <GLFW/glfw3.h>    // 窗口管理和OpenGL上下文创建
#include <iostream>        // 标准输入输出流

// 函数声明
// 当窗口大小改变时的回调函数
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
// 处理输入事件的函数
void processInput(GLFWwindow *window);

// 窗口设置
const unsigned int SCR_WIDTH = 800;    // 窗口宽度
const unsigned int SCR_HEIGHT = 600;   // 窗口高度

int main()
{
    // 初始化GLFW并进行配置
    // ------------------------------
    glfwInit();    // 初始化GLFW库
    // 设置OpenGL版本为3.3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);    // 主版本号
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);    // 次版本号
    // 使用核心模式(Core-profile)
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // 针对Mac OS X系统的特殊处理
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // 创建GLFW窗口
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    // 检查窗口是否创建成功
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    // 将窗口的上下文设置为当前线程的主上下文
    glfwMakeContextCurrent(window);
    // 注册窗口大小改变的回调函数
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);// 注册这个自定义的函数 framebuffer_size_callback

    // 初始化GLAD
    // ---------------------------------------
    // GLAD用于管理OpenGL的函数指针 
    // 给GLAD传入了用来加载系统相关的OpenGL函数指针地址的函数
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))//glfwGetProcAddress是GLFW的函数，用于返回一个函数指针
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }    

    // 渲染循环
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        // 输入处理
        // -----
        processInput(window);

        // 渲染指令
        // ------
        // 状态设置函数
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);    // 设置清屏颜色（深青色）
        // 状态使用函数
        glClear(GL_COLOR_BUFFER_BIT);             // 清除颜色缓冲

        // 交换缓冲和轮询IO事件
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);    // 交换颜色缓冲（双缓冲）
        glfwPollEvents();           // 检查是否有触发事件（键盘输入、鼠标移动等）
    }

    // 终止程序
    // ------------------------------------------------------------------
    glfwTerminate();    // 清理所有GLFW资源
    return 0;
}

// 处理输入函数
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
    // 检查用户是否按下ESC键
    // 如果按下ESC键，将窗口设置为关闭状态
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

// 窗口大小改变回调函数
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // 确保视口与新的窗口尺寸匹配
    // 注意：对于视网膜(Retina)显示器，width和height会明显大于原输入值
    glViewport(0, 0, width, height);    // 设置OpenGL渲染窗口的位置和大小
}
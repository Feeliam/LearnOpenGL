/*
 * OpenGL主要组件说明：
 * GLFW：用于创建OpenGL上下文和管理窗口
 * GLAD：用于管理OpenGL函数指针
 * VAO（顶点数组对象）：存储顶点属性配置和VBO的关联
 * VBO（顶点缓冲对象）：在GPU内存中存储顶点数据
 * 着色器：在GPU上运行的程序，处理图形渲染管线的不同阶段
 */

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

// 函数声明部分
// --------------------------------------
/*
 * framebuffer_size_callback：窗口大小改变时的回调函数
 * @param window：当前窗口的指针
 * @param width：新的窗口宽度
 * @param height：新的窗口高度
 * 作用：当窗口大小改变时，自动调整视口大小以匹配新窗口尺寸
 */
void framebuffer_size_callback(GLFWwindow* window, int width, int height);

/*
 * processInput：处理输入事件的函数
 * @param window：当前窗口的指针
 * 作用：检测键盘输入并作出相应反应，例如按ESC键关闭窗口
 */
void processInput(GLFWwindow *window);

// 窗口设置
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// 顶点着色器源码
const char *vertexShaderSource = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "}\0";
// 片段着色器源码
const char *fragmentShaderSource = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
    "}\n\0";

int main()
{
    // GLFW配置和初始化
    // --------------------------------------
    /*
     * glfwInit：初始化GLFW库
     * glfwWindowHint：配置GLFW
     * - GLFW_CONTEXT_VERSION_MAJOR, MINOR：设置OpenGL版本为3.3
     * - GLFW_OPENGL_PROFILE：使用核心模式，强制使用现代OpenGL
     */
    glfwInit();
    // 设置OpenGL版本为3.3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // 创建GLFW窗口
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
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    /*
     * 着色器编译和链接过程
     * 1. 创建着色器对象
     * 2. 将源码附加到着色器对象
     * 3. 编译着色器
     * 4. 创建着色器程序
     * 5. 将编译好的着色器附加到程序
     * 6. 链接程序
     * 这个过程是创建GPU可执行程序的必要步骤
     */
    // 创建和编译着色器程序
    // ------------------------------------
    // 顶点着色器
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    // 检查着色器编译错误
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "错误::着色器::顶点::编译失败\n" << infoLog << std::endl;
    }

    // 片段着色器
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    // 检查着色器编译错误
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "错误::着色器::片段::编译失败\n" << infoLog << std::endl;
    }

    // 链接着色器
    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    // 检查链接错误
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "错误::着色器::程序::链接失败\n" << infoLog << std::endl;
    }
    // 删除着色器对象
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    /*
     * 顶点数据处理
     * 1. 创建并绑定VAO：存储所有顶点属性设置
     * 2. 创建并绑定VBO：将顶点数据发送到GPU
     * 3. 设置顶点属性：告诉OpenGL如何解析顶点数据
     * VAO和VBO的配合使用能够高效地管理和渲染3D图形数据
     */
    // 设置顶点数据和配置顶点属性
    // ------------------------------------------------------------------
    float vertices[] = {
        -0.5f, -0.5f, 0.0f, // 左下
         0.5f, -0.5f, 0.0f, // 右下
         0.0f,  0.5f, 0.0f  // 顶部
    }; 

    // 创建VAO和VBO
    unsigned int VBO, VAO;
    // VBO 只负责存储顶点数据，VAO存储顶点数据的具体属性配置
    glGenVertexArrays(1, &VAO); // 创建1个VAO 并保存其ID到  unsign int VAO 
    glGenBuffers(1, &VBO); // 创建1个VBO 并保存其ID到  unsign int VBO
    // 首先绑定VAO，然后绑定和设置VBO，最后配置顶点属性
    //从绑定之后起，我们应该绑定和配置对应的VBO和属性指针，之后解绑VAO供之后使用。当我们打算绘制一个物体的时候，我们只要在绘制物体前简单地把VAO绑定到希望使用的设定上就行
    glBindVertexArray(VAO);

    // 将顶点数据复制到缓冲中供OpenGL使用
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // 设置顶点属性指针， 第一个参数是顶点属性的位置值（这个值对应于顶点着色器中的location关键字）
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0); // 启用顶点属性

    // 允许调用glVertexAttribPointer注册VBO作为顶点属性的绑定顶点缓冲对象后解绑
    glBindBuffer(GL_ARRAY_BUFFER, 0); //解绑VBO

    // 你可以在之后解绑VAO，这样其他VAO调用不会意外修改 这个VAO，但这很少发生。修改其他VAO需要调用glBindVertexArray，所以我们通常不直接解绑VAO（也不解绑VBO），除非直接需要。
    glBindVertexArray(0);  // 解绑VAO

    // 取消注释这行代码以绘制线框多边形。
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    /*
     * 渲染循环的关键步骤：
     * 1. 处理输入
     * 2. 清除颜色缓冲
     * 3. 激活着色器程序
     * 4. 绑定VAO
     * 5. 绘制图形
     * 6. 交换缓冲并处理事件
     * 这个循环会一直执行直到窗口被关闭
     */
    // 渲染循环
    while (!glfwWindowShouldClose(window))
    {
        // 输入处理
        processInput(window);

        // 渲染指令
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f); // 设置清屏颜色
        glClear(GL_COLOR_BUFFER_BIT);          // 清除颜色缓冲

        // 绘制三角形
        glUseProgram(shaderProgram); // 激活着色器程序
        glBindVertexArray(VAO);     // 绑定VAO
        // 绘制三角形 从顶点0开始绘制3个顶点
        glDrawArrays(GL_TRIANGLES, 0, 3);

        // 交换缓冲并查询IO事件
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    /*
     * 资源清理
     * - 删除VAO
     * - 删除VBO
     * - 删除着色器程序
     * - 终止GLFW
     * 正确的资源清理可以防止内存泄漏
     */
    // 清理资源
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);

    // 终止GLFW
    glfwTerminate();
    return 0;
}

// 处理输入：查询当前帧是否有相关按键被按下/释放，并作出相应反应
void processInput(GLFWwindow *window)
{
    /*
     * glfwGetKey：检查特定键是否被按下
     * GLFW_KEY_ESCAPE：ESC键的键码
     * GLFW_PRESS：按下状态
     * glfwSetWindowShouldClose：设置窗口关闭标志
     * 
     * 这个函数实现了按ESC键退出程序的功能
     */
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

// GLFW回调函数：当窗口大小改变时执行
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    /*
     * glViewport：设置渲染窗口的尺寸
     * 参数：x偏移，y偏移，窗口宽度，窗口高度
     * 
     * OpenGL会用这个视口大小进行裁剪，并将裁剪后的结果映射到窗口上
     * 这个函数确保图形正确地缩放到新窗口大小
     */
    // 确保视口与新的窗口尺寸匹配
    glViewport(0, 0, width, height);
}
/*
 * 索引绘制示例程序
 * 主要演示了如何使用EBO(Element Buffer Object)通过索引绘制图形
 * 相比普通绘制方式，索引绘制可以重用顶点数据，节省内存
 */

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

// 函数声明
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

// 窗口设置
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// 着色器源码
const char *vertexShaderSource = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "}\0";
const char *fragmentShaderSource = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
    "}\n\0";

int main()
{
    // GLFW初始化和配置
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw窗口创建
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

    // glad: 加载所有OpenGL函数指针
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }


    // 构建和编译我们的着色器程序
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
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
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
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
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
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // 顶点数据设置
    // ------------------------------------------------------------------
    /*
     * vertices数组：定义矩形的四个顶点
     * - 按照右上、右下、左下、左上的顺序存储
     * - 每个顶点包含3个float值表示xyz坐标
     */
    float vertices[] = {
         0.5f,  0.5f, 0.0f,  // 右上角 (顶点0)
         0.5f, -0.5f, 0.0f,  // 右下角 (顶点1)
        -0.5f, -0.5f, 0.0f,  // 左下角 (顶点2)
        -0.5f,  0.5f, 0.0f   // 左上角 (顶点3)
    };
    
    /*
     * indices数组：定义绘制顺序
     * - 通过顶点索引定义两个三角形
     * - 第一个三角形：0->1->3
     * - 第二个三角形：1->2->3
     * 使用索引可以复用顶点数据，4个顶点就能组成一个矩形
     */
    unsigned int indices[] = {  
        0, 1, 3,  // 第一个三角形
        1, 2, 3   // 第二个三角形
    };

    // 创建并配置缓冲对象
    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);  // 创建VAO
    glGenBuffers(1, &VBO);       // 创建VBO
    glGenBuffers(1, &EBO);       // 创建EBO

    // 绑定VAO
    glBindVertexArray(VAO);

    // 配置顶点缓冲
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // 配置索引缓冲
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // 设置顶点属性
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // 注意：不要在VAO处于活动状态时解绑EBO，因为EBO的绑定状态是存储在VAO中的
    // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    // 渲染循环
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        // 处理输入
        // -----
        processInput(window);

        // 渲染
        // ------
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // 绘制矩形
        glUseProgram(shaderProgram);
        glBindVertexArray(VAO); // 由于我们只有一个VAO，所以不需要每次都绑定它，但我们会这样做以保持代码的组织性
        /*
         * glDrawElements：使用索引绘制
         * 参数1：图元类型
         * 参数2：索引数量
         * 参数3：索引数据类型
         * 参数4：索引缓冲偏移量
         */
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        // glBindVertexArray(0); // 不需要每次都解绑它
 
        // glfw: 交换缓冲和轮询事件（按键按下/释放，鼠标移动等）
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // 资源清理
    // ------------------------------------------------------------------------
    /*
     * 按顺序删除：
     * 1. VAO
     * 2. VBO
     * 3. EBO
     * 4. 着色器程序
     */
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteProgram(shaderProgram);

    // 终止GLFW
    glfwTerminate();
    return 0;
}

/*
 * processInput：输入处理函数
 * 功能：检测ESC键是否按下，按下则关闭窗口
 */
void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

/*
 * framebuffer_size_callback：窗口大小改变回调
 * 功能：当窗口大小改变时更新视口尺寸
 */
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // 确保视口与新窗口尺寸匹配；注意宽度和高度在视网膜显示器上会显著增大
    glViewport(0, 0, width, height);
}
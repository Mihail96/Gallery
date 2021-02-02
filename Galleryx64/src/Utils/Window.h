#ifndef Window_H
#define Window_H

class Window
{
public:
    const static unsigned int SCR_WIDTH = 1000;
    const static unsigned int SCR_HEIGHT = 800;
    static int Result;
    static GLFWwindow* window;

    static Window& GetInstance()
    {
        static Window instance;
        return instance;
    }

private:
    Window();

    Window(Window const&) = delete;
    void operator=(Window const&) = delete;
};

#endif
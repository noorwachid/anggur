#include "Window.h"
#include "Anggur/Graphics/Function.h"
#include "Internal.h"
#include <cassert>

namespace Anggur
{
    Window::Window(const Vector2 &newSize, const std::string &newTitle) : size(newSize), title(newTitle)
    {
#ifdef ANGGUR_OS_X
        glfwInitHint(GLFW_COCOA_CHDIR_RESOURCES, 0);
#endif

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef ANGGUR_OS_X
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

        context = glfwCreateWindow(size.x, size.y, title.c_str(), nullptr, nullptr);
        assert(context && "[Window] Failed to create a window");

        bindContext();
        bindGraphics();

        initializeGraphics();

        int frameBufferWidth = 0;
        int frameBufferHeight = 0;
        glfwGetFramebufferSize(context, &frameBufferWidth, &frameBufferHeight);
        frameBufferSize.set(frameBufferWidth, frameBufferHeight);

        glfwSetWindowSizeCallback(context, [](GLFWwindow *context, int width, int height) {
            auto window = static_cast<Window *>(glfwGetWindowUserPointer(context));
            Vector2 size(width, height);
            WindowSizeEvent event("WindowResized", size);
            window->emitter.emit(event);
            window->size = size;
        });

        glfwSetWindowPosCallback(context, [](GLFWwindow *context, int x, int y) {
            auto window = static_cast<Window *>(glfwGetWindowUserPointer(context));
            Vector2 position(x, y);
            WindowSizeEvent event("WindowMoved", position);
            window->emitter.emit(event);
            window->position = position;
        });

        glfwSetFramebufferSizeCallback(context, [](GLFWwindow *context, int width, int height) {
            auto window = static_cast<Window *>(glfwGetWindowUserPointer(context));
            Vector2 size(width, height);
            FrameBufferSizeEvent event("FrameBufferResized", size);
            window->emitter.emit(event);
            window->frameBufferSize = size;
        });
    }

    Window::~Window()
    {
        if (context)
            glfwDestroyWindow(context);
    }

    WindowContext *Window::getContext()
    {
        return context;
    }

    float Window::getAspectRatio()
    {
        return size.y / size.x;
    }

    const Vector2 &Window::getCursorPosition()
    {
        return position;
    }

    const Vector2 &Window::getSize()
    {
        return size;
    }

    const Vector2 &Window::getFrameBufferSize()
    {
        return frameBufferSize;
    }

    const std::string &Window::getTitle()
    {
        return title;
    }

    void Window::setCursorPosition(const Vector2 &position)
    {
        glfwSetWindowPos(context, position.x, position.y);
    }

    void Window::setSize(const Vector2 &size)
    {
        glfwSetWindowSize(context, size.x, size.y);
    }

    void Window::setTitle(const std::string &newTitle)
    {
        glfwSetWindowTitle(context, newTitle.c_str());
        title = newTitle;
    }

    bool Window::isOpen()
    {
        return !glfwWindowShouldClose(context);
    }

    void Window::close()
    {
        glfwSetWindowShouldClose(context, true);
    }

    void Window::update()
    {
        bindGraphics();
        swapFrameBuffers();

        for (InputDevice *input : inputs)
            input->update();
    }

    void Window::registerInputDevice(InputDevice *input)
    {
        inputs.push_back(input);
    }

    void Window::initializeGraphics()
    {
        bool result = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
        assert(result && "[Window] Failed to load graphic functions");
    }

    void Window::bindGraphics()
    {
        glfwMakeContextCurrent(context);
    }

    void Window::swapFrameBuffers()
    {
        glfwSwapBuffers(context);
    }

    void Window::bindContext()
    {
        glfwSetWindowUserPointer(context, this);
    }
}

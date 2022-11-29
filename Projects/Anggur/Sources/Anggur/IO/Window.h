#pragma once

#include "Anggur/EventEmitter.h"
#include "Input.h"
#include "WindowContext.h"
#include "WindowEvent.h"

namespace Anggur
{
    class Window
    {
      public:
        EventDispatcher eventDispatcher;

      public:
        Window(const Vector2 &size = Vector2(800, 600), const std::string &title = "");

        ~Window();

        /**
         * Get native context
         */
        WindowContext *getContext();

        /**
         * Get aspect ratio
         */
        float getAspectRatio();

        /**
         * Get cursor position
         */
        const Vector2 &getCursorPosition();

        /**
         * Get size
         */
        const Vector2 &getSize();

        const Vector2 &getFrameBufferSize();

        const std::string &getTitle();

        void setCursorPosition(const Vector2 &pos);

        void setSize(const Vector2 &size);

        void setTitle(const std::string &title);

        bool isOpen();

        void close();

        void update();

        template <class InputDevice> InputDevice &getInputDevice()
        {
            static InputDevice device(*this);

            return device;
        }

        void registerInputDevice(InputDevice *device);

      private:
        void initializeGraphics();

        void bindGraphics();

        void bindContext();

        void swapFrameBuffers();

      private:
        WindowContext *context;
        Vector2 position;
        Vector2 size;
        Vector2 frameBufferSize;
        std::string title;

        std::vector<InputDevice *> inputs;
    };
}

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
        Window(const Vector2& size = Vector2(800, 600), const std::string& title = "");

        ~Window();

        /**
         * Get native context
         */
        WindowContext* GetContext();

        /**
         * Get aspect ratio
         */
        float GetAspectRatio();

        /**
         * Get cursor position
         */
        const Vector2& GetCursorPosition();

        /**
         * Get size
         */
        const Vector2& GetSize();

        const Vector2& GetFrameBufferSize();

        const std::string& GetTitle();

        void SetCursorPosition(const Vector2& pos);

        void SetSize(const Vector2& size);

        void SetTitle(const std::string& title);

        bool IsOpen();

        void Close();

        void Update();

        template <class InputDevice> InputDevice& GetInputDevice()
        {
            static InputDevice device(*this);

            return device;
        }

        void RegisterInputDevice(InputDevice* device);

      private:
        void InitializeGraphicsAPI();

        void BindGraphicsAPI();

        void BindContext();

        void SwapFrameBuffers();

      private:
        WindowContext* context;
        Vector2 position;
        Vector2 size;
        Vector2 frameBufferSize;
        std::string title;

        std::vector<InputDevice*> inputs;
    };
}

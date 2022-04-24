#include <Anggur/Graphics/Function.h>

class RxState {
public:
    static void setClearColor(float r, float g, float b, float a = 1.0f) {
		glClearColor(0.1, 0.1, 0.1, 1.0);
    }

    static void clear() {
		glClear(GL_COLOR_BUFFER_BIT);
    }
};
#include <SDL.h>

#define FRAME_VALUES 10

class FPSCounter {
    public:
        void begin();
        void end();

        float getFPS() {return m_FPS;}
    private:
        float m_FPS;
        float m_frameTime;
        unsigned int m_startTicks;
};
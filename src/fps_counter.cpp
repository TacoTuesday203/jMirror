#include "fps_counter.h"

void FPSCounter::begin() {
    m_startTicks = SDL_GetTicks();
}

void FPSCounter::end() {
    static const int NUM_SAMPLES = 10;
    static float frameTimes[NUM_SAMPLES];
    static int currentFrame = 0;

    static float prevTicks = SDL_GetTicks();
    float currentTicks = SDL_GetTicks();

    m_frameTime = currentTicks - prevTicks;
    frameTimes[currentFrame % NUM_SAMPLES] = m_frameTime;

    prevTicks = currentTicks;

    int count;

    if (currentFrame < NUM_SAMPLES) {
        count = currentFrame;
    } else {
        count = NUM_SAMPLES;
    }

    float frameTimeAverage = 0;
    for (int i = 0; i < count; i++) {
        frameTimeAverage += frameTimes[i];
    }

    frameTimeAverage /= count;

    if (frameTimeAverage > 0) {
        m_FPS = 1000.0f / frameTimeAverage;
    } else {
        m_FPS = 60.0f;
    }
    currentFrame++;
}
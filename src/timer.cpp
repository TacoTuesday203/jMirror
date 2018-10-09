#include "timer.h"

void Timer::setTime(int ms) {
        m_time = ms;
    }

void Timer::begin() {
    m_running = true;
    m_clock = std::chrono::system_clock::now();
}

void Timer::update() {
    std::chrono::time_point<std::chrono::system_clock> time_now = std::chrono::system_clock::now();
    if (!(m_finished) && m_running) {
        if (abs(std::chrono::duration_cast<std::chrono::milliseconds>(m_clock - time_now).count()) > m_time) {
            m_finished = true;
        }
    }
}

void Timer::reset() {
    m_running = false;
    m_finished = false;
}
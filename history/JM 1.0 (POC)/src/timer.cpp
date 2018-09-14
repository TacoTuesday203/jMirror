#include "timer.h"

void Timer::setTime(int ms) {
        m_time = ms;
    }

void Timer::begin() {
    m_running = true;
    m_clock = clock();
}

void Timer::update() {
    if (!(m_finished) && m_running) {
        //std::cout << (clock() - m_clock) / (CLOCKS_PER_SEC / 1000)<< std::endl;
        if ((clock() - m_clock) / (CLOCKS_PER_SEC / 1000) > m_time) {
            m_finished = true;
        }
    }
}

void Timer::reset() {
    m_running = false;
    m_finished = false;
}
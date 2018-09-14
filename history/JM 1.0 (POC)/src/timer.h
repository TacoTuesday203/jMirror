#include <ctime>
#include <iostream>

class Timer {
        public:
            void setTime(int ms);
            void begin();
            void update();
            bool isFinished() {return m_finished;}
            bool isRunning() {return m_running;}
            void reset();
        private:
            clock_t m_clock;
            unsigned int m_time;
            bool m_running = false;
            bool m_finished = false;
    };
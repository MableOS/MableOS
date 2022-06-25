#include <Core/EventLoop.hpp>

namespace Core
{
    int EventLoop::run()
    {
        while (!m_should_close)
            ;
        return m_exit_code;
    }
} // namespace Core
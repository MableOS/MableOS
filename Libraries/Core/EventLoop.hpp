#pragma once

namespace Core
{
    class EventLoop
    {
    public:
        int run();

    private:
        bool m_should_close{false};
        int m_exit_code{-1};
    };
} // namespace Core
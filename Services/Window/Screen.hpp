#pragma once

namespace Window
{
    class Screen
    {
    public:
        Screen(int width, int height);

    private:
        int m_fd{-1};
        int m_width{-1};
        int m_height{-1};
        char *m_buffer{nullptr};

        void set_resolution(int width, int height);
    };
} // namespace Window
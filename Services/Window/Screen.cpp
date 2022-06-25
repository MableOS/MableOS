#include <Core/Log.hpp>
#include <Window/Screen.hpp>
#include <fcntl.h>
#include <linux/fb.h>
#include <linux/kd.h>
#include <sys/ioctl.h>
#include <sys/mman.h>

namespace Window
{
    Screen::Screen(int width, int height)
    {
        int fd = open("/dev/tty0", O_RDWR);
        if (fd < 0)
        {
            Core::Log::perror("open()");
            assert(false);
        }

        if (ioctl(fd, KDSETMODE, KD_GRAPHICS) < 0)
        {
            Core::Log::perror("ioctl()");
            assert(false);
        }
        Core::Log::info("Changed /dev/tty0 to graphical mode");

        m_fd = open("/dev/fb0", O_RDWR);
        if (m_fd < 0)
        {
            Core::Log::perror("open()");
            assert(false);
        }

        set_resolution(width, height);
    }

    void Screen::set_resolution(int width, int height)
    {
        if (m_width == width && m_height == height)
            return;

        struct fb_var_screeninfo vinfo = {};
        if (ioctl(m_fd, FBIOGET_VSCREENINFO, &vinfo) < 0)
        {
            Core::Log::perror("ioctl()");
            assert(false);
        }

        auto screen_size = vinfo.xres_virtual * vinfo.yres_virtual * vinfo.bits_per_pixel / 8;
        if (m_buffer != nullptr)
        {
            auto result = munmap(m_buffer, screen_size);
            if (result < 0)
            {
                Core::Log::perror("munmap()");
                assert(false);
            }
        }

        vinfo.xres = width;
        vinfo.yres = height;

        if (ioctl(m_fd, FBIOPUT_VSCREENINFO, &vinfo) < 0)
        {
            Core::Log::perror("ioctl()");
            assert(false);
        }

        m_buffer = static_cast<char *>(mmap(nullptr, screen_size, PROT_READ | PROT_WRITE, MAP_SHARED, m_fd, 0));
        if (m_buffer == MAP_FAILED)
        {
            Core::Log::perror("mmap()");
            assert(false);
        }

        m_width = width;
        m_height = height;
        Core::Log::info("Updated resolution to %dx%d", width, height);
    }
} // namespace Window
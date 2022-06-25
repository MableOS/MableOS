#include <Core/EventLoop.hpp>
#include <Core/Log.hpp>
#include <memory>
#include <sys/wait.h>
#include <unistd.h>

void system_mount_all_fs()
{
    pid_t pid = fork();
    if (pid < 0)
    {
        Core::Log::perror("fork()");
        assert(false);
    }
    else if (!pid)
    {
        auto result = execl("/bin/mount", "mount", "-a", nullptr);
        if (result < 0)
        {
            Core::Log::perror("execl()");
            assert(false);
        }
    }
    else
        wait(nullptr);

    Core::Log::info("Mounted all filesystems");
}

int main()
{
    system_mount_all_fs();

    auto event_loop = std::make_unique<Core::EventLoop>();
    return event_loop->run();
}
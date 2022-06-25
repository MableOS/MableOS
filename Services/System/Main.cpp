#include <Core/EventLoop.hpp>
#include <cstdio>
#include <cstdlib>
#include <memory>
#include <sys/wait.h>
#include <unistd.h>

void system_mount_all_fs()
{
    pid_t pid = fork();
    if (pid < 0)
    {
        std::perror("fork()");
        std::exit(pid);
    }
    else if (!pid)
    {
        auto result = execl("/bin/mount", "mount", "-a", nullptr);
        if (result < 0)
        {
            std::perror("execl()");
            std::exit(result);
        }
    }
    else
        wait(nullptr);
}

int main()
{
    system_mount_all_fs();

    auto event_loop = std::make_unique<Core::EventLoop>();
    return event_loop->run();
}
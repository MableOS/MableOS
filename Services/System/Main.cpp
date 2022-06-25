#include <Core/EventLoop.hpp>
#include <Core/Log.hpp>
#include <System/Service.hpp>
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

    std::vector<System::Service> services;
    auto ini = new Core::Ini("/etc/Services/System.ini");
    for (const auto &name : ini->sections())
        services.emplace_back(name, ini);

    for (auto service : services)
        service.run();
    Core::Log::info("Started %d services", services.size());

    return event_loop->run();
}
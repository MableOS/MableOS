#include <Core/Log.hpp>
#include <System/Service.hpp>
#include <unistd.h>

namespace System
{
    Service::Service(std::string name, Core::Ini *ini)
    {
        m_name = std::move(name);
        m_path = ini->read_string(m_name, "Path");
    }

    void Service::run()
    {
        pid_t pid = fork();
        if (pid < 0)
        {
            Core::Log::perror("fork()");
            assert(false);
        }
        else if (!pid)
        {
            char *argv[] = {const_cast<char *>(m_path.c_str()), nullptr};
            int result = execv(argv[0], argv);
            if (result < 0)
            {
                Core::Log::perror("execv()");
                assert(false);
            }
        }
    }
} // namespace System
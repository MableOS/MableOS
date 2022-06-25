#pragma once

#include <Core/Ini.hpp>

namespace System
{
    class Service
    {
    public:
        Service(std::string name, Core::Ini *ini);

        void run();

    private:
        std::string m_name{};
        std::string m_path{};
    };
} // namespace System
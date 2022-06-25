#include <cstring>
#include <fstream>
#include <iostream>
#include <sstream>
#include <sys/mount.h>
#include <vector>

#define MOUNT_TOKEN "\t "

unsigned long mount_get_flags(const char *tokens)
{
    unsigned long flags = 0;

    std::istringstream s_tokens(tokens);
    std::string token;
    while (std::getline(s_tokens, token, ','))
    {
        if (token == "defaults")
            continue;
        else if (token == "remount")
            flags |= MS_REMOUNT;
        else if (token == "nosuid")
            flags |= MS_NOSUID;
        else if (token == "nodev")
            flags |= MS_NODEV;
        else if (token == "noexec")
            flags |= MS_NOEXEC;
        else if (token == "relatime")
            flags |= MS_RELATIME;
        else
            std::cout << "Invalid flag = " << token << std::endl;
    }

    return flags;
}

int mount_all()
{
    std::ifstream file("/etc/fstab");
    std::string line;
    while (std::getline(file, line))
    {
        if (line.starts_with("#"))
            continue;

        std::vector<const char *> tokens;
        auto token = strtok((char *)line.c_str(), MOUNT_TOKEN);
        while (token != nullptr)
        {
            tokens.emplace_back(token);
            token = strtok(nullptr, MOUNT_TOKEN);
        }

        auto special_file = tokens.at(0);
        auto dir = tokens.at(1);
        auto fstype = tokens.at(2);
        auto flags = mount_get_flags(tokens.at(3));

        auto result = mount(special_file, dir, fstype, flags, nullptr);
        if (result < 0)
        {
            std::cerr << "Failed to mount file = " << special_file << ", dir = " << dir << ", type = " << fstype
                      << ", flags = " << flags << std::endl;
            continue;
        }

        std::cout << "Mounted file = " << special_file << ", dir = " << dir << ", type = " << fstype
                  << ", flags = " << flags << std::endl;
    }

    return 0;
}

int main() { return mount_all(); }
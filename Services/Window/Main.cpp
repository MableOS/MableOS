#include <Core/EventLoop.hpp>
#include <Core/Ini.hpp>
#include <Window/Screen.hpp>
#include <memory>

int main()
{
    auto ini = new Core::Ini("/etc/Services/Window.ini");
    auto width = ini->read_integer("Screen", "Width", 1024);
    auto height = ini->read_integer("Screen", "Height", 768);
    auto screen = std::make_unique<Window::Screen>(width, height);

    auto event_loop = std::make_unique<Core::EventLoop>();
    return event_loop->run();
}
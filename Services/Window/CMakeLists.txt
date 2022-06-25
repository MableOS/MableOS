set(SOURCES
        Main.cpp
        Screen.cpp)

mableos_bin(WindowService)
target_link_libraries(WindowService LibCore)
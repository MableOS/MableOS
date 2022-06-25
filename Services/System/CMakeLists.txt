set(SOURCES
        Main.cpp
        Service.cpp)

mableos_bin(SystemService)
target_link_libraries(SystemService LibCore)
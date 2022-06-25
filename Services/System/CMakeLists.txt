set(SOURCES
        Main.cpp)

mableos_bin(SystemService)
target_link_libraries(SystemService LibCore)
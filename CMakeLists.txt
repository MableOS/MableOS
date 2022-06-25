cmake_minimum_required(VERSION 3.22)
project(MableOS CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_SYSTEM_NAME MableOS)
set(CMAKE_SYSTEM_PROCESSOR x86)

set(CMAKE_SYSROOT ${CMAKE_BINARY_DIR}/Sysroot)
set(CMAKE_STAGING_PREFIX ${CMAKE_SYSROOT})
set(CMAKE_INSTALL_PREFIX ${CMAKE_SYSROOT})
set(CMAKE_INSTALL_DATAROOTDIR ${CMAKE_SYSROOT}/res)

set(TOOLCHAIN ${CMAKE_BINARY_DIR}/Tools/Cross/bin/i686-mableos-linux-gnu)
set(CMAKE_ASM_COMPILER ${TOOLCHAIN}-gcc)
set(CMAKE_C_COMPILER ${TOOLCHAIN}-gcc)
set(CMAKE_CXX_COMPILER ${TOOLCHAIN}-g++)
set(CMAKE_LINKER ${TOOLCHAIN}-ld)
set(CMAKE_AR ${TOOLCHAIN}-ar)
set(CMAKE_OBJCOPY ${TOOLCHAIN}-objcopy)
set(CMAKE_RANLIB ${TOOLCHAIN}-ranlib)
set(CMAKE_STRIP ${TOOLCHAIN}-strip)

set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)

add_compile_options(-Wall -Wextra)

add_custom_target(toolchain
        COMMAND "SOURCE_DIR=${CMAKE_SOURCE_DIR}" "SYSROOT_DIR=${CMAKE_SYSROOT}" ${CMAKE_SOURCE_DIR}/Scripts/SetupToolchain.sh
        USES_TERMINAL)

add_custom_target(image
        COMMAND "SOURCE_DIR=${CMAKE_SOURCE_DIR}" "SYSROOT_DIR=${CMAKE_SYSROOT}" ${CMAKE_SOURCE_DIR}/Scripts/SetupImage.sh
        USES_TERMINAL)

add_custom_target(run
        COMMAND ${CMAKE_SOURCE_DIR}/Scripts/Run.sh
        USES_TERMINAL)

function(mableos_install_headers target_name)
    file(GLOB_RECURSE headers RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "*.hpp")
    foreach (header ${headers})
        get_filename_component(subdirectory ${header} DIRECTORY)
        install(FILES ${header} DESTINATION usr/include/${target_name}/${subdirectory})
    endforeach ()
endfunction()

function(mableos_install_sources)
    cmake_path(RELATIVE_PATH CMAKE_CURRENT_SOURCE_DIR BASE_DIRECTORY ${CMAKE_SOURCE_DIR} OUTPUT_VARIABLE relative_dir)
    file(GLOB_RECURSE sources RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "*.hpp" "*.cpp")
    foreach (source ${sources})
        get_filename_component(subdirectory ${source} DIRECTORY)
        install(FILES ${source} DESTINATION usr/src/MableOS/${relative_dir}/${subdirectory})
    endforeach ()
endfunction()

function(mableos_bin bin_name)
    add_executable(${bin_name} ${SOURCES})
    install(TARGETS ${bin_name} RUNTIME DESTINATION bin)
endfunction()

function(mableos_lib lib_name fs_name)
    mableos_install_headers(${lib_name})
    mableos_install_sources()
    add_library(${lib_name} SHARED ${SOURCES})
    set_target_properties(${lib_name} PROPERTIES OUTPUT_NAME ${fs_name})
    install(TARGETS ${lib_name} DESTINATION usr/lib)
endfunction()

include_directories(Libraries)

add_subdirectory(Services)
add_subdirectory(Utilities)
add_subdirectory(Libraries)
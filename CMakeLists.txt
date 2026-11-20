cmake_minimum_required(VERSION 3.16)

project(MyQtProject VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Tells CMake to run Qt Compilers
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)

# Required for .qrc files (Qt resource files for icons, images, etc)
set(CMAKE_AUTORCC ON)

# Auto-detect Qt 6 MSVC installations on Windows (and ignore invalid teammate paths)
if(WIN32 AND MSVC)
    set(_NEEDS_QT_AUTODETECT TRUE)
    if(CMAKE_PREFIX_PATH)
        foreach(_P IN LISTS CMAKE_PREFIX_PATH)
            if(EXISTS "${_P}/lib/cmake/Qt6/Qt6Config.cmake")
                set(_NEEDS_QT_AUTODETECT FALSE)
            endif()
        endforeach()
    endif()

    if(_NEEDS_QT_AUTODETECT)
        file(GLOB _QT_CANDIDATES
            "C:/Qt/6.*/msvc*_64"
            "D:/Qt/6.*/msvc*_64"
            "$ENV{USERPROFILE}/Qt/6.*/msvc*_64"
        )
        if(_QT_CANDIDATES)
            list(SORT _QT_CANDIDATES)
            list(REVERSE _QT_CANDIDATES) # Pick newest if multiple exist
            list(GET _QT_CANDIDATES 0 _QT_PATH)
            set(CMAKE_PREFIX_PATH "${_QT_PATH}")
            message(STATUS "Auto-detected Qt at: ${_QT_PATH}")
        else()
            message(WARNING "Could not auto-detect Qt 6 MSVC kit in C:/Qt, D:/Qt, or $ENV{USERPROFILE}/Qt")
        endif()
    endif()
endif()

find_package(Qt6 REQUIRED COMPONENTS Widgets)

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/*.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/*.ui"
)

add_executable(${PROJECT_NAME} ${SOURCES})

target_include_directories(${PROJECT_NAME} PRIVATE include)

target_link_libraries(${PROJECT_NAME} PRIVATE Qt6::Widgets)

# Suggestion from AI to automatically bundle Qt DLLs on Windows
if(WIN32)
    # Locate windeployqt.exe inside the detected Qt path
    find_program(WINDEPLOYQT_EXE windeployqt HINTS "${_QT_PATH}/bin" "${CMAKE_PREFIX_PATH}/bin")
    
    # Automatically copy all necessary Qt DLLs and plugins next to the .exe
    if(WINDEPLOYQT_EXE)
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
            COMMAND "${WINDEPLOYQT_EXE}" "$<TARGET_FILE:${PROJECT_NAME}>"
            COMMENT "Running windeployqt to bundle DLLs..."
        )
    endif()
endif()
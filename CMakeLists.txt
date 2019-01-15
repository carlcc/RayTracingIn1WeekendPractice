cmake_minimum_required(VERSION 3.7)
project(RayTracingIn1WeekendPractice)

set(CMAKE_CXX_STANDARD 11)


if(WIN32)
    set(COMMON_LIBS
            optimized
            SDL2
            SDL2main
            glad
            opengl32
            Imm32
            Winmm
            version
            pthread
            )
    set(OS_PREFIX win32)
elseif(APPLE)

    find_library(COCOA Cocoa)
    find_library(IOKIT IOKit)
    find_library(CORE_FOUNDATION CoreFoundation)
    find_library(CORE_VIDEO CoreVideo)
    find_library(CORE_AUDIO CoreAudio)
    find_library(FORCE_FEEDBACK ForceFeedback)
    find_library(AUDIO_TOOLBOX AudioToolbox)
    find_library(CARBON Carbon)

    set(COMMON_LIBS
            SDL2main
            SDL2
            glad
            assimp
            ${COCOA}
            ${IOKIT}
            ${CORE_FOUNDATION}
            ${CORE_VIDEO}
            ${CORE_AUDIO}
            ${FORCE_FEEDBACK}
            ${AUDIO_TOOLBOX}
            ${CARBON}
            iconv
            )
    set(OS_PREFIX mac)
elseif(UNIX)
    set(COMMON_LIBS
            SDL2main
            SDL2
            glad

            dl
            )
    set(OS_PREFIX linux)
endif()

set(COMMON_INCLUDES
        3rdparty/${OS_PREFIX}/include
        lib
        )

link_directories(
        3rdparty/${OS_PREFIX}/lib
)

include_directories(
        ${COMMON_INCLUDES}
        app
)

add_library(app STATIC
        app/BaseApplication.cpp
        app/BaseApplication.h
        app/Log.h
        app/Log.cpp
        app/Paths.h
        app/Image.cpp
        app/Image.h
        app/Camera.cpp
        app/Camera.h
        app/InputManager.cpp
        app/InputManager.h
        app/TimeManager.cpp
        app/TimeManager.h
        app/FrameRateCounter.cpp
        app/FrameRateCounter.h
        app/gl/ShaderProgram.cpp
        app/gl/ShaderProgram.h
        app/gl/WorldGridRenderer.cpp
        app/gl/WorldGridRenderer.h
        app/gl/Texture2D.cpp
        app/gl/Texture2D.h
        app/Vec3f.cpp
        app/Vec3f.h
        app/stb_image_write.h
        app/Ray.h
        app/Hitable.h
        app/Sphere.cpp
        app/Sphere.h
        app/HitableList.cpp
        app/HitableList.h
        app/RTCamera.h
        app/RTMaterial.h
        )


set(PROJECTS
        "1.test"
        "1.Program1"
        "2.Program2"
        "3.Antialiasing"
        "4.DiffuseMaterials"
        "5.MetalMaterials"
        "6.DielectricMaterials"
        )


foreach (proj ${PROJECTS})
    file(GLOB_RECURSE proj_src src/${proj}/*)
    add_executable(${proj} ${proj_src} )
    set_property(TARGET ${proj} PROPERTY DEBUG_POSTFIX _d)
    add_dependencies(${proj} app)
    target_link_libraries(${proj} app ${COMMON_LIBS})
endforeach (proj)

set(TESTS
        "TestImage"
        )

foreach (proj ${TESTS})
    file(GLOB_RECURSE proj_src tests/${proj}/*)
    add_executable(${proj} ${proj_src} )
    set_property(TARGET ${proj} PROPERTY DEBUG_POSTFIX _d)
    add_dependencies(${proj} app)
    target_link_libraries(${proj} app ${COMMON_LIBS})
endforeach (proj)

#add_executable(PlayVulkan src/main.cpp)
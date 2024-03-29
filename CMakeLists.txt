# CMakeList.txt : CMake project for vulkan_guide, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.10)

project ("enix")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Vulkan REQUIRED)

add_subdirectory(ThirdParty)

set (CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/bin")

add_subdirectory(Enix)
find_program(GLSL_VALIDATOR glslangValidator HINTS /usr/bin /usr/local/bin $ENV{VULKAN_SDK}/Bin/ $ENV{VULKAN_SDK}/Bin32/)
find_program(GLSLC glslc HINTS /usr/bin /usr/local/bin $ENV{VULKAN_SDK}/Bin/ $ENV{VULKAN_SDK}/Bin32/)
message(STATUS ${GLSL_VALIDATOR})
message(STATUS ${GLSLC})

file(GLOB_RECURSE GLSL_SOURCE_FILES
    "${PROJECT_SOURCE_DIR}/Shaders/*.frag"
    "${PROJECT_SOURCE_DIR}/Shaders/*.vert"
    "${PROJECT_SOURCE_DIR}/Shaders/*.comp"
    )

foreach(GLSL ${GLSL_SOURCE_FILES})
  message(STATUS "BUILDING SHADER")
  get_filename_component(FILE_NAME ${GLSL} NAME)
  set(SPIRV "${PROJECT_SOURCE_DIR}/Shaders/${FILE_NAME}.spv")
  message(STATUS ${GLSL})
  #message(STATUS ${SPIRV})
  add_custom_command(
    OUTPUT ${SPIRV}
#    COMMAND ${GLSL_VALIDATOR} -H ${GLSL} -o ${SPIRV}
    COMMAND ${GLSLC} ${GLSL} -o ${SPIRV}
    DEPENDS ${GLSL})
  list(APPEND SPIRV_BINARY_FILES ${SPIRV})
endforeach(GLSL)

add_custom_target(
    Shaders 
    DEPENDS ${SPIRV_BINARY_FILES}
    )
    
target_sources(Shaders
    PRIVATE
    ${GLSL_SOURCE_FILES}
    )

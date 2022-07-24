find_package(Vulkan REQUIRED)
find_package(OpenGL REQUIRED)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)
add_subdirectory(glfw/glfw)

add_subdirectory(glm)

add_library(imgui STATIC)

target_include_directories(imgui PUBLIC imgui/imgui imgui/imgui/backends )

target_sources(imgui PRIVATE 
  imgui/imgui/imgui.h
  imgui/imgui/imgui.cpp

  imgui/imgui/imgui_demo.cpp
  imgui/imgui/imgui_draw.cpp
  imgui/imgui/imgui_tables.cpp
  imgui/imgui/imgui_widgets.cpp

  imgui/imgui/backends/imgui_impl_vulkan.h
  imgui/imgui/backends/imgui_impl_vulkan.cpp
  imgui/imgui/backends/imgui_impl_glfw.h
  imgui/imgui/backends/imgui_impl_glfw.cpp
  imgui/imgui/backends/imgui_impl_opengl3.cpp
  imgui/imgui/backends/imgui_impl_opengl3.h
  )

target_link_libraries(imgui PUBLIC Vulkan::Vulkan ${OPENGL_LIBRARY} glfw)

add_library(stb_image INTERFACE)
target_include_directories(stb_image INTERFACE stb_image)


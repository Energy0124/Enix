
# Add source to this project's executable.
add_executable (Enix
    Main.cpp
    Engine.h
    OpenGLEngine.h
    OpenGLEngine.cpp
    VulkanEngine.h
    VulkanEngine.cpp
    VulkanEngineImgui.h
    VulkanEngineImgui.cpp
)


set_property(TARGET Enix PROPERTY VS_DEBUGGER_WORKING_DIRECTORY $<TARGET_FILE_DIR:Enix>)

target_include_directories(Enix PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")

target_link_libraries(Enix glm Vulkan::Vulkan ${OPENGL_LIBRARY} glfw imgui stb_image tinyobjloader)

# make sure that the shaders are compiled before running Enix
add_dependencies(Enix Shaders)

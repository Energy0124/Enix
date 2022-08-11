
# Add source to this project's executable.
add_executable(Enix
        Main.cpp
        Engine.h
        OpenGLEngine.h
        OpenGLEngine.cpp
        VulkanEngine.h
        VulkanEngine.cpp
        VulkanEngineImgui.h
        VulkanEngineImgui.cpp
        Render/Vertex.h
        Render/Window.h
        Render/Window.cpp
        Render/Device.h
        Render/Device.cpp
        Render/Model.h
        Render/Model.cpp
        Asset/MeshAsset.cpp Asset/MeshAsset.h
        Render/Instance.cpp Render/Instance.h
        Render/WindowSurface.cpp Render/WindowSurface.h
        Render/SwapChain.cpp Render/SwapChain.h Render/RenderPass.cpp Render/RenderPass.h Render/Image.cpp Render/Image.h Render/VulkanUtility.h Render/GraphicsPipeline.cpp Render/GraphicsPipeline.h Render/Texture.cpp Render/Texture.h Asset/TextureAsset.cpp Asset/TextureAsset.h)


set_property(TARGET Enix PROPERTY VS_DEBUGGER_WORKING_DIRECTORY $<TARGET_FILE_DIR:Enix>)

target_include_directories(Enix PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")

target_link_libraries(Enix glm Vulkan::Vulkan ${OPENGL_LIBRARY} glfw imgui stb_image tinyobjloader spdlog)

# make sure that the shaders are compiled before running Enix
add_dependencies(Enix Shaders)

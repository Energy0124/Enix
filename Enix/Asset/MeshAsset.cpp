//
// Created by Energy on 2022/08/05.
//

#include "MeshAsset.h"
#include "tiny_obj_loader.h"
#include "stb_image.h"

#include <utility>

namespace Enix {
    MeshAsset::MeshAsset(std::string modelPath, std::string texturePath, const Device &device)
            : _modelPath(std::move(modelPath)),
              _texturePath(std::move(texturePath)) {
        loadModel(device);
        loadTexture();
    }

    MeshAsset::~MeshAsset() = default;

    void MeshAsset::loadModel(const Device &device) {
        tinyobj::attrib_t attrib;
        std::vector<tinyobj::shape_t> shapes;
        std::vector<tinyobj::material_t> materials;
        std::string warn, err;

        if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, (_modelPath).c_str())) {
            throw std::runtime_error(warn + err);
        }

        std::unordered_map<Vertex, uint32_t> uniqueVertices{};
        std::vector<Vertex> vertices{};
        std::vector<uint32_t> indices{};

        for (const auto &shape: shapes) {
            for (const auto &index: shape.mesh.indices) {
                Vertex vertex{};
                vertex.pos = {
                        attrib.vertices[3 * index.vertex_index + 0],
                        attrib.vertices[3 * index.vertex_index + 1],
                        attrib.vertices[3 * index.vertex_index + 2]
                };

                vertex.texCoord = {
                        attrib.texcoords[2 * index.texcoord_index + 0],
                        1.0f - attrib.texcoords[2 * index.texcoord_index + 1]
                };

                vertex.color = {1.0f, 1.0f, 1.0f};
                vertex.texCoord = {
                        attrib.texcoords[2 * index.texcoord_index + 0],
                        1.0f - attrib.texcoords[2 * index.texcoord_index + 1]
                };

                if (!uniqueVertices.contains(vertex)) {
                    uniqueVertices[vertex] = static_cast<uint32_t>(vertices.size());
                    vertices.push_back(vertex);
                }
                indices.push_back(uniqueVertices[vertex]);
            }
        }
        _model = std::make_unique<Model>(device, std::move(vertices), std::move(indices));
    }

    void MeshAsset::loadTexture() {
        int texWidth, texHeight, texChannels;
        std::string filename = _texturePath;
        stbi_uc *pixels = stbi_load(filename.data(), &texWidth, &texHeight, &texChannels, STBI_rgb_alpha);
        VkDeviceSize imageSize = texWidth * texHeight * 4;

        if (!pixels) {
            throw std::runtime_error("failed to load texture image!");
        }
    }
} // Enix
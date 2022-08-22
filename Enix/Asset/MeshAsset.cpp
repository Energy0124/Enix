//
// Created by Energy on 2022/08/05.
//

#include "MeshAsset.h"
#include "tiny_obj_loader.h"
#include "stb_image.h"
#include "TextureAsset.h"
#include "spdlog/spdlog.h"

#include <utility>

namespace Enix {
    MeshAsset::MeshAsset(std::string modelPath, std::string texturePath, Device &device)
            : _modelPath(std::move(modelPath)),
              _texturePath(std::move(texturePath)) //, _textureAsset(_texturePath)
    {
        loadModel(device);
//        loadTexture(device);
    }

    MeshAsset::~MeshAsset() = default;

    void MeshAsset::loadModel(Device &device) {
        spdlog::info("Loading model: {}", _modelPath);

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

                vertex.color = {
                        attrib.colors[3 * index.vertex_index + 0],
                        attrib.colors[3 * index.vertex_index + 1],
                        attrib.colors[3 * index.vertex_index + 2],
                };
                if (index.normal_index >= 0) {
                    vertex.normal = {
                            attrib.normals[3 * index.normal_index + 0],
                            attrib.normals[3 * index.normal_index + 1],
                            attrib.normals[3 * index.normal_index + 2],
                    };
                }
                vertex.texCoord = {
                        attrib.texcoords[2 * index.texcoord_index + 0],
                        1.0f - attrib.texcoords[2 * index.texcoord_index + 1]
                };

//                vertex.color = {1.0f, 1.0f, 1.0f};

                if (!uniqueVertices.contains(vertex)) {
                    uniqueVertices[vertex] = static_cast<uint32_t>(vertices.size());
                    vertices.push_back(vertex);
                }
                indices.push_back(uniqueVertices[vertex]);
            }
        }
        _model = std::make_unique<Model>(device, std::move(vertices), std::move(indices));
    }

//    void MeshAsset::loadTexture(Device &device) {
//
//        _texture = std::make_unique<Texture>(device, _textureAsset);
//    }
} // Enix
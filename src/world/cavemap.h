#pragma once

#include <cstddef>
#include <cstdint>
#include <filesystem>
#include <string>
#include <vector>

#include <fmt/base.h>
#include <glm/ext/vector_float3.hpp>

#include "core/io/binary_reader.h"
#include "core/io/file.h"
#include "glm/ext/vector_double3.hpp"
#include "utils/logger.h"
#include "utils/timer.h"

namespace fs = std::filesystem;

struct CaveMap : public Loggable<CaveMap>
{
    std::vector<glm::vec3> voxels;

    glm::dvec3 center;

    CaveMap()
    {
        Timer timer;

        std::string filename = "ignore/cavemap/region_0.bin";

        for (const auto &entry : fs::directory_iterator("ignore/cavemap"))
        {
            if (entry.is_regular_file() && entry.path().extension() == ".bin")
            {
                readCaveRegion(entry.path().string());
            }
        }

        center /= voxels.size();

        logger.info("{} voxels parsed in {}ms", voxels.size(), timer.elapsedMiliseconds());
    }

    void readCaveRegion(const std::string &filename)
    {
        std::vector<uint8_t> bytes = File::readAllBytes(filename);

        size_t offset = 0;
        size_t voxelsCount = 0;

        while (offset < bytes.size())
        {
            int x = BinaryReader::readInt32LE(bytes, offset);
            int z = BinaryReader::readInt32LE(bytes, offset);
            int layersCount = BinaryReader::readInt32LE(bytes, offset);

            for (int i = 0; i < layersCount; i++)
            {
                int layerData = BinaryReader::readInt32LE(bytes, offset);

                uint8_t start = (layerData >> 16) & 255;
                uint8_t end = (layerData >> 8) & 255;

                for (int y = start; y <= end; y++)
                {
                    voxels.emplace_back(x, y, z);

                    center.x += x;
                    center.y += y;
                    center.z += z;

                    voxelsCount++;
                }
            }
        }
    }
};
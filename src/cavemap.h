#pragma once

#include <raylib.h>
#include <cstddef>
#include <cstdint>
#include <string>
#include <vector>

#include "fmt/base.h"
#include "io/binary_reader.h"
#include "io/file.h"
#include "timer.h"

struct CaveMap
{
    std::vector<Vector3> voxels;

    CaveMap()
    {
        Timer timer;

        std::string filename = "ignore/cavemap/region_0.bin";
        std::vector<uint8_t> bytes = File::readAllBytes(filename);

        size_t offset = 0;

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
                }
            }
        }

        fmt::println("{} voxels parsed in {}ms", voxels.size(), timer.elapsedMiliseconds());
    }
};
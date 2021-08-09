//  -------------------------------------------------------------------------
//  Copyright (C) 2020 BMW AG
//  -------------------------------------------------------------------------
//  This Source Code Form is subject to the terms of the Mozilla Public
//  License, v. 2.0. If a copy of the MPL was not distributed with this
//  file, You can obtain one at https://mozilla.org/MPL/2.0/.
//  -------------------------------------------------------------------------

#include "FileUtils.h"
#include "StdFilesystemWrapper.h"
#include <fstream>

namespace rlogic::internal
{
    bool FileUtils::SaveBinary(const std::string& filename, const void* binaryBuffer, size_t bufferLength)
    {
        std::ofstream fileStream(filename, std::ofstream::binary);
        if (!fileStream.is_open())
        {
            return false;
        }
        fileStream.write(static_cast<const char*>(binaryBuffer), static_cast<std::streamsize>(bufferLength));
        return !fileStream.bad();
    }

    std::optional<std::vector<char>> FileUtils::LoadBinary(const std::string& filename)
    {
        // ifstream does not prevent opening directories (and crashes in some cases), have to use filesystem to check
        if(fs::is_directory(filename))
        {
            return std::nullopt;
        }

        std::ifstream fileStream(filename, std::ifstream::binary);
        if (!fileStream.is_open())
        {
            return std::nullopt;
        }
        fileStream.seekg(0, std::ios::end);
        std::vector<char> byteBuffer(static_cast<size_t>(fileStream.tellg()));
        fileStream.seekg(0, std::ios::beg);
        fileStream.read(byteBuffer.data(), static_cast<std::streamsize>(byteBuffer.size()));
        if (fileStream.bad())
        {
            return std::nullopt;
        }

        return byteBuffer;
    }
}

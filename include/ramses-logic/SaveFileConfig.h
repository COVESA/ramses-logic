//  -------------------------------------------------------------------------
//  Copyright (C) 2022 BMW AG
//  -------------------------------------------------------------------------
//  This Source Code Form is subject to the terms of the Mozilla Public
//  License, v. 2.0. If a copy of the MPL was not distributed with this
//  file, You can obtain one at https://mozilla.org/MPL/2.0/.
//  -------------------------------------------------------------------------

#pragma once

#include "ramses-logic/APIExport.h"
#include "ramses-logic/ELuaSavingMode.h"

#include <string>
#include <memory>

namespace rlogic::internal
{
    class SaveFileConfigImpl;
}

namespace rlogic
{
    /**
     * Holds configuration settings for saving #rlogic::LogicEngine instances into a file.
     * This config file is designed to work with the Ramses Composer editor, but you can use the metadata to store
     * version and export information from any exporter (or script). Use the config to trace the origin and
     * export environment of assets during runtime.
     */
    class SaveFileConfig
    {
    public:
        RLOGIC_API SaveFileConfig() noexcept;

        /**
         * Adds custom string metadata to the binary file saved by #rlogic::LogicEngine. Can be any string. It is not
         * used by anything other than logging info, particularly when loading a file and when errors occur.
         *
         * @param metadata the string to be written together with the saved binary data
         */
        RLOGIC_API void setMetadataString(std::string_view metadata);

        /**
         * Specifies the semantic version of the exporter (binary, or script, or runtime logic). Use this to match a
         * given asset later (e.g. when used in runtime) to the originating export tool to verify compatibility or
         * diagnose errors and issues easier.
         *
         * @param major the major version number
         * @param minor the minor version number
         * @param patch the patch version number
         * @param fileFormatVersion the file format used by the exporter itself (use 0 if not applicable)
         */
        RLOGIC_API void setExporterVersion(uint32_t major, uint32_t minor, uint32_t patch, uint32_t fileFormatVersion);

        /**
        * By default, saving to file validates the content and issues warnings (see #rlogic::LogicEngine::validate).
        * This behavior can be disabled here. Calling this with validationEnabled=false will itself cause a INFO log,
        * but will silence further warnings in the content.
        *
        * @param validationEnabled flag to disable/enable validation upon saving to file
        */
        RLOGIC_API void setValidationEnabled(bool validationEnabled);

        /**
        * Sets saving mode for all #rlogic::LuaScript and/or #rlogic::LuaModule instances.
        * See #rlogic::ELuaSavingMode for the available options and their implications.
        * Note that this is just a hint and the export logic will decide what to actually export,
        * depending on availabilty of Lua source code or bytecode:
        *  - if only source code is available (always the case when using #rlogic::EFeatureLevel_01)
        *    then only source code is exported regardless of the selected saving \c mode
        *  - if only byte code is available then only bytecode is exported regardless of the selected saving \c mode
        *  - if both source and bytecode are available then the selected saving \c mode is applied
        * There will be no error produced if selected saving mode cannot be respected.
        *
        * @param mode selected saving mode, default is #rlogic::ELuaSavingMode::SourceAndByteCode
        */
        RLOGIC_API void setLuaSavingMode(ELuaSavingMode mode);

        /**
         * Destructor of #SaveFileConfig
         */
        RLOGIC_API ~SaveFileConfig() noexcept;

        /**
         * Copy Constructor of #SaveFileConfig
         * @param other the other #SaveFileConfig to copy from
         */
        RLOGIC_API SaveFileConfig(const SaveFileConfig& other);

        /**
         * Move Constructor of #SaveFileConfig
         * @param other the other #SaveFileConfig to move from
         */
        RLOGIC_API SaveFileConfig(SaveFileConfig&& other) noexcept;

        /**
         * Assignment operator of #SaveFileConfig
         * @param other the other #SaveFileConfig to copy from
         * @return self
         */
        RLOGIC_API SaveFileConfig& operator=(const SaveFileConfig& other);

        /**
         * Move assignment operator of #SaveFileConfig
         * @param other the other #SaveFileConfig to move from
         * @return self
         */
        RLOGIC_API SaveFileConfig& operator=(SaveFileConfig&& other) noexcept;

        /**
         * Implementation detail of #SaveFileConfig
         */
        std::unique_ptr<internal::SaveFileConfigImpl> m_impl;
    };
}

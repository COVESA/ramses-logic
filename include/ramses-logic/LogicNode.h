//  -------------------------------------------------------------------------
//  Copyright (C) 2020 BMW AG
//  -------------------------------------------------------------------------
//  This Source Code Form is subject to the terms of the Mozilla Public
//  License, v. 2.0. If a copy of the MPL was not distributed with this
//  file, You can obtain one at https://mozilla.org/MPL/2.0/.
//  -------------------------------------------------------------------------

#pragma once

#include "ramses-logic/APIExport.h"

#include <vector>
#include <string>
#include <functional>

namespace rlogic::internal
{
    class LogicNodeImpl;
}

namespace rlogic
{
    class Property;

    /**
     * A base class for multiple logic classes which provides a unified interface to their
     * inputs and outputs. Some subclasses don't have inputs or outputs - in that case the
     * #getInputs or #getOutputs methods respectively will return nullptr. Some subclasses,
     * like the #rlogic::RamsesAppearanceBinding, will have their inputs depending on their
     * current state (in this example the GLSL uniforms of the shader to which the bound ramses
     * Appearance belongs). In those cases, #getInputs()/#getOutputs() will return a #rlogic::Property
     * which represents an empty struct (type Struct, but no child properties).
     */
    class LogicNode
    {
    public:

        /**
         * Returns a property of type Struct which holds the inputs of the #LogicNode.
         *
         * Returns the root Property of the LogicNode which contains potentially
         * nested list of properties. The properties are different for the classes which derive from
         * #LogicNode. Look at the documentation of each derived class for more information on the properties.             *
         * @return a tree like structure with the inputs of the #LogicNode
         */
        [[nodiscard]] RLOGIC_API Property* getInputs();

        /**
         * @copydoc getInputs()
         */
        [[nodiscard]] RLOGIC_API const Property* getInputs() const;

        /**
         * Returns a property of type Struct which holds the outputs of the #LogicNode
         *
         * Returns the root Property of the LogicNode which contains potentially
         * nested list of properties. The properties are different for the classes which derive from
         * #LogicNode. Look at the documentation of each derived class for more information on the properties.             *
         * @return a tree like structure with the outputs of the LogicNode
         */
        [[nodiscard]] RLOGIC_API const Property* getOutputs() const;

        /**
         * Returns the name of this #LogicNode.
         *
         * @return the name of the LogicNode
         */
        [[nodiscard]] RLOGIC_API std::string_view getName() const;

        /**
        * Sets the name of this #LogicNode.
        *
        * @param name new name of the #LogicNode
        */
        RLOGIC_API void setName(std::string_view name);

        /**
        * Destructor of #LogicNode
        */
        virtual ~LogicNode() noexcept;

        /**
        * Copy Constructor of LogicNode is deleted because LogicNodes are not supposed to be copied
        *
        * @param other LogicNode to copy from
        */
        LogicNode(const LogicNode& other) = delete;

        /**
        * Move Constructor of LogicNode is deleted because LogicNodes are not supposed to be moved
        *
        * @param other LogicNode to move from
        */
        LogicNode(LogicNode&& other) = delete;

        /**
        * Assignment operator of LogicNode is deleted because LogicNodes are not supposed to be copied
        *
        * @param other LogicNode to assign from
        */
        LogicNode& operator=(const LogicNode& other) = delete;

        /**
        * Move assignment operator of LogicNode is deleted because LogicNodes are not supposed to be moved
        *
        * @param other LogicNode to move from
        */
        LogicNode& operator=(LogicNode&& other) = delete;

        /**
         * Implementation detail of LuaScript
         */
        std::reference_wrapper<internal::LogicNodeImpl> m_impl;
    protected:

        /**
         * Constructor of LogicNode. User is not supposed to call this - LogcNodes are created by subclasses
         *
         * @param impl implementation details of the LogicNode
         */
        explicit LogicNode(std::reference_wrapper<internal::LogicNodeImpl> impl) noexcept;

    };
}

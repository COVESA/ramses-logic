//  -------------------------------------------------------------------------
//  Copyright (C) 2022 BMW AG
//  -------------------------------------------------------------------------
//  This Source Code Form is subject to the terms of the Mozilla Public
//  License, v. 2.0. If a copy of the MPL was not distributed with this
//  file, You can obtain one at https://mozilla.org/MPL/2.0/.
//  -------------------------------------------------------------------------

#pragma once

#include "ramses-logic/APIExport.h"
#include "ramses-logic/RamsesBinding.h"

namespace ramses
{
    class MeshNode;
}

namespace rlogic::internal
{
    class RamsesMeshNodeBindingImpl;
}

namespace rlogic
{
    /**
     * The #RamsesMeshNodeBinding binds to a Ramses object instance of ramses::MeshNode and exposes a set of input properties
     * allowing to control certain states the MeshNode.
     *
     * #RamsesMeshNodeBinding has these input properties:
     *   'vertexOffset'  (type int32)
     *       - binds to MeshNode's vertex offset, i.e. start offset into vertex arrays of the mesh (ramses::MeshNode::setStartVertex).
     *   'indexOffset'   (type int32)
     *       - binds to MeshNode's index offset, i.e. start offset into indices of the mesh (ramses::MeshNode::setStartIndex).
     *   'indexCount'    (type int32)
     *       - binds to MeshNode's index count, i.e. number of indices to use for rendering of the mesh (ramses::MeshNode::setIndexCount).
     *   'instanceCount' (type int32)
     *       - binds to MeshNode's instance count, i.e. number of instances to render when using geometry instancing (ramses::MeshNode::setInstanceCount).
     *
     * The initial values of the input properties are taken from the bound ramses::MeshNode when the #RamsesMeshNodeBinding is created.
     *
     * The #RamsesMeshNodeBinding class has no output properties (thus #rlogic::LogicNode::getOutputs() will return nullptr) because
     * the outputs are implicitly forwarded to the bound ramses::MeshNode.
     *
     * The changes via binding objects are applied to the bound object right away when calling rlogic::LogicEngine::update(),
     * however keep in mind that Ramses has a mechanism for bundling scene changes and applying them at once using ramses::Scene::flush,
     * so the changes will be applied all the way only after calling this method on the scene.
     */
    class RamsesMeshNodeBinding : public RamsesBinding
    {
    public:
        /**
        * Constructor of RamsesMeshNodeBinding. User is not supposed to call this - RamsesMeshNodeBindings are created by other factory classes
        *
        * @param impl implementation details of the RamsesMeshNodeBinding
        */
        explicit RamsesMeshNodeBinding(std::unique_ptr<internal::RamsesMeshNodeBindingImpl> impl) noexcept;

        /// Destructor of RamsesMeshNodeBinding.
        ~RamsesMeshNodeBinding() noexcept override;

        /// Copy Constructor of RamsesMeshNodeBinding is deleted because RamsesMeshNodeBindings are not supposed to be copied
        RamsesMeshNodeBinding(const RamsesMeshNodeBinding&) = delete;

        /// Move Constructor of RamsesMeshNodeBinding is deleted because RamsesMeshNodeBindings are not supposed to be moved
        RamsesMeshNodeBinding(RamsesMeshNodeBinding&&) = delete;

        /// Assignment operator of RamsesMeshNodeBinding is deleted because RamsesMeshNodeBindings are not supposed to be copied
        RamsesMeshNodeBinding& operator=(const RamsesMeshNodeBinding&) = delete;

        /// Move assignment operator of RamsesMeshNodeBinding is deleted because RamsesMeshNodeBindings are not supposed to be moved
        RamsesMeshNodeBinding& operator=(RamsesMeshNodeBinding&&) = delete;

        /**
        * Returns the bound Ramses MeshNode.
        *
        * @return the bound Ramses MeshNode
        */
        [[nodiscard]] RLOGIC_API const ramses::MeshNode& getRamsesMeshNode() const;

        /**
        * Returns the bound Ramses MeshNode.
        *
        * @return the bound Ramses MeshNode
        */
        [[nodiscard]] RLOGIC_API ramses::MeshNode& getRamsesMeshNode();

        /// Implementation detail of RamsesMeshNodeBinding
        internal::RamsesMeshNodeBindingImpl& m_meshNodeBinding;
    };
}

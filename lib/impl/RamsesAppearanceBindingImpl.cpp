//  -------------------------------------------------------------------------
//  Copyright (C) 2020 BMW AG
//  -------------------------------------------------------------------------
//  This Source Code Form is subject to the terms of the Mozilla Public
//  License, v. 2.0. If a copy of the MPL was not distributed with this
//  file, You can obtain one at https://mozilla.org/MPL/2.0/.
//  -------------------------------------------------------------------------

#include "impl/RamsesAppearanceBindingImpl.h"

#include "ramses-client-api/Appearance.h"
#include "ramses-client-api/Effect.h"
#include "ramses-client-api/UniformInput.h"

#include "ramses-logic/EPropertyType.h"
#include "ramses-logic/Property.h"

#include "impl/PropertyImpl.h"
#include "impl/LoggerImpl.h"

#include "internals/RamsesHelper.h"
#include "internals/ErrorReporting.h"
#include "internals/TypeUtils.h"
#include "internals/RamsesObjectResolver.h"

#include "generated/RamsesAppearanceBindingGen.h"

namespace rlogic::internal
{
    RamsesAppearanceBindingImpl::RamsesAppearanceBindingImpl(ramses::Appearance& ramsesAppearance, std::string_view name, uint64_t id)
        : RamsesBindingImpl(name, id)
        , m_ramsesAppearance(ramsesAppearance)
    {
        const auto& effect = m_ramsesAppearance.get().getEffect();
        const uint32_t uniformCount = effect.getUniformInputCount();
        m_uniformIndices.reserve(uniformCount);

        // create mapping from property children indices to uniform inputs, this must match properties (either created or deserialized)
        for (uint32_t i = 0; i < uniformCount; ++i)
        {
            ramses::UniformInput uniformInput;
            ramses::status_t result = effect.getUniformInput(i, uniformInput);
            assert(result == ramses::StatusOK);
            assert(uniformInput.isValid());
            (void)result;

            if (GetPropertyTypeForUniform(uniformInput))
                m_uniformIndices.push_back(i);
        }
    }

    void RamsesAppearanceBindingImpl::createRootProperties()
    {
        const auto& effect = m_ramsesAppearance.get().getEffect();
        const uint32_t uniformCount = effect.getUniformInputCount();

        std::vector<HierarchicalTypeData> bindingInputs;
        bindingInputs.reserve(uniformCount);

        for (uint32_t i = 0; i < uniformCount; ++i)
        {
            ramses::UniformInput uniformInput;
            effect.getUniformInput(i, uniformInput);
            const std::optional<EPropertyType> convertedType = GetPropertyTypeForUniform(uniformInput);

            // TODO Violin handle all types eventually (need some more breaking ramses features for that)
            if (convertedType)
            {
                // Non-array case
                if (uniformInput.getElementCount() == 1)
                {
                    bindingInputs.emplace_back(MakeType(uniformInput.getName(), *convertedType));
                }
                // Array case
                else
                {
                    bindingInputs.emplace_back(MakeArray(uniformInput.getName(), uniformInput.getElementCount(), *convertedType));
                }
            }
        }

        HierarchicalTypeData bindingInputsType(TypeData{ "", EPropertyType::Struct }, bindingInputs);

        setRootInputs(std::make_unique<Property>(std::make_unique<PropertyImpl>(bindingInputsType, EPropertySemantics::BindingInput)));
    }

    flatbuffers::Offset<rlogic_serialization::RamsesAppearanceBinding> RamsesAppearanceBindingImpl::Serialize(
        const RamsesAppearanceBindingImpl& binding,
        flatbuffers::FlatBufferBuilder& builder,
        SerializationMap& serializationMap,
        EFeatureLevel /*featureLevel*/)
    {
        auto ramsesReference = RamsesBindingImpl::SerializeRamsesReference(binding.m_ramsesAppearance, builder);

        const auto logicObject = LogicObjectImpl::Serialize(binding, builder);
        const auto propertyObject = PropertyImpl::Serialize(*binding.getInputs()->m_impl, builder, serializationMap);
        auto ramsesBinding = rlogic_serialization::CreateRamsesBinding(builder,
            logicObject,
            ramsesReference,
            propertyObject);
        builder.Finish(ramsesBinding);

        rlogic_serialization::ResourceId parentEffectResourceId;
        parentEffectResourceId = rlogic_serialization::ResourceId(binding.m_ramsesAppearance.get().getEffect().getResourceId().lowPart, binding.m_ramsesAppearance.get().getEffect().getResourceId().highPart);

        auto ramsesAppearanceBinding = rlogic_serialization::CreateRamsesAppearanceBinding(builder,
            ramsesBinding,
            &parentEffectResourceId
            );
        builder.Finish(ramsesAppearanceBinding);

        return ramsesAppearanceBinding;
    }

    std::unique_ptr<RamsesAppearanceBindingImpl> RamsesAppearanceBindingImpl::Deserialize(
        const rlogic_serialization::RamsesAppearanceBinding& appearanceBinding,
        const IRamsesObjectResolver& ramsesResolver,
        ErrorReporting& errorReporting,
        DeserializationMap& deserializationMap)
    {
        if (!appearanceBinding.base())
        {
            errorReporting.add("Fatal error during loading of RamsesAppearanceBinding from serialized data: missing base class info!", nullptr, EErrorType::BinaryVersionMismatch);
            return nullptr;
        }

        std::string name;
        uint64_t id = 0u;
        uint64_t userIdHigh = 0u;
        uint64_t userIdLow = 0u;
        if (!LogicObjectImpl::Deserialize(appearanceBinding.base()->base(), name, id, userIdHigh, userIdLow, errorReporting))
        {
            errorReporting.add("Fatal error during loading of RamsesAppearanceBinding from serialized data: missing name and/or ID!", nullptr, EErrorType::BinaryVersionMismatch);
            return nullptr;
        }

        if (!appearanceBinding.base()->rootInput())
        {
            errorReporting.add("Fatal error during loading of RamsesAppearanceBinding from serialized data: missing root input!", nullptr, EErrorType::BinaryVersionMismatch);
            return nullptr;
        }

        std::unique_ptr<PropertyImpl> deserializedRootInput = PropertyImpl::Deserialize(*appearanceBinding.base()->rootInput(), EPropertySemantics::BindingInput, errorReporting, deserializationMap);

        if (!deserializedRootInput)
        {
            return nullptr;
        }

        if (deserializedRootInput->getType() != EPropertyType::Struct)
        {
            errorReporting.add("Fatal error during loading of RamsesAppearanceBinding from serialized data: root input has unexpected type!", nullptr, EErrorType::BinaryVersionMismatch);
            return nullptr;
        }

        const auto* boundObject = appearanceBinding.base()->boundRamsesObject();
        if (!boundObject)
        {
            errorReporting.add("Fatal error during loading of RamsesAppearanceBinding from serialized data: no reference to appearance!", nullptr, EErrorType::BinaryVersionMismatch);
            return nullptr;
        }

        const ramses::sceneObjectId_t objectId(boundObject->objectId());
        ramses::Appearance* resolvedAppearance = ramsesResolver.findRamsesAppearanceInScene(name, objectId);
        if (!resolvedAppearance)
        {
            // TODO Violin improve error reporting for this particular error (it's reported in ramsesResolver currently): provide better message and scene/app ids
            return nullptr;
        }

        const ramses::Effect& effect = resolvedAppearance->getEffect();
        const ramses::resourceId_t effectResourceId = effect.getResourceId();
        if (effectResourceId.lowPart != appearanceBinding.parentEffectId()->resourceIdLow() || effectResourceId.highPart != appearanceBinding.parentEffectId()->resourceIdHigh())
        {
            errorReporting.add("Fatal error during loading of RamsesAppearanceBinding from serialized data: effect signature doesn't match after loading!", nullptr, EErrorType::BinaryVersionMismatch);
            return nullptr;
        }

        auto binding = std::make_unique<RamsesAppearanceBindingImpl>(*resolvedAppearance, name, id);
        binding->setUserId(userIdHigh, userIdLow);
        binding->setRootInputs(std::make_unique<Property>(std::move(deserializedRootInput)));

        return binding;
    }

    std::optional<LogicNodeRuntimeError> RamsesAppearanceBindingImpl::update()
    {
        const size_t childCount = getInputs()->getChildCount();
        for (size_t i = 0; i < childCount; ++i)
        {
            setInputValueToUniform(i);
        }

        return std::nullopt;
    }

    void RamsesAppearanceBindingImpl::setInputValueToUniform(size_t inputIndex)
    {
        PropertyImpl& inputProperty = *getInputs()->getChild(inputIndex)->m_impl;
        const EPropertyType propertyType = inputProperty.getType();

        if (TypeUtils::IsPrimitiveType(propertyType))
        {
            if (inputProperty.checkForBindingInputNewValueAndReset())
            {
                ramses::UniformInput uniform;
                m_ramsesAppearance.get().getEffect().getUniformInput(m_uniformIndices[inputIndex], uniform);

                switch (propertyType)
                {
                case EPropertyType::Float:
                    m_ramsesAppearance.get().setInputValueFloat(uniform, inputProperty.getValueAs<float>());
                    break;
                case EPropertyType::Int32:
                    m_ramsesAppearance.get().setInputValueInt32(uniform, inputProperty.getValueAs<int32_t>());
                    break;
                case EPropertyType::Vec2f:
                {
                    const auto vec = inputProperty.getValueAs<vec2f>();
                    m_ramsesAppearance.get().setInputValueVector2f(uniform, vec[0], vec[1]);
                    break;
                }
                case EPropertyType::Vec2i:
                {
                    const auto vec = inputProperty.getValueAs<vec2i>();
                    m_ramsesAppearance.get().setInputValueVector2i(uniform, vec[0], vec[1]);
                    break;
                }
                case EPropertyType::Vec3f:
                {
                    const auto vec = inputProperty.getValueAs<vec3f>();
                    m_ramsesAppearance.get().setInputValueVector3f(uniform, vec[0], vec[1], vec[2]);
                    break;
                }
                case EPropertyType::Vec3i:
                {
                    const auto vec = inputProperty.getValueAs<vec3i>();
                    m_ramsesAppearance.get().setInputValueVector3i(uniform, vec[0], vec[1], vec[2]);
                    break;
                }
                case EPropertyType::Vec4f:
                {
                    const auto vec = inputProperty.getValueAs<vec4f>();
                    m_ramsesAppearance.get().setInputValueVector4f(uniform, vec[0], vec[1], vec[2], vec[3]);
                    break;
                }
                case EPropertyType::Vec4i:
                {
                    const auto vec = inputProperty.getValueAs<vec4i>();
                    m_ramsesAppearance.get().setInputValueVector4i(uniform, vec[0], vec[1], vec[2], vec[3]);
                    break;
                }
                case EPropertyType::String:
                case EPropertyType::Array:
                case EPropertyType::Struct:
                case EPropertyType::Bool:
                case EPropertyType::Int64:
                    assert(false && "This should never happen");
                    break;
                }
            }
        }
        else
        {
            assert(propertyType == EPropertyType::Array);

            // A new value on any of the array element causes the whole array to be updated
            // Ramses does not allow partial updates so this is the only option here
            bool anyArrayElementWasSet = false;
            const size_t arraySize = inputProperty.getChildCount();
            for (size_t i = 0; i < arraySize; ++i)
            {
                if (inputProperty.getChild(i)->m_impl->checkForBindingInputNewValueAndReset())
                {
                    anyArrayElementWasSet = true;
                }
            }

            if (anyArrayElementWasSet)
            {
                ramses::UniformInput uniform;
                m_ramsesAppearance.get().getEffect().getUniformInput(m_uniformIndices[inputIndex], uniform);

                const EPropertyType arrayElementType = inputProperty.getChild(0)->getType();
                switch (arrayElementType)
                {
                case EPropertyType::Float:
                    m_ramsesAppearance.get().setInputValueFloat(uniform, static_cast<uint32_t>(arraySize), TypeUtils::FlattenArrayData<float, float>(inputProperty).data());
                    break;
                case EPropertyType::Int32:
                    m_ramsesAppearance.get().setInputValueInt32(uniform, static_cast<uint32_t>(arraySize), TypeUtils::FlattenArrayData<int32_t, int32_t>(inputProperty).data());
                    break;
                case EPropertyType::Vec2f:
                    m_ramsesAppearance.get().setInputValueVector2f(uniform, static_cast<uint32_t>(arraySize), TypeUtils::FlattenArrayData<float, vec2f>(inputProperty).data());
                    break;
                case EPropertyType::Vec2i:
                    m_ramsesAppearance.get().setInputValueVector2i(uniform, static_cast<uint32_t>(arraySize), TypeUtils::FlattenArrayData<int32_t, vec2i>(inputProperty).data());
                    break;
                case EPropertyType::Vec3f:
                    m_ramsesAppearance.get().setInputValueVector3f(uniform, static_cast<uint32_t>(arraySize), TypeUtils::FlattenArrayData<float, vec3f>(inputProperty).data());
                    break;
                case EPropertyType::Vec3i:
                    m_ramsesAppearance.get().setInputValueVector3i(uniform, static_cast<uint32_t>(arraySize), TypeUtils::FlattenArrayData<int32_t, vec3i>(inputProperty).data());
                    break;
                case EPropertyType::Vec4f:
                    m_ramsesAppearance.get().setInputValueVector4f(uniform, static_cast<uint32_t>(arraySize), TypeUtils::FlattenArrayData<float, vec4f>(inputProperty).data());
                    break;
                case EPropertyType::Vec4i:
                    m_ramsesAppearance.get().setInputValueVector4i(uniform, static_cast<uint32_t>(arraySize), TypeUtils::FlattenArrayData<int32_t, vec4i>(inputProperty).data());
                    break;
                case EPropertyType::String:
                case EPropertyType::Array:
                case EPropertyType::Struct:
                case EPropertyType::Bool:
                case EPropertyType::Int64:
                    assert(false && "This should never happen");
                    break;
                }
            }
        }
    }

    ramses::Appearance& RamsesAppearanceBindingImpl::getRamsesAppearance() const
    {
        return m_ramsesAppearance;
    }

    std::optional<EPropertyType> RamsesAppearanceBindingImpl::GetPropertyTypeForUniform(const ramses::UniformInput& uniform)
    {
        // Can't bind semantic uniforms
        if (uniform.getSemantics() != ramses::EEffectUniformSemantic::Invalid)
        {
            return std::nullopt;
        }

        return ConvertRamsesUniformTypeToPropertyType(uniform.getDataType());
    }
}

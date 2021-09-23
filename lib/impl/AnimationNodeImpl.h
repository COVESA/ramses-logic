//  -------------------------------------------------------------------------
//  Copyright (C) 2021 BMW AG
//  -------------------------------------------------------------------------
//  This Source Code Form is subject to the terms of the Mozilla Public
//  License, v. 2.0. If a copy of the MPL was not distributed with this
//  file, You can obtain one at https://mozilla.org/MPL/2.0/.
//  -------------------------------------------------------------------------

#pragma once

#include "impl/LogicNodeImpl.h"
#include "PropertyImpl.h"
#include <memory>
#include "ramses-logic/AnimationTypes.h"

namespace rlogic_serialization
{
    struct AnimationNode;
}

namespace flatbuffers
{
    template<typename T> struct Offset;
    class FlatBufferBuilder;
}

namespace rlogic::internal
{
    class SerializationMap;
    class DeserializationMap;

    class AnimationNodeImpl : public LogicNodeImpl
    {
    public:
        AnimationNodeImpl(AnimationChannels channels, std::string_view name) noexcept;

        [[nodiscard]] float getDuration() const;
        [[nodiscard]] const AnimationChannels& getChannels() const;

        std::optional<LogicNodeRuntimeError> update() override;

        [[nodiscard]] static flatbuffers::Offset<rlogic_serialization::AnimationNode> Serialize(
            const AnimationNodeImpl& animNode,
            flatbuffers::FlatBufferBuilder& builder,
            SerializationMap& serializationMap);
        [[nodiscard]] static std::unique_ptr<AnimationNodeImpl> Deserialize(
            const rlogic_serialization::AnimationNode& animNodeFB,
            ErrorReporting& errorReporting,
            DeserializationMap& deserializationMap);

    private:
        void updateChannel(size_t channelIdx);

        template <typename T>
        T interpolateKeyframes_linear(T lowerVal, T upperVal, float interpRatio);
        template <typename T>
        T interpolateKeyframes_cubic(T lowerVal, T upperVal, T lowerTangentOut, T upperTangentIn, float interpRatio, float timeBetweenKeys);

        AnimationChannels m_channels;
        float m_duration = 0.f;
        float m_elapsedPlayTime = 0.f;

        enum EInputIdx
        {
            EInputIdx_TimeDelta = 0,
            EInputIdx_Play,
            EInputIdx_Loop,
            EInputIdx_RewindOnStop
        };

        enum EOutputIdx
        {
            EOutputIdx_Progress = 0,
            EOutputIdx_ChannelsBegin // must be last
        };
    };
}

//  -------------------------------------------------------------------------
//  Copyright (C) 2021 BMW AG
//  -------------------------------------------------------------------------
//  This Source Code Form is subject to the terms of the Mozilla Public
//  License, v. 2.0. If a copy of the MPL was not distributed with this
//  file, You can obtain one at https://mozilla.org/MPL/2.0/.
//  -------------------------------------------------------------------------

#pragma once

#include "ramses-logic/APIExport.h"
#include <vector>
#include <string>

namespace rlogic
{
    class DataArray;

    /**
    * Interpolation types used for animations.
    */
    enum class EInterpolationType : uint8_t
    {
        Step,               ///< Switches the value in the middle between two keyframe values
        Linear,             ///< Interpolates using linear function
        Cubic,              ///< Interpolates using cubic function, see https://github.com/KhronosGroup/glTF/tree/master/specification/2.0?ts=4#appendix-c-spline-interpolation
        Linear_Quaternions, ///< Linear interpolation for vec4f values which are normalized for use as Quaternions after interpolation
        Cubic_Quaternions,  ///< Cubic interpolation for vec4f values which are normalized for use as Quaternions after interpolation
    };

    /**
    * Animation channel data bundle.
    * #timeStamps, #keyframes must always be provided,
    * tangents #tangentsIn, #tangentsOut are mandatory only for #rlogic::EInterpolationType::Cubic
    * interpolation, ignored for other interpolation types.
    * See GLTF 2.0 specification appendix C for details on tangents
    * (https://github.com/KhronosGroup/glTF/tree/master/specification/2.0?ts=4#appendix-c-spline-interpolation)
    */
    struct AnimationChannel
    {
        /// Name of the channel for identification when linking
        std::string name;
        /// Timestamps, must be of type float and have strictly ascending order
        const DataArray* timeStamps = nullptr;
        /// Keyframes to interpolate from, must be same size as #timeStamps
        const DataArray* keyframes = nullptr;
        /// Type of interpolation for this channel
        EInterpolationType interpolationType = EInterpolationType::Linear;
        /// Tangents in (mandatory only for cubic interpolation), must be same type and size as #keyframes
        const DataArray* tangentsIn = nullptr;
        /// Tangents out (mandatory only for cubic interpolation), must be same type and size as #keyframes
        const DataArray* tangentsOut = nullptr;

        /// Comparison operator
        bool operator==(const AnimationChannel& rhs) const
        {
            return timeStamps == rhs.timeStamps
                && keyframes == rhs.keyframes
                && interpolationType == rhs.interpolationType
                && tangentsIn == rhs.tangentsIn
                && tangentsOut == rhs.tangentsOut;
        }
        /// Comparison operator
        bool operator!=(const AnimationChannel& rhs) const
        {
            return !operator==(rhs);
        }
    };
    using AnimationChannels = std::vector<AnimationChannel>;
}

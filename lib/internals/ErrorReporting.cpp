//  -------------------------------------------------------------------------
//  Copyright (C) 2020 BMW AG
//  -------------------------------------------------------------------------
//  This Source Code Form is subject to the terms of the Mozilla Public
//  License, v. 2.0. If a copy of the MPL was not distributed with this
//  file, You can obtain one at https://mozilla.org/MPL/2.0/.
//  -------------------------------------------------------------------------

#include "internals/ErrorReporting.h"
#include "ramses-logic/LogicNode.h"
#include "impl/LoggerImpl.h"

namespace rlogic::internal
{
    void ErrorReporting::add(std::string errorMessage, LogicObject* logicObject)
    {
        if (logicObject)
        {
            LOG_ERROR("[{}] {}", logicObject->getName(), errorMessage);
        }
        else
        {
            LOG_ERROR(errorMessage);
        }

        m_errors.emplace_back(ErrorData{ std::move(errorMessage), logicObject });
    }

    void ErrorReporting::clear()
    {
        m_errors.clear();
    }

    const std::vector<ErrorData>& ErrorReporting::getErrors() const
    {
        return m_errors;
    }

}

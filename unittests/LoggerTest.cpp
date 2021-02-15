//  -------------------------------------------------------------------------
//  Copyright (C) 2020 BMW AG
//  -------------------------------------------------------------------------
//  This Source Code Form is subject to the terms of the Mozilla Public
//  License, v. 2.0. If a copy of the MPL was not distributed with this
//  file, You can obtain one at https://mozilla.org/MPL/2.0/.
//  -------------------------------------------------------------------------

#include "gtest/gtest.h"
#include "gmock/gmock-matchers.h"

#include "ramses-logic/Logger.h"
#include "ramses-logic/ELogMessageType.h"

#include "impl/LoggerImpl.h"

#include "LogTestUtils.h"

namespace rlogic
{
    // Test default state without fixture
    TEST(ALogger_ByDefault, HasInfoAsVerbosityLimit)
    {
        EXPECT_EQ(ELogMessageType::INFO, Logger::GetLogVerbosityLimit());
    }

    class ALogger : public ::testing::Test
    {
    protected:
        std::vector<ELogMessageType> m_logTypes;
        std::vector<std::string> m_logMessages;
        ScopedLogContextLevel m_logCollector{ ELogMessageType::DEBUG, [this](ELogMessageType type, std::string_view message)
            {
                m_logTypes.emplace_back(type);
                m_logMessages.emplace_back(message);
        }
        };
    };

    TEST_F(ALogger, LogsDifferentLogLevelsSequentially)
    {
        LOG_INFO("Info");
        LOG_ERROR("Error");
        LOG_WARN("Warning");
        LOG_DEBUG("Debug");

        EXPECT_THAT(m_logTypes, ::testing::ElementsAre(ELogMessageType::INFO, ELogMessageType::ERROR, ELogMessageType::WARNING, ELogMessageType::DEBUG));
        EXPECT_THAT(m_logMessages, ::testing::ElementsAre("Info", "Error", "Warning", "Debug"));
    }

    TEST_F(ALogger, LogsFormattedMessage)
    {
        LOG_INFO("Info Message {}", 42);

        EXPECT_EQ(m_logMessages[0], "Info Message 42");
    }

    TEST_F(ALogger, LogsFormattedMessageWithMultipleArgumentsAndTypes)
    {
        LOG_INFO("Info Message {} {} {} {}", 42, 0.5f, "bool:", true);

        EXPECT_EQ(m_logMessages[0], "Info Message 42 0.5 bool: true");
    }

    TEST_F(ALogger, SetsDefaultLoggingOffAndOnAgain)
    {
        Logger::SetDefaultLogging(false);
        LOG_INFO("Info Message {} {} {}", 42, 42.0f, "42");
        Logger::SetDefaultLogging(true);
        LOG_INFO("Info Message {} {} {}", 42, 42.0f, "43");

        // Can't expect anything because default logging goes to stdout
    }

    TEST_F(ALogger, SetsDefaultLoggingOff_DoesNotAffectCustomLogHandler)
    {
        Logger::SetDefaultLogging(false);

        LOG_INFO("info");
        EXPECT_EQ(m_logMessages[0], "info");

        // Reset to not affect other tests
        Logger::SetDefaultLogging(true);
    }

    TEST_F(ALogger, ChangesLogVerbosityAffectsWhichMessagesAreProcessed)
    {
        Logger::SetLogVerbosityLimit(ELogMessageType::ERROR);

        // Simulate logs of all types
        LOG_DEBUG("debug");
        LOG_ERROR("error");
        LOG_WARN("warning");
        LOG_INFO("info");
        LOG_DEBUG("debug");
        LOG_ERROR("error");

        EXPECT_THAT(m_logTypes, ::testing::ElementsAre(ELogMessageType::ERROR, ELogMessageType::ERROR));
    }
}

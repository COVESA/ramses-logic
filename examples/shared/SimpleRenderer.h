//  -------------------------------------------------------------------------
//  Copyright (C) 2022 BMW AG
//  -------------------------------------------------------------------------
//  This Source Code Form is subject to the terms of the Mozilla Public
//  License, v. 2.0. If a copy of the MPL was not distributed with this
//  file, You can obtain one at https://mozilla.org/MPL/2.0/.
//  -------------------------------------------------------------------------

#pragma once

#include "ramses-client.h"

#include "ramses-renderer-api/RendererConfig.h"
#include "ramses-renderer-api/DisplayConfig.h"
#include "ramses-renderer-api/RamsesRenderer.h"
#include "ramses-renderer-api/RendererSceneControl.h"
#include "ramses-renderer-api/IRendererEventHandler.h"

#include <memory>
#include <vector>

// Simple header-only class which creates a ramses framework, client and renderer and can show a scene
class SimpleRenderer : public ramses::RendererEventHandlerEmpty
{
public:
    SimpleRenderer()
    {
        const std::vector<const char*> args = {"example", "-l", "0"};
        const auto argc = static_cast<int32_t>(args.size());
        ramses::RamsesFrameworkConfig frameworkConfig(argc, args.data());
        frameworkConfig.setPeriodicLogsEnabled(false);
        m_framework = std::make_unique<ramses::RamsesFramework>(frameworkConfig);

        m_ramsesClient = m_framework->createClient("example client");

        ramses::RendererConfig config(argc, args.data());
        m_renderer = m_framework->createRenderer(config);
        m_renderer->startThread();

        ramses::DisplayConfig displayConfig(argc, args.data());
        displayConfig.setWindowRectangle(100, 100, GetDisplaySize()[0], GetDisplaySize()[1]);
        m_display = m_renderer->createDisplay(displayConfig);
        m_renderer->flush();

    }

    [[nodiscard]] ramses::RamsesClient* getClient()
    {
        return m_ramsesClient;
    }

    void showScene(ramses::sceneId_t sceneId)
    {
        ramses::RendererSceneControl* sceneControlAPI = m_renderer->getSceneControlAPI();
        sceneControlAPI->setSceneMapping(sceneId, m_display);
        sceneControlAPI->setSceneState(sceneId, ramses::RendererSceneState::Rendered);
        sceneControlAPI->flush();
    }

    void processEvents()
    {
        m_renderer->dispatchEvents(*this);
    }

    void windowClosed(ramses::displayId_t /*displayId*/) override
    {
        m_windowClosed = true;
    }

    [[nodiscard]] bool isWindowClosed() const
    {
        return m_windowClosed;
    }

    [[nodiscard]] static std::array<uint32_t, 2> GetDisplaySize()
    {
        return { 1280u, 480u };
    }

private:
    std::unique_ptr<ramses::RamsesFramework> m_framework;
    ramses::RamsesClient* m_ramsesClient;
    ramses::RamsesRenderer* m_renderer;
    ramses::displayId_t m_display;
    bool m_windowClosed = false;
};

#include "engine.h"

namespace Electric {

    void ElectricEngine::clear()
    {

        LOGI("ElectricEngine( Cleared: %d )", m_id);
    }

    bool ElectricEngine::init(AAssetManager* pAssetManager)
    {
        LOGI("ElectricEngine( Init begin: %d )", m_id);

        if(m_updater.init())
            return false;

        LOGI("ElectricEngine( Init end: %d )", m_id);
        return true;
    }

    bool ElectricEngine::render()
    {
        glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
        glDisable(GL_DEPTH_TEST);
        glDepthFunc(GL_LEQUAL);
        glDisable(GL_CULL_FACE);
        glCullFace(GL_BACK);

        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glBlendEquation(GL_FUNC_ADD);
        glViewport(m_viewport[0], m_viewport[1], m_viewport[2], m_viewport[3]);
        glClear(GL_COLOR_BUFFER_BIT);

        Matrix vp;
        {
            std::lock_guard<std::mutex> _(m_matrixMutex);
            if(m_vpDirty)
                m_vp = m_projection * m_view;
            vp = m_vp;
        }



        return true;
    }

}
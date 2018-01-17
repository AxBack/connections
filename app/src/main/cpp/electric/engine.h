#pragma once

#include "../engine/engine.h"
#include "updater.h"

#include "../engine/mesh.h"
#include "particle_shader.h"
#include "node_shader.h"
#include "charge_shader.h"
#include "../engine/framebuffer.h"
#include "../engine/path.h"
#include "../engine/vector3.h"

namespace Electric {

    typedef Engine::Mesh<PositionVertex, ParticleInstance> ParticleMesh;
    typedef Engine::Mesh<PositionVertex, NodeInstance> NodeMesh;
	typedef Engine::Mesh<ChargeVertex, ChargeInstance> ChargeMesh;
    typedef Engine::Camera Camera;
	typedef Engine::Framebuffer Framebuffer;
    typedef Engine::Path<Math::Vector3> vec3_path;

    class ElectricEngine : public Engine::Engine
    {
    private:

        //temp
        float           m_posTime;
        std::atomic<float> m_offset;
        vec3_path       m_positionPath;
        vec3_path	    m_rotationPath;
        Camera 			m_camera;

        Updater     	m_updater;

        ParticleMesh    m_particlesMesh;
        ParticleShader  m_particleShader;

        NodeMesh        m_nodeMesh;
        NodeShader      m_nodeShader;

		ChargeMesh		m_chargeMesh;
		ChargeShader	m_chargeShader;

		std::atomic_bool m_sizeDirty;
		std::mutex 		 m_sizeMutex;
		Framebuffer		 m_renderTarget;

    public:

        ElectricEngine()
		: m_sizeDirty(false)
        , m_posTime(0.0f)
        , m_offset(0.5f)
        {
            {
                Math::Vector3 points[] = {
                        {0, 0, -1.0f},
                        {0, 0, -100.0f}
                };
                m_positionPath.add(10.0f, 2, points);
            }

            {
                Math::Vector3 points[] = {
                        {0, -180.0f, 0},
                        {0, 180.0f,  0}
                };
                m_rotationPath.add(1.0f, 2, points);
            }
        }

        virtual void clear() override;
        virtual bool init(AAssetManager* pAssetManager) override;
        virtual bool render() override;

        virtual void resume() override { m_updater.resume(); }
        virtual void pause() override { m_updater.pause(); }

        virtual void updateSize(int width, int height) override
        {
			if(width != m_viewport[2] || height != m_viewport[3])
			{
				m_sizeDirty = true;
				std::lock_guard<std::mutex> _(m_sizeMutex);
				Engine::Engine::updateSize(width, height);
				m_camera.updateProjection(static_cast<float>(width), static_cast<float>(height));
			}
		}

        virtual void setOffset(float x, float y) override
        {
            Engine::Engine::setOffset(x, y);
            m_offset = x;
        }
    };

}
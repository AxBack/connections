#include "updater.h"

#include <chrono>
#include <cmath>

#define EPSILON 0.0001f

UINT Updater::sThreadCounter = 0;

bool Updater::init()
{
	stop();

	return true;
}

void Updater::start()
{
	if(m_running)
		return;

	m_running = true;
	m_workThread = std::thread([=]() { this->run(); });
	LOGI("WorkerThread( Started: %d )", m_id);
}

void Updater::stop()
{
	if(!m_running)
		return;

    m_running = false;

	if(m_workThread.joinable())
	{
		try
		{
			m_workThread.join();
			LOGI("WorkerThread( Ended: %d )", m_id);
		}
		catch(...)
		{
			LOGE("WorkerThread( Failed to join: %d )", m_id);
		}
	}
}

void Updater::run()
{
	using framerate = std::chrono::duration<std::chrono::steady_clock::rep, std::ratio<1, 60>>;
	auto next = std::chrono::steady_clock::now() + framerate(1);
	float dt = 1.0f/60.0f;

	while(true)
	{
		std::this_thread::sleep_until(next);
		next += framerate{1};

		advance(dt);

		if(!m_running)
			break;
	}
}

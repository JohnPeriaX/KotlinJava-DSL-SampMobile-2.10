#include "CFPSFix.h"
#include "main.h"
#include <sys/syscall.h>
#include <algorithm>

static void setThreadAffinityMask(pid_t tid, uint32_t mask)
{
	syscall(__NR_sched_setaffinity, tid, sizeof(mask), &mask);
}

void CFPSFix::Routine()
{
	while (m_Running)
	{
		{
			std::lock_guard<std::mutex> lock(m_Mutex);
			for (auto& i : m_Threads)
			{
				uint32_t mask = 0xff;
				setThreadAffinityMask(i, mask);
			}
		}
		
		std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	}
}

CFPSFix::CFPSFix() : m_Running(true)
{
	m_WorkerThread = std::thread(&CFPSFix::Routine, this);
}

CFPSFix::~CFPSFix()
{
	m_Running = false;
	if (m_WorkerThread.joinable())
	{
		m_WorkerThread.join();
	}
}

void CFPSFix::PushThread(pid_t tid)
{
	std::lock_guard<std::mutex> lock(m_Mutex);
	
	// Check if thread already exists to avoid duplicates
	if (std::find(m_Threads.begin(), m_Threads.end(), tid) == m_Threads.end())
	{
		m_Threads.push_back(tid);
	}
}
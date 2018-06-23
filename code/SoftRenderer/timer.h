/**
  @file timer.h
  
  @brief ��ʱ���࣬srʹ�õ�ʱ��ӿ�

  @author Kaiming

  ������־ history
  ver:1.0
   
 */

#ifndef timer_h__
#define timer_h__

#include <MMSystem.h>

class SrTimer
{
public:
	SrTimer():m_time(0)
		,m_elapsedTime(0)
	{

	}
	~SrTimer()
	{

	}

	void Init()
	{
		m_time = timeGetTime() / 1000.f;
		m_elapsedTime = 0;
		QueryPerformanceFrequency(&m_freq); // ��ȡcpuʱ������   
	}

	void Update()
	{
		float newTime = timeGetTime() / 1000.f;
		m_elapsedTime = newTime - m_time;
		m_time = newTime;
	}

	void Stop()
	{
		m_elapsedTime = 0;
	}

	float getElapsedTime()
	{
		return m_elapsedTime;
	}

	float getTime()
	{
		float newTime = timeGetTime() / 1000.f;
		return newTime;
	}

	float getRealTime()
	{
		LARGE_INTEGER now;
		QueryPerformanceCounter(&now); // ��ȡcpuʱ�Ӽ���
		return (float)(now.QuadPart)/m_freq.QuadPart;
	}

private:
	float m_time;
	float m_elapsedTime;

	// high precision
	LARGE_INTEGER m_freq;
};

#endif
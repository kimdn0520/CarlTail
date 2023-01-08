#include "GamePch.h"
#include "Timer.h"

std::shared_ptr<GameEngineSpace::Timer> GameEngineSpace::Timer::instance = nullptr;

namespace GameEngineSpace
{

	std::shared_ptr<Timer> Timer::GetInstance()
	{
		if (instance == nullptr)
			instance = std::make_shared<Timer>();

		return instance;
	}

	void Timer::Init()
	{
		secondsPerCount = 0.0f;
		deltaTime = 0.0f;
		baseTime = 0;
		stopTime = 0;
		pausedTime = 0;
		prevTime = 0;
		currTime = 0;
		isStopped = false;

		int64 _countsPerSec;

		QueryPerformanceFrequency(reinterpret_cast<LARGE_INTEGER*>(&_countsPerSec));
		secondsPerCount = 1.0f / static_cast<float>(_countsPerSec);
	}

	void Timer::Release()
	{

	}

	float Timer::TotalTime() const
	{
		if (isStopped == true)
			return static_cast<float>(((stopTime - pausedTime) - baseTime) * secondsPerCount);
			// ���� �ð����� ���� �ð��� ����. �� ���̿� �Ͻ������� �ð��� ���� �� �ִ�.
		else		
			return static_cast<float>(((currTime - pausedTime) - baseTime) * secondsPerCount);		
	}

	float Timer::DeltaTime() const
	{
		return static_cast<float>(deltaTime);

	}

	void Timer::Reset()
	{
		int64 _currTime;
		QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER*>(&_currTime));

		baseTime = _currTime;
		prevTime = 0;
		stopTime = 0;
		isStopped = false;
	}

	void Timer::Start()
	{
		int64 _startTime;
		QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER*>(&_startTime));

		if (isStopped == true)
		{
			pausedTime += (_startTime - stopTime);

			prevTime = _startTime;

			stopTime = 0;
			isStopped = false;
		}
	}


	void Timer::Stop()
	{
		if(isStopped != true)
		{
			int64 _currTime;
			QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER*>(&_currTime));

			// �ߴ� �ð��� ���� �� �ߴ� �Ǿ��ٰ� �˸�.
			stopTime = _currTime;
			isStopped = true;
		}
	}

	void Timer::Tick()
	{
		if (isStopped == true)
		{
			deltaTime = 0.0f;
			return;
		}

		// �̹� �����ӿ� �ð��� ���
		int64 _currTime;
		QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER*>(&_currTime));
		currTime = _currTime;

		// ��Ÿ Ÿ���� ���Ѵ�..
		deltaTime = (currTime - prevTime) * secondsPerCount;

		// ���� �������� �غ��ϰ�.
		prevTime = currTime;

		// ��Ÿ Ÿ���� ������ ���� �ʰԲ� �Ѵ�.
		if (deltaTime < 0.0f)
			deltaTime = 0.0f;
	}
}

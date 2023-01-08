#pragma once
#include "Define/FootEngineTypedef.h"

namespace GameEngineSpace
{
	/**
	 * \brief ��å ������ Timer Ŭ����
	 *
	 * 2022.08.01. DevBlackFoot.
	 */
	class Timer
	{
	private:
		static std::shared_ptr<Timer> instance;

		float secondsPerCount;
		float deltaTime;

		int64 baseTime;
		int64 pausedTime;
		int64 stopTime;
		int64 prevTime;
		int64 currTime;

		bool isStopped;


	public:
		Timer() = default;
		~Timer() = default;
		Timer(const Timer& _other) = delete;
		const Timer& operator=(const Timer& _other) = delete;
		Timer(const Timer&& _other) = delete;
		const Timer& operator=(const Timer&& _other) = delete;

		static std::shared_ptr<Timer> GetInstance();

		void Init();
		void Release();

		float TotalTime() const;
		float DeltaTime() const;

		void Reset();
		void Start();
		void Stop();
		void Tick();

	};

}

#pragma once


namespace GameEngineSpace
{
	/**
	 * \brief �Ŵ������� �̿��ؼ� ���� ������Ʈ ��Ű�� ���ӿ���
	 */
	class GameEngine
	{
	private:
		static std::shared_ptr<GameEngine> gameEngineInstance;

	public:
		void Init(tstring gameTitle, HINSTANCE hInst, int screenWidth, int screenHeight);

		INT Tick();

		void Release();

		static std::shared_ptr<GameEngine> GetEngineInstance() { return gameEngineInstance; }
	};
}

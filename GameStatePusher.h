#pragma once
#ifndef GAMESTATE_PUSHER_H
#define GAMESTATE_PUSHER_H

#include <functional>
#include <cppgamelib/processes/ProcessManager.h>
#include <cppgamelib/time/PeriodicTimer.h>

namespace gamelib
{
	class Process;
}

namespace gamenetlib
{
	class GameStatePusher
	{
	public:
		void Initialise();
		GameStatePusher(const std::function<void()>& sendGameStateFunc, int sendRateMs, bool isGameServer,
		                gamelib::ProcessManager& processManager);
		void Run() const;

	private:
		bool isGameServer;

		// Periodically send game state to the game server
		int sendRateMs;

		std::shared_ptr<gamelib::Process> sendTask;

		gamelib::PeriodicTimer gameStateSendInterval;
		gamelib::ProcessManager& processManager;  // NOLINT(cppcoreguidelines-avoid-const-or-ref-data-members)
		std::function<void()> sendGameStateFunc;
	};
}

#endif


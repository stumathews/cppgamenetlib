#include <cppgamelib/processes/Process.h>
#include <cppgamelib/processes/ProcessManager.h>
#include "cppgamelib/processes/Action.h"

#include "GameStatePusher.h"


namespace gamenetlib
{
	GameStatePusher::GameStatePusher(const std::function<void()>& sendGameStateFunc, const int sendRateMs, const bool isGameServer,
	                                 gamelib::ProcessManager& processManager): isGameServer(isGameServer), sendRateMs(sendRateMs), processManager(processManager),
	                                                                           sendGameStateFunc(sendGameStateFunc)
	{
	}

	void GameStatePusher::Initialise()
	{
		if (isGameServer)
		{
			// We won't send out periodic state to the game server if we are the game server
			return;
		}

		// Send game state every sendRateMs
		gameStateSendInterval.SetFrequency(sendRateMs);

		// create task to send the game state as a background task
		sendTask = std::dynamic_pointer_cast<gamelib::Process>(std::make_shared<gamelib::Action>([&](const unsigned long deltaMs)
		{
			// Update timers
			gameStateSendInterval.Update(deltaMs);

			// Send game state of sending interval is elapsed
			gameStateSendInterval.DoIfReady([&]()
			{
				if (sendGameStateFunc) 
				{
					sendGameStateFunc();
				}
			});
		}, false));
	}

	void GameStatePusher::Run() const
	{
		// Add as a background task
		processManager.AttachProcess(sendTask);
	}
}

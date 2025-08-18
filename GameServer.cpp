#include <cppgamelib/events/EventManager.h>
#include <cppgamelib/file/Logger.h>

#include "GameServer.h"
#include "Networking.h"
#include "EventFactory.h"
#include "cppgamelib/events/PlayerMovedEvent.h"
#include "StartNetworkLevelEvent.h"

using namespace json11;


namespace gamenetlib
{
	GameServer::GameServer(const std::string& address, const std::string& port,
	                       const std::shared_ptr<IGameServerConnection> gameServerConnection,
	                       const std::string& nickName, const gamenetlib::Encoding wireFormat)
	{		
		this->gameServerConnection = gameServerConnection;
		this->nickname = nickName;
		this->Address = address;
		this->Port = port;
		this->encoding = wireFormat;
	}

	void GameServer::Initialize()
	{
		gamelib::Logger::Get()->LogThis("Game server initializing...");
		
		serializationManager = std::make_shared<SerializationManager>(encoding);
		eventManager = gamelib::EventManager::Get();
		networking = Networking::Get();
		eventFactory = EventFactory::Get();
		
		gameServerConnection->Initialize();
		gameServerConnection->Create();
		
		// We're interested in some of the our own game's events
		eventManager->SubscribeToEvent(gamelib::PlayerMovedEventTypeEventId, this);
		eventManager->SubscribeToEvent(gamelib::ControllerMoveEventId, this);
		eventManager->SubscribeToEvent(gamenetlib::StartNetworkLevelEventId, this);

		gamelib::Logger::Get()->LogThis("Game server initialization complete.");
	}
	
	void GameServer::Listen(const unsigned long deltaMs) const
	{		
		gameServerConnection->Listen(deltaMs);
	}
		
	void GameServer::CheckForPlayerTraffic(const unsigned long deltaMs) const
	{
		gameServerConnection->CheckForPlayerTraffic(deltaMs);		
	}

	void GameServer::Disconnect() const
	{
		gameServerConnection->Disconnect();
	}

	std::vector<std::shared_ptr<gamelib::Event>> GameServer::HandleEvent(const std::shared_ptr<gamelib::Event>& evt, const unsigned long deltaMs)
	{
		gameServerConnection->SendEventToAllPlayers(serializationManager->SerializeEvent(evt, nickname)); 

		return {};
	}

	std::string GameServer::GetSubscriberName()
	{
		return "Game Server";
	}
		
	GameServer::~GameServer() = default;
}

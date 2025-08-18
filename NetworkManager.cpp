#include <cppgamelib/file/SettingsManager.h>

#include "NetworkManager.h"
#include "Networking.h"
#include "GameClient.h"
#include "GameServerConnectionFactory.h"
#include "GameClientConnectionFactory.h"
#include "GameServer.h"
#include "Networking.h"
#include "SerializationManager.h"

namespace gamenetlib
{
	NetworkManager::NetworkManager()
	{
		// NB: both game server and game client use the same underlying settings

		isGameServer = gamelib::SettingsManager::Get()->GetBool("networking", "isGameServer");		
		gameServerAddress = gamelib::SettingsManager::Get()->GetString("networking", "gameServerAddress");
		gameServerPort = gamelib::SettingsManager::Get()->GetString("networking", "gameServerPort");
		maxPlayers = gamelib::SettingsManager::Get()->GetInt("networking", "maxPlayers");
		nickName = gamelib::SettingsManager::Get()->GetString("networking", "nickname");
		isTcp = gamelib::SettingsManager::Get()->GetBool("networking", "isTcp");
		useEncryption = gamelib::SettingsManager::Get()->GetBool("networking", "useEncryption");
		encoding = static_cast<Encoding>(gamelib::SettingsManager::Get()->GetInt("networking", "encoding"));
	}

	bool NetworkManager::Initialize()
	{
		Networking::Get()->InitializeWinSock();

		// Determine what protocol we are going to use for connections
		const auto useReliableUdp = gamelib::SettingsManager::Get()->GetBool("networking", "useReliableUdp");
		const auto sendClientEventsToServer = gamelib::SettingsManager::Get()->GetBool("networking", "sendClientEventsToServer");
		
		// eg. tcp, udp, reliable-udp connection
		auto gameServerConnection = GameServerConnectionFactory::Create(isTcp, gameServerAddress, gameServerPort,
		                                                                useReliableUdp, useEncryption, encoding);
		// Game server object needs to be created when Network Manager is acting
		// as a game server (will initialize it) or game client (will connect to game server)
		Server = std::make_shared<GameServer>(gameServerAddress, gameServerPort, gameServerConnection, nickName, encoding);

		if(isGameServer)
		{
			Server->Initialize();
		}
		else
		{
			// eg. UDP or TCP connection
			auto gameClientConnection = GameClientConnectionFactory::Create(isTcp);

			Client = std::make_shared<GameClient>(nickName, gameClientConnection, useReliableUdp, useEncryption,
			                                      encoding, sendClientEventsToServer);
			Client->Initialize();
			Client->Connect(Server);
		}

		return true;
	}

	void NetworkManager::Listen(const unsigned long deltaMs) const
	{
		if (gamelib::SettingsManager::Get()->GetBool("global", "isNetworkGame"))
		{
			if (isGameServer)
			{
				Server->Listen(deltaMs);
			}
			else
			{
				Client->Read(deltaMs);
			}
		}
	}
	
	void NetworkManager::PingGameServer(const unsigned long deltaMs) const
	{
		if(!isGameServer)
		{
			Client->PingGameServer(deltaMs);
		}
	}
		
	bool NetworkManager::IsGameServer() const
	{
		return isGameServer;
	}


	NetworkManager* NetworkManager::Get()
	{
		if (instance == nullptr)
		{
			instance = new NetworkManager();
		}
		return instance;
	}

	NetworkManager* NetworkManager::instance = nullptr;
		
	NetworkManager::~NetworkManager()
	{		
		instance = nullptr;
	}	
}

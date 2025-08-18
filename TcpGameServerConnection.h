#pragma once
#include <memory>
#include <string>
#include <WinSock2.h>
#include <vector>


#include "TcpNetworkPlayer.h"
#include "GameServer.h"

namespace gamenetlib
{
	class Event;
	class Networking;
	class SerializationManager;
	class EventFactory;

	class TcpGameServerConnection final : public IGameServerConnection,  public gamelib::EventSubscriber
	{
	public:

		TcpGameServerConnection(const std::string& host, const std::string& port, Encoding wireFormat);
		~TcpGameServerConnection() override;
		void Initialize() override;
	private:
		SOCKET listeningSocket{};
		std::string host, port;

		std::shared_ptr<SerializationManager> serializationManager;
		gamelib::EventManager* _eventManager;
		Networking* networking;
		EventFactory* _eventFactory;
		Encoding encoding;


		// Inherited via IGameServerConnection
		void CheckForPlayerTraffic(unsigned long deltaMs) override;
		void ProcessPingMessage(const size_t& playerId) const;
		void SendToConnectedPlayersExceptToSender(const std::string& senderNickname, const std::string&
		                                          serializedMessage, const size_t& playerId) const;
		void RaiseNetworkTrafficReceivedEvent(char buffer[512], const size_t& i, int bytesReceived);
		void ParseReceivedPlayerPayload(const size_t& playerId, const char* inPayload, int payloadLength);
		void ProcessRequestPlayerDetailsMessage(int playerId, const MessageHeader& messageHeader);
		void SendEventToAllPlayers(std::string serializedEvent) override;
		fd_set readfds{};

		// Inherited via IGameServerConnection
		void Listen(const unsigned long deltaMs) override;
		void CheckForNewTcpPlayers();
		/// <summary>
		/// How long to wait for network data the arrive {0,0} means non-blocking
		/// </summary>
		timeval timeout{};
		std::vector<TcpNetworkPlayer> Players;

		// Inherited via EventSubscriber
		std::vector<std::shared_ptr<gamelib::Event>> HandleEvent(const std::shared_ptr<gamelib::Event>& evt, const unsigned long deltaMs) override;
		std::string GetSubscriberName() override;

		// Inherited via IGameServerConnection
		void Create() override;

	public:
		void Disconnect() override;
	};
}


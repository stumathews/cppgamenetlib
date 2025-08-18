#pragma once

#include <string>
#include <WinSock2.h>
#include <vector>
#include <cppgamelib/events/Event.h>
#include <cppgamelib/events/EventSubscriber.h>

#include "IGameServerConnection.h"
#include "UdpNetworkPlayer.h"
#include "PeerInfo.h"
#include "SerializationManager.h"
#include "Security.h"

namespace gamelib
{
	class EventFactory;
	class Networking;
	class EventManager;
}

namespace gamenetlib
{
	class Networking;
	class EventFactory;

	class UdpGameServerConnection : public IGameServerConnection, public gamelib::EventSubscriber
	{
	public:
		UdpGameServerConnection(const std::string& host, const std::string& port, gamenetlib::Encoding wireFormat);
		// Inherited via IGameServerConnection
		void Initialize() override;
	protected:		
		SOCKET listeningSocket{};
		Encoding Encoding;
		void RaiseNetworkTrafficReceivedEvent(const char* buffer, int bytesReceived, PeerInfo fromClient);
		void ParseReceivedPlayerPayload(const char* inPayload, int payloadLength, PeerInfo fromClient);
	private:
		std::string host, port;

		std::shared_ptr<SerializationManager> serializationManager;
		gamelib::EventManager* eventManager;
		Networking* networking;
		EventFactory* eventFactory;

		// Inherited via IGameServerConnection
		void CheckForPlayerTraffic(unsigned long deltaMs) override;
		
		virtual int InternalSend(SOCKET socket, const char* buf, int len, int flags, const sockaddr* to, int toLen, unsigned long sendTimeMs = 0);
		void SendToConnectedPlayersExceptToSender(const std::string& senderNickname, const std::string&
		                                          serializedMessage);
		timeval timeout{};
		fd_set readfds{};

		// Inherited via IGameServerConnection
		void Listen(unsigned long deltaMs) override;

		// Inherited via IGameServerConnection
		void ProcessPingMessage(PeerInfo fromClient);
		void ProcessRequestPlayerDetailsMessage(const MessageHeader& messageHeader, PeerInfo fromClient);
		
		std::vector<UdpNetworkPlayer> players;

		// Inherited via IGameServerConnection
		void SendEventToAllPlayers(std::string serializedEvent) override;

		// Inherited via EventSubscriber
		std::vector<std::shared_ptr<gamelib::Event>> HandleEvent(const std::shared_ptr<gamelib::Event>& evt, unsigned long deltaMs) override;
		std::string GetSubscriberName() override;

		// Inherited via IGameServerConnection
		void Create() override;
		
		
	protected:		
		constexpr static auto ReadBufferMaxElements = 300;
		constexpr static auto ReadBufferSizeInBytes = ReadBufferMaxElements * 32 / 8;
		uint32_t readBuffer[ReadBufferMaxElements]{};
		
		SecuritySide securitySide;
		unsigned char remotePublicKey[SecuritySide::PublicKeyLengthBytes]{0};
		unsigned char sharedNonce[SecuritySide::NonceLengthBytes]{0};
	public:
		void Disconnect() override;
	};
}


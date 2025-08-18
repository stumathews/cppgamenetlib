#pragma once
#include <memory>
#include <xstring>

#include <cppgamelib/events/EventSubscriber.h>
#include <cppgamelib/ai/FSM.h>
#include <cppgamelib/events/EventSubscriber.h>
#include <cppgamelib/time/PeriodicTimer.h>

#include "IGameServerConnection.h"
#include "IProtocolManager.h"
#include "BitPacker.h"
#include "IConnectedNetworkSocket.h"
#include "ReliableUdp.h"
#include "Security.h"
#include "ReliableUdp.h"
#include "Security.h"
#ifndef NETWORK_PROTOCOL_MANAGER_H
#define NETWORK_PROTOCOL_MANAGER_H
#pragma once

namespace gamenetlib
{


	class ReliableUdpProtocolManager : public IProtocolManager, public gamelib::EventSubscriber
	{
		private:
			bool initialized{};

			/// <summary>
			/// The socket the game client will use to communicate with the game server
			/// </summary>
			std::shared_ptr<IConnectedNetworkSocket> gameClientConnection;
			SecuritySide clientSecuritySide;
			std::shared_ptr<IGameServerConnection> gameServerConnection;
			constexpr static auto PackingBufferElements = 300;
			constexpr static auto ReceiveBufferMaxElements = 300;
			uint32_t packingBuffer[PackingBufferElements]{};
			uint32_t readBuffer[ReceiveBufferMaxElements]{};
			unsigned char remotePublicKey[SecuritySide::PublicKeyLengthBytes]{0};
			unsigned char sharedNonce[SecuritySide::NonceLengthBytes]{0};
			ReliableUdp reliableUdp;

			bool sessionEstablished {false};
		    bool useEncryption {true}; 
			int SendInternal(const char* callersSendBuffer, int dataLength, unsigned long deltaMs, MessageType messageType);
			int ReceiveInternal(char* callersReceiveBuffer, int bufLength, unsigned long deltaMs);
		

		public:
			ReliableUdpProtocolManager(std::shared_ptr<IConnectedNetworkSocket> gameClientConnection, bool useEncryption = true);
			ReliableUdpProtocolManager(std::shared_ptr<IGameServerConnection> gameServerConnection, bool useEncryption = true);

			// Cannot copy
			ReliableUdpProtocolManager(ReliableUdpProtocolManager const&) = delete;

			// Cannot assign to
			void operator=(ReliableUdpProtocolManager const&) = delete;
		
			bool Initialize() override;		
			std::shared_ptr<IConnectedNetworkSocket> GetConnection() override;
			void Connect(const char* address, const char* port) override;

			int Send(const char* callersSendBuffer, int dataLength, unsigned long deltaMs = 0) override;
			int Receive(char* callersReceiveBuffer, int bufLength, unsigned long deltaMs = 0) override;
			int SendAck(const Message& messageToAck, unsigned long sendTimeMs) override;
			static void Update(unsigned long deltaMs);
			std::string GetSubscriberName() override;
			std::vector<std::shared_ptr<gamelib::Event>> HandleEvent(const std::shared_ptr<gamelib::Event>& evt, unsigned long deltaMs) override;
			~ReliableUdpProtocolManager() override;
			int GetSubscriberId() override;
	};

}

#endif


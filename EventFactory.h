#pragma once

#include <cppgamelib/events/ControllerMoveEvent.h>
#include <cppgamelib/character/Direction.h>

#include <memory>
#include <string>

#include "GameServerConnectionFactory.h"
#include "NetworkPlayer.h"
#include "Message.h"
#include "Rtt.h"

namespace gamelib
{
	class GameObject;
	class AddGameObjectToCurrentSceneEvent;
	class UpdateProcessesEvent;
	class UpdateAllGameObjectsEvent;
	class SceneChangedEvent;
	class PlayerMovedEvent;
}

namespace gamenetlib
{
	class StartNetworkLevelEvent;
	class ReliableUdpPacketRttCalculatedEvent;
	class ReliableUdpPacketLossDetectedEvent;
	class ReliableUdpAckPacketEvent;
	class ReliableUdpCheckSumFailedEvent;
	class ReliableUdpPacketReceivedEvent;
	class NetworkTrafficReceivedEvent;
	class IEventSerializationManager;

	class EventFactory
	{
	protected:
			static EventFactory* instance;
	private:
			std::shared_ptr<IEventSerializationManager> eventSerializationManager;
	public:
		static EventFactory* Get();
		EventFactory();

		// Cannot copy an EventFactory
		EventFactory(EventFactory const&) = delete;
	
		~EventFactory();
		
		// Cannot assign to an EventFactory
		void operator=(EventFactory const&) = delete;

		[[nodiscard]] std::shared_ptr<gamelib::PlayerMovedEvent> CreatePlayerMovedEvent(gamelib::Direction direction, const std::string& target = "") const;
		[[nodiscard]] std::shared_ptr<gamelib::PlayerMovedEvent> CreatePlayerMovedEvent(const std::string& serializedMessage) const;
		[[nodiscard]] std::shared_ptr<NetworkTrafficReceivedEvent> CreateNetworkTrafficReceivedEvent(const std::string& message, const std::string& identifier, const int bytesReceived, const std::string& origin) const;
		[[nodiscard]] std::shared_ptr<gamelib::SceneChangedEvent> CreateLevelEvent(int level) const;
		[[nodiscard]] std::shared_ptr<gamelib::UpdateAllGameObjectsEvent> CreateUpdateAllGameObjectsEvent() const;
		[[nodiscard]] std::shared_ptr<gamelib::UpdateProcessesEvent> CreateUpdateProcessesEvent() const;
		[[nodiscard]] std::shared_ptr<gamenetlib::StartNetworkLevelEvent> CreateStartNetworkLevelEvent(int level) const;
		[[nodiscard]] std::shared_ptr<gamelib::Event> CreateNetworkPlayerJoinedEvent(const NetworkPlayer& player) const;
		[[nodiscard]] std::shared_ptr<gamelib::ControllerMoveEvent> CreateControllerMoveEvent(gamelib::Direction direction, gamelib::ControllerMoveEvent::KeyState keyState) const;
		[[nodiscard]] std::shared_ptr<gamelib::Event> CreateSubscriberHandledEvent(gamelib::IEventSubscriber* value, const std::shared_ptr<gamelib::Event>& event, unsigned long deltaMs) const;
		[[nodiscard]] std::shared_ptr<ReliableUdpPacketReceivedEvent> CreateReliableUdpPacketReceived(std::shared_ptr<Message> message) const;
		[[nodiscard]] std::shared_ptr<ReliableUdpCheckSumFailedEvent> CreateReliableUdpCheckSumFailedEvent(std::shared_ptr<Message> failedMessage) const;
		[[nodiscard]] std::shared_ptr<ReliableUdpPacketLossDetectedEvent> CreateReliableUdpPacketLossDetectedEvent(const std::shared_ptr<Message>& messageBundle) const;
		[[nodiscard]] std::shared_ptr<ReliableUdpAckPacketEvent> CreateReliableUdpAckPacketEvent(const std::shared_ptr<Message>& message, bool isSent) const;
		[[nodiscard]]std::shared_ptr<ReliableUdpPacketRttCalculatedEvent> CreateReliableUdpPacketRttCalculatedEvent(const std::shared_ptr<Message>& message, Rtt rtt) const;
		[[nodiscard]] static std::shared_ptr<gamelib::AddGameObjectToCurrentSceneEvent> CreateAddToSceneEvent(const std::shared_ptr<gamelib::GameObject>& obj);
		[[nodiscard]] std::shared_ptr<gamelib::SceneChangedEvent> CreateSceneChangedEventEvent(int newLevel) const;
		[[nodiscard]] std::shared_ptr<gamelib::Event> CreateGenericEvent(const gamelib::EventId& id, const std::string& origin) const;
	};
}


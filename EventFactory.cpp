
#include <cppgamelib/events/AddGameObjectToCurrentSceneEvent.h>
#include <cppgamelib/events/UpdateAllGameObjectsEvent.h>
#include <cppgamelib/events/UpdateProcessesEvent.h>
#include <cppgamelib/events/SubscriberHandledEvent.h>
#include <cppgamelib/events/PlayerMovedEvent.h>
#include <cppgamelib/events/SceneChangedEvent.h>

#include "ReliableUdpCheckSumFailedEvent.h"
#include "ReliableUdpPacketLossDetectedEvent.h"
#include "NetworkTrafficReceivedEvent.h"
#include "ReliableUdpAckPacketEvent.h"
#include "ReliableUdpPacketRttCalculatedEvent.h"
#include "ReliableUdpPacketReceivedEvent.h"
#include "EventFactory.h"
#include "StartNetworkLevelEvent.h"
#include "NetworkPlayerJoinedEvent.h"

namespace gamenetlib
{
	EventFactory* EventFactory::Get()
	{
		if (instance == nullptr)
		{
			instance = new EventFactory();
		}
		return instance;
	}

	EventFactory::EventFactory()
	{
		this->eventSerializationManager = std::make_shared<JsonEventSerializationManager>();
	}

	EventFactory* EventFactory::instance = nullptr;

	
	EventFactory::~EventFactory()
	{		
		instance = nullptr;
	}

	std::shared_ptr<gamelib::Event> EventFactory::CreateGenericEvent(const gamelib::EventId& id, const std::string& origin = "") const
	{
		auto event = std::make_shared<gamelib::Event>(id);
		event->Origin = origin;
		return event;
	}
	

	std::shared_ptr<gamelib::PlayerMovedEvent> EventFactory::CreatePlayerMovedEvent(const gamelib::Direction direction, const std::string
	                                                                                & target) const
	{
		return std::make_shared<gamelib::PlayerMovedEvent>(direction);
	}

	std::shared_ptr<gamelib::PlayerMovedEvent> EventFactory::CreatePlayerMovedEvent(const std::string& serializedMessage) const
	{
		return eventSerializationManager->DeserializePlayerMovedEvent(serializedMessage);
	}

	std::shared_ptr<NetworkTrafficReceivedEvent> EventFactory::CreateNetworkTrafficReceivedEvent(const std::string&
		message, const std::string& identifier, const int bytesReceived, const std::string& origin) const
	{
		auto event = std::make_shared<NetworkTrafficReceivedEvent>();
		event->Message = message;
		event->Identifier = identifier;
		event->BytesReceived = bytesReceived;
		event->Origin = origin; // Origin of the event is the where the event was emitted from
		return event;
	}

	std::shared_ptr<gamelib::SceneChangedEvent> EventFactory::CreateLevelEvent(const int level) const
	{
		return std::make_shared<gamelib::SceneChangedEvent>(level);
	}

	std::shared_ptr<gamelib::UpdateAllGameObjectsEvent> EventFactory::CreateUpdateAllGameObjectsEvent() const
	{
		return std::make_shared<gamelib::UpdateAllGameObjectsEvent>();
	}

	std::shared_ptr<gamelib::UpdateProcessesEvent> EventFactory::CreateUpdateProcessesEvent() const
	{
		return std::make_shared<gamelib::UpdateProcessesEvent>();
	}

	std::shared_ptr<gamenetlib::StartNetworkLevelEvent> EventFactory::CreateStartNetworkLevelEvent(const int level) const
	{
		return std::make_shared<StartNetworkLevelEvent>(level);
	}

	std::shared_ptr<gamelib::Event> EventFactory::CreateNetworkPlayerJoinedEvent(const NetworkPlayer& player) const
	{
		return std::make_shared<gamenetlib::NetworkPlayerJoinedEvent>(player);
	}

	std::shared_ptr<gamelib::ControllerMoveEvent> EventFactory::CreateControllerMoveEvent(gamelib::Direction direction,
	                                                                                      gamelib::ControllerMoveEvent::KeyState keyState) const
	{
		return std::make_shared<gamelib::ControllerMoveEvent>(direction, keyState);
	}

	std::shared_ptr<gamelib::Event> EventFactory::CreateSubscriberHandledEvent(gamelib::IEventSubscriber* value,
	                                                                  const std::shared_ptr<gamelib::Event>& event,
	                                                                  unsigned long deltaMs) const
	{
		return std::make_shared<gamelib::SubscriberHandledEvent>(value, event, deltaMs);
	}

	std::shared_ptr<ReliableUdpPacketReceivedEvent> EventFactory::CreateReliableUdpPacketReceived(std::shared_ptr<Message> message) const
	{
		return std::make_shared<ReliableUdpPacketReceivedEvent>(message);
	}

	std::shared_ptr<ReliableUdpCheckSumFailedEvent> EventFactory::CreateReliableUdpCheckSumFailedEvent(std::shared_ptr<Message> failedMessage) const
	{
		return std::make_shared<ReliableUdpCheckSumFailedEvent>(failedMessage);
	}

	std::shared_ptr<ReliableUdpPacketLossDetectedEvent> EventFactory::CreateReliableUdpPacketLossDetectedEvent(const std::shared_ptr<Message>& messageBundle) const
	{
		return std::make_shared<ReliableUdpPacketLossDetectedEvent>(messageBundle);
	}

	std::shared_ptr<ReliableUdpAckPacketEvent> EventFactory::CreateReliableUdpAckPacketEvent(
		const std::shared_ptr<Message>& message, bool isSent) const
	{
		return std::make_shared<ReliableUdpAckPacketEvent>(message, isSent);
	}

	std::shared_ptr<ReliableUdpPacketRttCalculatedEvent> EventFactory::CreateReliableUdpPacketRttCalculatedEvent(
		const std::shared_ptr<Message>& message, Rtt rtt) const
	{
		return std::make_shared<ReliableUdpPacketRttCalculatedEvent>(message, rtt);
	}

	std::shared_ptr<gamelib::AddGameObjectToCurrentSceneEvent> EventFactory::CreateAddToSceneEvent(const std::shared_ptr<gamelib::GameObject> & obj)
	{
		return std::make_shared<gamelib::AddGameObjectToCurrentSceneEvent>(obj);
	}

	std::shared_ptr<gamelib::SceneChangedEvent> EventFactory::CreateSceneChangedEventEvent(const int newLevel) const
	{
		return std::make_shared<gamelib::SceneChangedEvent>(newLevel);
	}
}

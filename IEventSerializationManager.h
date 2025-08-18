#pragma once
#include <string>
#include <memory>

namespace gamelib
{
	class Event;
	class ControllerMoveEvent;
	class PlayerMovedEvent;
}

namespace gamenetlib
{
	class StartNetworkLevelEvent;

	class IEventSerializationManager	 
	{
	public:
		virtual ~IEventSerializationManager() = default;
		virtual std::shared_ptr<gamelib::PlayerMovedEvent> DeserializePlayerMovedEvent(std::string serializedMessage) = 0;
		virtual std::string SerializePlayerMovedEvent(std::shared_ptr<gamelib::PlayerMovedEvent> object, std::string target) = 0;
		virtual std::string CreateRequestPlayerDetailsMessage() = 0;
		virtual std::string CreatePongMessage() = 0;
		virtual std::string CreatePingMessage() = 0;
		virtual std::string SerializeControllerMoveEvent(std::shared_ptr<gamelib::ControllerMoveEvent> object, std::string target) = 0;
		virtual std::string CreateUnknownEventMessage(std::shared_ptr<gamelib::Event> evt, std::string target) = 0;
		virtual std::string SerializeStartNetworkLevelEvent(std::shared_ptr<StartNetworkLevelEvent> evt, std::string target) = 0;
		virtual std::shared_ptr<StartNetworkLevelEvent> DeserializeStartNetworkLevel(std::string serializedMessage) = 0;
		virtual std::string CreateRequestPlayerDetailsMessageResponse(const std::string& target) = 0;
	};
}


#pragma once
#include "json11.h"

#include "IEventSerializationManager.h"

namespace gamenetlib
{
	class JsonEventSerializationManager final :  public IEventSerializationManager
	{
	public:
		std::shared_ptr<gamelib::PlayerMovedEvent> DeserializePlayerMovedEvent(std::string serializedMessage) override;
		std::shared_ptr<StartNetworkLevelEvent> DeserializeStartNetworkLevel(std::string serializedMessage) override;
		std::string SerializeControllerMoveEvent(std::shared_ptr<gamelib::ControllerMoveEvent> object, std::string target) override;
		std::string SerializePlayerMovedEvent(std::shared_ptr<gamelib::PlayerMovedEvent> object, std::string target) override;
		std::string CreateRequestPlayerDetailsMessage() override;
		std::string SerializeStartNetworkLevelEvent(std::shared_ptr<StartNetworkLevelEvent> evt, std::string target) override;
		std::string CreatePongMessage() override;
		std::string CreatePingMessage() override;
		std::string CreateUnknownEventMessage(std::shared_ptr<gamelib::Event> evt, std::string target) override;
		std::string CreateRequestPlayerDetailsMessageResponse(const std::string& target) override;
	};
}


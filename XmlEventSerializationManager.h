#pragma once
#include "JsonEventSerializationManager.h"
#include "IEventSerializationManager.h"

namespace gamenetlib
{
	class StartNetworkLevelEvent;

	class XmlEventSerializationManager : public IEventSerializationManager
	{
	public:
		 std::shared_ptr<gamelib::PlayerMovedEvent> DeserializePlayerMovedEvent(std::string serializedMessage) override;
		 std::string SerializePlayerMovedEvent(std::shared_ptr<gamelib::PlayerMovedEvent> object, std::string target) override;
		 std::string CreateRequestPlayerDetailsMessage() override;
		 std::string CreatePongMessage() override;
		 static std::string CreateDummyXml();
		 std::string CreatePingMessage() override;
		 std::string SerializeControllerMoveEvent(std::shared_ptr<gamelib::ControllerMoveEvent> object, std::string target) override;
		 std::string CreateUnknownEventMessage(std::shared_ptr<gamelib::Event> evt, std::string target) override;
		 std::string SerializeStartNetworkLevelEvent(std::shared_ptr<gamenetlib::StartNetworkLevelEvent> evt, std::string target) override;
		 std::shared_ptr<gamenetlib::StartNetworkLevelEvent> DeserializeStartNetworkLevel(std::string serializedMessage) override;
		 std::string CreateRequestPlayerDetailsMessageResponse(const std::string& target) override;

	 private:
			JsonEventSerializationManager json;
	};
}


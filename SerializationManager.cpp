

#include "SerializationManager.h"

#include <cassert>
#include <events/ControllerMoveEvent.h>
#include <events/PlayerMovedEvent.h>
#include <utils/Utils.h>

#include "StartNetworkLevelEvent.h"
#include "XmlEventSerializationManager.h"

namespace gamenetlib
{
	SerializationManager::SerializationManager(const gamenetlib::Encoding desiredEncoding = Encoding::json)
	{
		Encoding = desiredEncoding;
		Initialize();
	}

	void SerializationManager::Initialize()
	{
		assert(Encoding == gamenetlib::Encoding::json 
			|| Encoding == gamenetlib::Encoding::xml 
			|| Encoding == gamenetlib::Encoding::bit_packed 
			|| Encoding == gamenetlib::Encoding::none);

		switch (Encoding)
		{
			case Encoding::json: 
				eventSerialization = std::make_shared<gamenetlib::JsonEventSerializationManager>();
				break;
			case Encoding::xml:
				eventSerialization = std::make_shared<gamenetlib::XmlEventSerializationManager>();
				break;
			case Encoding::bit_packed:
				eventSerialization = std::make_shared<gamenetlib::BitPackedEventSerializationManager>();
				break;
			case Encoding::none:
				throw std::exception(std::string("Unknown serialization format").c_str());
		}
	}

	MessageHeader SerializationManager::GetMessageHeader(const std::string& serializedMessage)
	{
		std::string jsonParseError;
		const auto jsonObject = json11::Json::parse(serializedMessage, jsonParseError);

		if (!jsonParseError.empty())
		{
			throw std::runtime_error("Failed to parse JSON: " + jsonParseError);
		}

		MessageHeader header
		{
			.MessageType = jsonObject["messageType"].string_value(),
			.MessageTarget = jsonObject["nickname"].string_value(),
		};

		return header;
	}

	std::string SerializationManager::SerializeEvent(const std::shared_ptr<gamelib::Event>& evt, const std::string& target) const
	{
		// TODO: Update the list of events that can be sent over the network

		if(evt->Id.PrimaryId == gamelib::PlayerMovedEventTypeEventId.PrimaryId) { return CreatePlayerMovedEventMessage(evt, target);}
		if(evt->Id.PrimaryId == gamelib::ControllerMoveEventId.PrimaryId) { return CreateControllerMoveEventMessage(evt, target);}
		if(evt->Id.PrimaryId == gamenetlib::StartNetworkLevelEventId.PrimaryId) { return CreateStartNetworkLevelMessage(evt, target);}

		return CreateUnknownEventMessage(evt, target);
		
	}

	std::shared_ptr<gamelib::Event> SerializationManager::Deserialize(const MessageHeader& messageHeader, const std::string&
	                                                         serializedMessage) const
	{
		std::shared_ptr<gamelib::Event> event = nullptr;

		// TODO: Update the list of messages (serialised events) that we can be consumed by the network clients, i.e., deserialized
		
		if(messageHeader.MessageType == gamenetlib::StartNetworkLevelEventId.Name)
		{
			return To<gamelib::Event>(eventSerialization->DeserializeStartNetworkLevel(serializedMessage));
		}

		if(messageHeader.MessageType == gamelib::PlayerMovedEventTypeEventId.Name)
		{
			return To<gamelib::Event>(eventSerialization->DeserializePlayerMovedEvent(serializedMessage));
		}
		
		return event;
	}

	std::string SerializationManager::CreatePlayerMovedEventMessage(const std::shared_ptr<gamelib::Event>& event, const std::string
	                                                                & target) const
	{
		const auto playerMovedEvent = gamelib::To<gamelib::PlayerMovedEvent>(event);
		return eventSerialization->SerializePlayerMovedEvent(playerMovedEvent, target);
	}

	std::string SerializationManager::CreateControllerMoveEventMessage(const std::shared_ptr<gamelib::Event>& evt, const std::string
	                                                                   & target) const
	{
		const auto controllerMoveEvent = gamelib::To<gamelib::ControllerMoveEvent>(evt);
		return eventSerialization->SerializeControllerMoveEvent(controllerMoveEvent, target);
	}

	std::string SerializationManager::CreateStartNetworkLevelMessage(const std::shared_ptr<gamelib::Event>& evt, const std::string
	                                                                 & target) const
	{
		const auto startNetworkLevelEvent = gamelib::To<gamenetlib::StartNetworkLevelEvent>(evt);
		return eventSerialization->SerializeStartNetworkLevelEvent(startNetworkLevelEvent, target);
	}
		
	std::string SerializationManager::CreateUnknownEventMessage(const std::shared_ptr<gamelib::Event>& evt, const std::string&
	                                                            target) const
	{
		return eventSerialization->CreateUnknownEventMessage(evt, target);
	}

	std::string SerializationManager::CreateRequestPlayerDetailsMessage() const
	{
		return eventSerialization->CreateRequestPlayerDetailsMessage();		
	}

	std::string SerializationManager::CreateRequestPlayerDetailsMessageResponse(const std::string& target) const
	{
		return eventSerialization->CreateRequestPlayerDetailsMessageResponse(target);		
	}

	std::string SerializationManager::CreatePongMessage() const
	{
		return eventSerialization->CreatePongMessage();
	}

	std::string SerializationManager::CreatePingMessage() const
	{
		return eventSerialization->CreatePingMessage();
	}

	std::string SerializationManager::UpdateTarget(const std::string& target, const std::string& serialisedMessage)
	{
		std::string jsonParseError;
		const auto jsonObject = json11::Json::parse(serialisedMessage, jsonParseError);

		if (!jsonParseError.empty())
		{
			throw std::runtime_error("Failed to parse JSON: " + jsonParseError);
		}

		json11::Json::object jsonObj = jsonObject.object_items();
		jsonObj["nickname"] = target;
		const json11::Json anotherJson = jsonObj;
		return anotherJson.dump();
	}
}

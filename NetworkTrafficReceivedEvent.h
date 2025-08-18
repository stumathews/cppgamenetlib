#pragma once
#include <string>

#include <cppgamelib/events/Event.h>
#include <cppgamelib/events/EventNumbers.h>

namespace gamenetlib
{
	const static gamelib::EventId NetworkTrafficReceivedEventId(gamelib::NetworkTrafficReceived, "NetworkTrafficReceived");
	class NetworkTrafficReceivedEvent final :  public gamelib::Event
	{
	public:
		NetworkTrafficReceivedEvent();
		std::string Identifier;
		std::string Message;
		int BytesReceived{};
		[[nodiscard]] const char* GetPayload() const { return Message.c_str();}
	};
}


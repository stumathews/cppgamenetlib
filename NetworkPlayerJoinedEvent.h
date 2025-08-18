#pragma once
#include <cppgamelib/events/Event.h>
#include <cppgamelib/events/EventNumbers.h>
#include "NetworkPlayer.h"

namespace gamenetlib
{
	const static gamelib::EventId NetworkPlayerJoinedEventId(gamelib::NetworkPlayerJoined, "NetworkPlayerJoinedEvent");
	class NetworkPlayerJoinedEvent final : public gamelib::Event
	{	
	public:
		explicit NetworkPlayerJoinedEvent(const NetworkPlayer& player) : Event(NetworkPlayerJoinedEventId)
		{
			this->Player = player;
		}

		NetworkPlayer Player;
	};
}

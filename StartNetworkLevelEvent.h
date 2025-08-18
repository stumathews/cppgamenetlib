#pragma once
#include <cppgamelib/events/Event.h>

namespace gamenetlib
{
	const static gamelib::EventId StartNetworkLevelEventId(gamelib::StartNetworkLevel, "StartNetworkLevel");
	class StartNetworkLevelEvent final : public gamelib::Event
	{
	public:

		explicit StartNetworkLevelEvent(const int level) : Event(StartNetworkLevelEventId)
		{
			this->Level = level;
		}
	
		int Level;

	};
}

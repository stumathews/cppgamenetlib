#ifndef RELIABLE_UDP_CHECKSUM_FAILED_EVENT_H
#pragma once
#include <cppgamelib/events/Event.h>
#include <cppgamelib/events/EventNumbers.h>

namespace gamenetlib
{
	class Message;
	const static gamelib::EventId ReliableUdpCheckSumFailedEventId(gamelib::ReliableUdpCheckSumFailed, "ReliableUdpCheckSumFailed");
	class ReliableUdpCheckSumFailedEvent : public gamelib::Event
	{
	public:
		explicit ReliableUdpCheckSumFailedEvent(std::shared_ptr<Message> failedMessage)
			: Event(ReliableUdpCheckSumFailedEventId), failedMessage(std::move(failedMessage))
		{
		}

		std::shared_ptr<Message> failedMessage;
	};
}

#endif RELIABLE_UDP_CHECKSUM_FAILED_EVENT_H
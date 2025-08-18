
#ifndef RELIABLE_UDP_PACKET_LOSS_DETECTED_EVENT_H
#define RELIABLE_UDP_PACKET_LOSS_DETECTED_EVENT_H

#include <cppgamelib/events/Event.h>
#include <cppgamelib/events/EventNumbers.h>

namespace gamenetlib
{
	class Message;
	const static gamelib::EventId ReliableUdpPacketLossDetectedEventId(gamelib::ReliableUdpPacketLossDetected, "ReliableUdpPacketLossDetected");
	class ReliableUdpPacketLossDetectedEvent : public gamelib::Event
	{
	public:
		explicit ReliableUdpPacketLossDetectedEvent(std::shared_ptr<Message> messageBundle)
			: Event(ReliableUdpPacketLossDetectedEventId), messageBundle(std::move(messageBundle))
		{
		}

		std::shared_ptr<Message> messageBundle;
	};
}

#endif

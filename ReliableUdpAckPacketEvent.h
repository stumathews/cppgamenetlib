#ifndef RELIABLE_ACK_PACKET_EVENT_H
#define RELIABLE_ACK_PACKET_EVENT_H
#pragma once
#include <cppgamelib/events/Event.h>
#include <cppgamelib/events/EventNumbers.h>

namespace gamenetlib
{
	class Message;
	const static gamelib::EventId ReliableUdpAckPacketEventId(gamelib::ReliableUdpAckPacket, "ReliableUdpAckPacketEvent");
	class ReliableUdpAckPacketEvent final : public gamelib::Event
	{
	public:

		explicit ReliableUdpAckPacketEvent(const std::shared_ptr<Message> message, const bool sent) : Event(ReliableUdpAckPacketEventId), ReceivedMessage(message), Sent(sent)
		{
		}

		const std::shared_ptr<Message> ReceivedMessage;
		bool Sent; // if not sent, its received
	};
}

#endif

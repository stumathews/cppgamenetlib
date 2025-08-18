#pragma once
#ifndef RELIABLE_UDP_PACKET_RTT_EVENT_H
#define RELIABLE_UDP_PACKET_RTT_EVENT_H
#include <memory>

#include <cppgamelib/events/Event.h>
#include <cppgamelib/events/EventId.h>
#include <cppgamelib/events/EventNumbers.h>
#include "Rtt.h"

namespace gamenetlib
{
	class Message;
	const static gamelib::EventId ReliableUdpPacketRttCalculatedEventId(gamelib::ReliableUdpPacketRttCalculated, "ReliableUdpPacketRttCalculatedEvent");

	class ReliableUdpPacketRttCalculatedEvent final : public gamelib::Event
	{
	public:

		explicit ReliableUdpPacketRttCalculatedEvent(std::shared_ptr<Message> message, const Rtt rtt)
		: Event(ReliableUdpPacketRttCalculatedEventId), ReceivedMessage(std::move(message)), Rtt(rtt)
		{
		}

		const std::shared_ptr<Message> ReceivedMessage;
		Rtt Rtt;
	};
}

#endif
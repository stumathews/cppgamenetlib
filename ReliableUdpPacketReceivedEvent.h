#ifndef RELIABLE_UDP_PACKET_RECEIVED_EVENT_H
#define RELIABLE_UDP_PACKET_RECEIVED_EVENT_H
#pragma once
#include <cppgamelib/events/Event.h>
#include <cppgamelib/events/EventNumbers.h>

namespace gamenetlib
{
	class Message;

	const static gamelib::EventId ReliableUdpPacketReceivedEventId(gamelib::ReliableUdpPacketReceived, "ReliableUdpPacketReceived");
	class ReliableUdpPacketReceivedEvent final : public gamelib::Event
	{
	public:

		explicit ReliableUdpPacketReceivedEvent(const std::shared_ptr<Message> message) :
		gamelib::Event(ReliableUdpPacketReceivedEventId), ReceivedMessage(message)
		{
		}

		const std::shared_ptr<Message> ReceivedMessage;
	};

}

#endif RELIABLE_UDP_PACKET_RECEIVED_EVENT_H
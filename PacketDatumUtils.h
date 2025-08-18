#pragma once
#ifndef PACKET_DATUM_UTILS
#define PACKET_DATUM_UTILS
#include <vector>

#include "PacketDatum.h"
#include "ReliableUdpGameServerConnection.h"
#include "Rtt.h"
#include "RingBuffer.h"
#include "Statistics.h"
#include "Message.h"

namespace gamenetlib
{
	struct PacketDatumUtils
	{
		static std::vector<unsigned long> GetLastKRtts(const int k, RingBuffer<PacketDatum>& receiveBuffer)
		{
			std::vector<unsigned long> p {0}; // dataPoints
			const auto n = receiveBuffer.GetLastAddedSequence(); // number of dataPoints
			for(int i = 1 ; i <= k ; i++)
			{
				const auto* datum = receiveBuffer.Get(n-k+i);
				if( datum == nullptr)
				{
					p.push_back(0);
				}
				else
				{
					p.push_back(datum->RttMs);
				}
			}
			return p;
		}

		static Rtt CalculateRttStats(const Message& message, RingBuffer<PacketDatum>& sendBuffer)
		{
			const auto* datum = sendBuffer.Get(message.Header.Sequence);
			const auto sma3 = gamelib::Statistics::SMA(3, GetLastKRtts(3, sendBuffer));
			return Rtt(datum->RttMs, sma3);
		}
	};

};

#endif
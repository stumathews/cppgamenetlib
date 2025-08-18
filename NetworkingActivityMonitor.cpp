#include <cppgamelib/processes/ProcessManager.h>
#include <cppgamelib/file/Logger.h>

#include "NetworkingActivityMonitor.h"
#include "EventFactory.h"
#include "NetworkPlayerJoinedEvent.h"
#include "NetworkTrafficReceivedEvent.h"
#include "ReliableUdpAckPacketEvent.h"
#include "ReliableUdpCheckSumFailedEvent.h"
#include "ReliableUdpPacketLossDetectedEvent.h"
#include "ReliableUdpPacketReceivedEvent.h"
#include "ReliableUdpPacketRttCalculatedEvent.h"


#include "BitFiddler.h"

#include "GameClient.h"

using namespace std;

namespace gamenetlib
{
	class EventManager;

	NetworkingActivityMonitor::NetworkingActivityMonitor(gamelib::ProcessManager& processManager, gamelib::EventManager& eventManager,
	                                                     std::shared_ptr<IElapsedTimeProvider> elapsedTimeProvider, const bool verbose = false):
		processManager(processManager), eventManager(eventManager), verbose(verbose),
		elapsedTimeProvider(std::move(elapsedTimeProvider))
	{
		// Prepare a file that will receive the statistics every saveIntervalMs
		statisticsFile = std::make_shared<gamelib::TextFile>("statistics.txt");
	}

	void NetworkingActivityMonitor::Initialise()
	{
		constexpr auto saveIntervalMs = 1000; // every second

		// Subscribe to Network Events
		eventManager.SubscribeToEvent(gamenetlib::NetworkTrafficReceivedEventId, this);
		eventManager.SubscribeToEvent(gamenetlib::ReliableUdpPacketReceivedEventId, this);
		eventManager.SubscribeToEvent(gamenetlib::ReliableUdpCheckSumFailedEventId, this);
		eventManager.SubscribeToEvent(gamenetlib::ReliableUdpPacketLossDetectedEventId, this);
		eventManager.SubscribeToEvent(gamenetlib::ReliableUdpAckPacketEventId, this);
		eventManager.SubscribeToEvent(gamenetlib::ReliableUdpPacketRttCalculatedEventId, this);

		// Schedule save operation to run periodically
		statisticsSampleInterval.SetFrequency(saveIntervalMs);

		InitialiseStatisticsFile();

		// Schedule sampler to run as a background process
		ScheduleSaveStatistics();
	}

	// Handle events
	gamelib::ListOfEvents NetworkingActivityMonitor::HandleEvent(const std::shared_ptr<gamelib::Event>& evt, const unsigned long inDeltaMs)
	{
		if (evt->Id.PrimaryId == gamenetlib::NetworkPlayerJoinedEventId.PrimaryId) { OnNetworkPlayerJoinedEvent(evt); }
		if (evt->Id.PrimaryId == gamenetlib::ReliableUdpPacketReceivedEventId.PrimaryId) { OnReliableUdpPacketReceivedEvent(evt); }
		if (evt->Id.PrimaryId == gamenetlib::ReliableUdpCheckSumFailedEventId.PrimaryId) { OnReliableUdpCheckSumFailedEvent(evt); }
		if (evt->Id.PrimaryId == gamenetlib::ReliableUdpPacketLossDetectedEventId.PrimaryId) { OnReliableUdpPacketLossDetectedEvent(evt); }
		if (evt->Id.PrimaryId == gamenetlib::ReliableUdpAckPacketEventId.PrimaryId) { OnReliableUdpAckPacketEvent(evt); }
		if (evt->Id.PrimaryId == gamenetlib::ReliableUdpPacketRttCalculatedEventId.PrimaryId) { OnReliableUdpPacketRttCalculatedEvent(evt); }
		if (evt->Id.PrimaryId == gamenetlib::NetworkTrafficReceivedEventId.PrimaryId) { OnNetworkTrafficReceivedEvent(evt); }
		return {};
	}

	void NetworkingActivityMonitor::InitialiseStatisticsFile() const
	{
		// Setup statistics file header (first line)
		std::stringstream header;
		header << "TimeSecs" << "\t"
			<< "BytesReceived" << "\t"
			<< "CountPacketsLost" << "\t"
			<< "CountPacketsReceived" << "\t"
			<< "AverageLatencyMsSma3" << "\t"
			<< "CountAcks" << "\t"
			<< "VerificationFailedCount" << "\t"
			<< "CountAggregateMessagesReceived" << "\t"
			<< "SendingRateMs" << "\t"
			<< "SendingRatePs" << "\t"
			<< "RttMs"
			<< "\n";

		statisticsFile->Append(header.str(), false);
	}

	void NetworkingActivityMonitor::AppendStatsToFile(const int tSeconds) const
	{
		std::stringstream message;

		// Fetch statistics and add them to the log file (see header)
		message
			<< tSeconds << "\t"
			<< stats.BytesReceived << "\t"
			<< stats.CountPacketsLost << "\t"
			<< stats.CountPacketsReceived << "\t"
			<< stats.AverageLatencySMA3 << "\t"
			<< stats.CountAcks << "\t"
			<< stats.VerificationFailedCount << "\t"
			<< stats.CountAggregateMessagesReceived << "\t"
			<< stats.SendRateMs << "\t"
			<< stats.SendRatePs << "\t"
			<< stats.RttMs
			<< "\n";

		// Write to file
		statisticsFile->Append(message.str(), false);
	}

	void NetworkingActivityMonitor::SaveAndReset(const unsigned long deltaMs)
	{
		// Progress the interval
		statisticsSampleInterval.Update(deltaMs);

		// Will only write statistics to file if the statistics sampling interval has elapsed (ready)
		statisticsSampleInterval.DoIfReady([&]()
		{
			// Record the current second we are in since game was started, this will be the independent variable
			const auto tNowSecs = elapsedTimeProvider->GetElapsedTime();

			AppendStatsToFile(tNowSecs);

			// Reset statistics to zero.
			stats.Reset();
		});
	}

	void NetworkingActivityMonitor::ScheduleSaveStatistics()
	{
		// Schedule process to write statistics to file

		/*auto process = std::make_shared<gamelib::Action>([this](auto&& deltaMs)
		{
			SaveAndReset(deltaMs);
		}, false);*/

		//ScheduleProcess(process);
	}

	// ReSharper disable once CppPassValueParameterByConstReference
	void NetworkingActivityMonitor::ScheduleProcess(shared_ptr<gamelib::Process> process) const
	{
		// Add this process to the process manager - schedules process to receive updates
		processManager.AttachProcess(std::static_pointer_cast<gamelib::Process>(process));
	}

	std::string NetworkingActivityMonitor::GetSubscriberName()
	{
		return "NetworkStatistics";
	}
	
	void NetworkingActivityMonitor::OnNetworkPlayerJoinedEvent(const std::shared_ptr<gamelib::Event>& evt) const
	{
		if (verbose)
		{
			const auto joinEvent = gamelib::To<gamenetlib::NetworkPlayerJoinedEvent>(evt);
			stringstream message;
			message << joinEvent->Player.GetNickName() << " joined.";
			gamelib::Logger::Get()->LogThis(message.str());
		}
	}

	void NetworkingActivityMonitor::OnNetworkTrafficReceivedEvent(const std::shared_ptr<gamelib::Event>& evt)
	{
		const auto networkPlayerTrafficReceivedEvent = gamelib::To<gamenetlib::NetworkTrafficReceivedEvent>(evt);
		
		// how much data did we receive in this second?
		stats.BytesReceived += networkPlayerTrafficReceivedEvent->BytesReceived;

		if (verbose)
		{
			std::stringstream message;
			message
				<< networkPlayerTrafficReceivedEvent->BytesReceived << " bytes of data received from " << networkPlayerTrafficReceivedEvent->Identifier
				<< ". Message: \"" << networkPlayerTrafficReceivedEvent->Message << "\"";

			gamelib::Logger::Get()->LogThis(message.str());
		}
	}

	void NetworkingActivityMonitor::OnReliableUdpPacketReceivedEvent(const std::shared_ptr<gamelib::Event>& evt)
	{
		const auto rudpEvent = gamelib::To<gamenetlib::ReliableUdpPacketReceivedEvent>(evt);
		const auto rudpMessage = rudpEvent->ReceivedMessage;
		// write stats
		stats.CountPacketsReceived++;
		stats.CountAggregateMessagesReceived += rudpMessage->DataCount();

		if (verbose)
		{
			stringstream bundledSeqs;

			bundledSeqs << "(";
			for (int i = 0; i < rudpMessage->DataCount(); i++)
			{
				bundledSeqs << rudpMessage->Data()[i].Sequence;
				if (i < rudpMessage->DataCount() - 1)
				{
					bundledSeqs << ",";
				}
			}
			bundledSeqs << ")";

			std::stringstream message;
			message
				<< "Received " << rudpMessage->Header.Sequence << ". Playload: " << bundledSeqs.str()
				<< " Sender acks: "
				<< BitFiddler<uint32_t>::ToString(rudpMessage->Header.LastAckedSequence);

			gamelib::Logger::Get()->LogThis(message.str());
		}
	}

	void NetworkingActivityMonitor::OnReliableUdpCheckSumFailedEvent(const std::shared_ptr<gamelib::Event>& evt)
	{		
		// write stats
		stats.VerificationFailedCount++;

		if (verbose) 
		{
			const auto failedChecksumEvent = gamelib::To<ReliableUdpCheckSumFailedEvent>(evt);
			const auto failedMessage = failedChecksumEvent->failedMessage;

			std::stringstream message;
			message << "Checksum failed for sequence " << failedMessage->Header.Sequence << ". Dropping packet.";
			gamelib::Logger::Get()->LogThis(message.str());
		}
	}

	void NetworkingActivityMonitor::OnReliableUdpPacketLossDetectedEvent(const std::shared_ptr<gamelib::Event>& evt)
	{
		// Track statistics
		stats.CountPacketsLost++;

		if (verbose)
		{
			const auto reliableUdpPacketLossDetectedEvent = To<ReliableUdpPacketLossDetectedEvent>(evt);
			const auto resendingMessage = reliableUdpPacketLossDetectedEvent->messageBundle;

			stringstream bundledSequences;
			std::stringstream message;

			for (int i = static_cast<int>(resendingMessage->Data().size()) - 1; i >= 0; i--)
			{
				bundledSequences << resendingMessage->Data()[i].Sequence;
				if (i < resendingMessage->DataCount() - 1)
				{
					bundledSequences << ",";
				}
			}

			message << "Packet loss detected. Sequences " << bundledSequences.str() << " were not acknowledged by receiver and will be resent with sending sequence "
				<< resendingMessage->Header.Sequence;

			gamelib::Logger::Get()->LogThis(message.str());
		}
	}

	void NetworkingActivityMonitor::OnReliableUdpAckPacketEvent(const std::shared_ptr<gamelib::Event>& evt)
	{
		// how many acknowledgements did we receive?
		stats.CountAcks++;

		if (verbose)
		{
			const auto reliableUdpPacketLossDetectedEvent = To<ReliableUdpAckPacketEvent>(evt);
			const auto ackMessage = reliableUdpPacketLossDetectedEvent->ReceivedMessage;

			std::stringstream message;

			message << "Acknowledgement " << (reliableUdpPacketLossDetectedEvent->Sent ? "sent: " : " received: ") << ackMessage->Header.Sequence;

			gamelib::Logger::Get()->LogThis(message.str());
		}
	}

	void NetworkingActivityMonitor::OnReliableUdpPacketRttCalculatedEvent(const std::shared_ptr<gamelib::Event>& evt)
	{
		const auto rttEvent = To<ReliableUdpPacketRttCalculatedEvent>(evt);

		// The last latency recorded is a smooth moving average considering last 3 packets
		stats.AverageLatencySMA3 = rttEvent->Rtt.Sma3;
		stats.RttMs = static_cast<int>(rttEvent->Rtt.rtt);
	}

	void NetworkingActivityMonitor::SetSendRateMs(const int ms)
	{
		stats.SendRateMs = ms;
	}
	void NetworkingActivityMonitor::SetSendRatePs(const int sec)
	{
		stats.SendRatePs = sec;
	}
}
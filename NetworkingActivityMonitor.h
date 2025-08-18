#pragma once
#ifndef NETWORKING_ACTIVITY_MONITOR_H
#define NETWORKING_ACTIVITY_MONITOR_H

#include <cppgamelib/events/EventManager.h>
#include <cppgamelib/objects/GameObject.h>
#include <cppgamelib/time/PeriodicTimer.h>
#include <cppgamelib/file/TextFile.h>

#include "IElapsedTimeProvider.h"
#include "NetworkingStatistics.h"


namespace gamenetlib
{	
	class NetworkingActivityMonitor : public gamelib::EventSubscriber
	{
	public:
	
		NetworkingActivityMonitor(gamelib::ProcessManager& processManager, gamelib::EventManager& eventManager,
		                          std::shared_ptr<IElapsedTimeProvider> elapsedTimeProvider, bool verbose);
		void ScheduleSaveStatistics();
		void ScheduleProcess(std::shared_ptr<gamelib::Process> process) const;
		void InitialiseStatisticsFile() const;
		void AppendStatsToFile(int tSeconds) const;
		void SaveAndReset(unsigned long deltaMs);
		void Initialise();
		gamelib::ListOfEvents HandleEvent(const std::shared_ptr<gamelib::Event>& evt, const unsigned long inDeltaMs) override;
		std::string GetSubscriberName() override;
		void SetSendRateMs(int ms);
		void SetSendRatePs(int sec);
				
	private:

		NetworkingStatistics stats;
		std::shared_ptr<gamelib::TextFile> statisticsFile;
		gamelib::PeriodicTimer statisticsSampleInterval;
		gamelib::ProcessManager& processManager;
		gamelib::EventManager& eventManager;
		bool verbose;
		std::shared_ptr<IElapsedTimeProvider> elapsedTimeProvider;

		void OnReliableUdpAckPacketEvent(const std::shared_ptr<gamelib::Event>& evt);
		void OnReliableUdpPacketRttCalculatedEvent(const std::shared_ptr<gamelib::Event>& evt);
		void OnNetworkTrafficReceivedEvent(const std::shared_ptr<gamelib::Event>& evt);
		void OnReliableUdpPacketReceivedEvent(const std::shared_ptr<gamelib::Event>& evt);
		void OnReliableUdpCheckSumFailedEvent(const std::shared_ptr<gamelib::Event>& evt);
		void OnReliableUdpPacketLossDetectedEvent(const std::shared_ptr<gamelib::Event>& evt);
		void OnNetworkPlayerJoinedEvent(const std::shared_ptr<gamelib::Event>& evt) const;
	};
}

#endif
#pragma once
#ifndef NETWORKSOCKETFACTORY_H
#define NETWORKSOCKETFACTORY_H

#include <memory>

#include "TcpNetworkSocket.h"
#include "UdpConnectedNetworkSocket.h"

namespace gamenetlib
{
	// Creates connection to server for clients
	class GameClientConnectionFactory
	{
	public:

		static std::shared_ptr<IConnectedNetworkSocket> Create(const bool isTcp)
		{
			auto networkSocket =  
				isTcp
					? std::dynamic_pointer_cast<IConnectedNetworkSocket>(std::make_shared<TcpNetworkSocket>(TcpNetworkSocket()))
					: std::dynamic_pointer_cast<IConnectedNetworkSocket>(
						std::make_shared<UdpConnectedNetworkSocket>(UdpConnectedNetworkSocket()));

			return networkSocket;
		}		
	};
}
#endif


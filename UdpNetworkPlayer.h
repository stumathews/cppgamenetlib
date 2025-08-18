#pragma once

#include <string>

#include "NetworkPlayer.h"
#include "PeerInfo.h"


namespace gamenetlib
{
	class UdpNetworkPlayer : public NetworkPlayer
	{
	public:
		UdpNetworkPlayer(const PeerInfo peerInfo, const std::string& nickName)
		{
			this->PeerInfo = peerInfo;
			SetNickName(nickName);
		}

		PeerInfo PeerInfo;		
	};
}

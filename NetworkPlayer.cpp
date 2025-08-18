#include "NetworkPlayer.h"

namespace gamenetlib
{
	void NetworkPlayer::SetNickName(const std::string& nick)
	{
		nickName = nick;
	}

	std::string NetworkPlayer::GetNickName()
	{
		return nickName;
	}
}
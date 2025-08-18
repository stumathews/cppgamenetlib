#pragma once
#include <string>
#include <vector>

#include "Fish.h"
#ifndef PONG_MESSAGE_H

namespace gamenetlib
{

	class PongMessage
	{
	public:
		PongMessage() { fish = gamenetlib::Fish("Neemo", "mathews"); }
		const char* Type;
		bool isHappy;
		int eventType;
		std::vector<std::string> names;
		std::vector<int> ages;
		gamenetlib::Fish fish;
	};
}

#endif
#pragma once
#ifndef FISH_H
#define FISH_H

namespace gamenetlib
{
	class Fish
	{
	public:
		Fish(): Name("NoName"), Surname("NoSurname"){}
		Fish(std::string name, std::string surname): Name(std::move(name)), Surname(std::move(surname)) {  }
		std::string Name;
		std::string Surname;
	};
}

#endif
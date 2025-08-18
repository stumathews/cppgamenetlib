#pragma once

namespace gamenetlib
{
	class IElapsedTimeProvider  // NOLINT(cppcoreguidelines-special-member-functions)
	{
	public:
		virtual ~IElapsedTimeProvider() = default;
		virtual int GetElapsedTime() = 0;
	};
}
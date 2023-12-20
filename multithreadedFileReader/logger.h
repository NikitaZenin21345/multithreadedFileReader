#pragma once
#include <syncstream>

class logger final
{
	std::mutex mutex;
	logger() = default;
public:
	static logger& get_logger()
	{
		static logger logger_;
		return logger_;
	}
	void log_event(std::ostream& stream, const std::string& event)
	{
		std::lock_guard lock(mutex);
		stream << event << std::endl;
	}
	logger(const logger&) = delete;
	logger& operator=(const logger&) = delete;
};

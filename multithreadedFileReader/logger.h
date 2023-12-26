#pragma once
#include <syncstream>
#include "parser.h"

class logger final
{
	enum class log_strategy { log, no_log };
	std::mutex mutex;
	std::string log_file_name;
	std::ofstream log_file;
	log_strategy strategy = log_strategy::no_log;

	void settings_output(const tokens& parameters)
	{
		if(parameters.getTokens(0) == "log")
		{
			strategy = log_strategy::log;
		}
		else if(parameters.getTokens(0) == "no_log")
		{
			strategy = log_strategy::no_log;
		}
	}

	void settings_log_strategy(const tokens& parameters)
	{
		log_file_name = parameters.getTokens(1);
	}
	logger() = default;
public:

	static logger& get_logger()
	{
		static logger logger_;
		return logger_;
	}

	void settings_logger(std::istream& file)
	{
		text_file_separated_by_simple_delimiters_parser parser;
		std::lock_guard lock(mutex);
		auto tokens = parser.get_parsed(file);
		settings_output(tokens);
		settings_log_strategy(tokens);
		log_file = std::ofstream(log_file_name);
		if (!log_file.is_open())
		{
			throw;
		}
	}

	void log_event( const std::string& event)
	{
		std::lock_guard lock(mutex);
		if(strategy == log_strategy::log)
		{
			log_file << event << std::endl;
		}
	}

	logger(const logger&) = delete;
	logger& operator=(const logger&) = delete;
	~logger()
	{
		std::lock_guard lock(mutex);
		log_file.close();
	}
};

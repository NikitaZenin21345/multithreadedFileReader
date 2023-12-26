#pragma once
#include <string>
#include <thread>
#include <filesystem>
#include <iostream>
#include "parallel_file_accumulate.h"
namespace impl
{
	template<typename Number>
	Number do_parallel_accumulate_file_range(std::vector<std::string>::const_iterator begin_files, std::vector<std::string>::const_iterator end_files,
		const std::string& logger_settings_file_name,const size_t thread_count)
	{
		std::ifstream logger_settings_file(logger_settings_file_name);
		if (!logger_settings_file.is_open()) { return 1; }
		logger::get_logger().settings_logger(logger_settings_file);

		return parallel_file_accumulate<Number>(begin_files, end_files, thread_count);
	}
}


void execute(const std::string& folder_name, const std::string& template_file_name, const std::string& logger_settings_file_name, const size_t thread_count);

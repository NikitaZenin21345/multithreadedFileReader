#pragma once
#include <thread>
#include <vector>
#include <future>
#include <algorithm>
#include <fstream>
#include <numeric>
#include <functional>
#include <filesystem>
#include <iostream>
#include "logger.h"
#include "operation_deserializer.h"

template< typename Number, typename Str_to_number_convertor = impl::default_str_to_number_convertor<Number>>
	requires std::is_default_constructible_v<Str_to_number_convertor>
class accumulate_block final
{
	[[nodiscard]] std::string get_log_message_from_token(const tokens& tokens_list, Number tmp_result) const
	{
		return tokens_list.getTokens(0) + " " + tokens_list.getTokens(1) + " " + tokens_list.getTokens(2) + "| Result = " + std::to_string(tmp_result);
	}

	[[nodiscard]] std::string get_error_message(const std::string& file_name_template, const std::exception& error) const
	{
		return "Error in file " + file_name_template + ": " + error.what();
	}

public:
	Number operator()(const size_t start, const size_t end, const std::vector<std::string>::const_iterator& begin_files)
	{
		text_file_separated_by_simple_delimiters_parser parser;
		Number tmp_result = 0;
		for (size_t index = start; index < end; ++index)
		{
			std::string file_name = *(begin_files + index);
			std::ifstream file(file_name);
			if (!file.is_open()){continue;}
			try
			{
				auto tokens_list = parser.get_parsed(file);
				tmp_result += get_operation_result<Number, Str_to_number_convertor>(tokens_list);
				logger::get_logger().log_event(get_log_message_from_token(tokens_list, tmp_result));
			}
			catch (std::exception& error)
			{
				logger::get_logger().log_event(get_error_message(file_name, error));
				continue;
			}
			file.close();
		}
		return tmp_result;
	}
};

template< typename Number>
Number parallel_file_accumulate(const std::vector<std::string>::const_iterator& begin_files,const std::vector<std::string>::const_iterator& end_files, size_t num_threads)
{
	if (num_threads > std::thread::hardware_concurrency()) { num_threads = std::thread::hardware_concurrency(); }
	const size_t block_size = std::distance(begin_files, end_files) / num_threads;
	std::vector<std::future<Number>> tasks;
	for (size_t index = 0; index < num_threads; ++index)
	{
		size_t end_position = (index + 1) * block_size;
		if (index == num_threads - 1)
		{
			end_position = std::distance(begin_files, end_files);
		}
		tasks.push_back(std::async(accumulate_block<Number>(), index * block_size, end_position, std::ref(begin_files)));
	}
	Number result = 0;
	for(auto& task: tasks)
	{
		result += task.get();
	}
	return result;
}
#pragma once
#include <thread>
#include <vector>
#include <algorithm>
#include <fstream>
#include <numeric>
#include <functional>
#include <filesystem>
#include <iostream>
#include "deserializer.h"
#include "logger.h"

enum class log_strategy { log, no_log };

template< typename Number, typename Str_to_number_convertor = impl::default_str_to_number_convertor<Number>>
	requires std::is_default_constructible_v<Str_to_number_convertor>
class accumulate_block final
{

	[[nodiscard]] std::string get_log_message_from_token(const tokens& tokens_list, Number tmp_result) const
	{
		return tokens_list.getTokens(0) + " " + tokens_list.getTokens(1) + " " + tokens_list.getTokens(2) + " = " + std::to_string(tmp_result);
	}

	[[nodiscard]] std::string get_error_message(const std::string& file_name_template, const std::exception& error,const size_t index) const
	{
		return "Error in file " + file_name_template + std::to_string(index) + ": " + error.what();
	}

public:
	void operator()(const size_t start, const size_t end, const std::string& catalog_name, const std::string& file_name_template, Number& result,
		std::ostream& log_stream = std::cout, log_strategy log_strategy = log_strategy::no_log)
	{
		text_file_parser parser;
		Number tmp_result = 0;
		for (size_t index = start; index < end; ++index)
		{
			std::string file_name = catalog_name + file_name_template + std::to_string(index) + ".txt";
			std::ifstream file(file_name);
			if (!file.is_open()){continue;}
			try
			{
				auto tokens_list = parser.get_parsed(file);
				tmp_result += get_operation_result<Number, Str_to_number_convertor>(tokens_list);
				if (log_strategy == log_strategy::log)
				{
					logger::get_logger().log_event(log_stream, get_log_message_from_token(tokens_list, tmp_result));
				}
			}catch (std::exception& error)
			{
				if(log_strategy == log_strategy::log)
				{
					logger::get_logger().log_event(log_stream, get_error_message(file_name_template, error, index));
					continue;
				}
				std::cerr << error.what() << std::endl;
			}
		}
		result = tmp_result;
	}
};

template< typename Number>
Number parallel_file_accumulate(const size_t length, const size_t num_threads, const std::string& catalog_name,
	const std::string& file_name_template, log_strategy strategy = log_strategy::no_log, std::ostream& output = std::cout)
{
	const size_t block_size = length / num_threads;
	std::vector<Number> results(num_threads);
	std::vector<std::thread> threads(num_threads);
	for (size_t index = 0; index < (num_threads); ++index)
	{
		size_t end_position = (index + 1) * block_size;
		if (index == num_threads - 1)
		{
			end_position = length;
		}
		threads[index] = std::thread(accumulate_block<Number>(), index * block_size, end_position,
			catalog_name, file_name_template, std::ref(results[index]), std::ref(output), strategy);
	}
	std::ranges::for_each(threads.begin(), threads.end(), &std::thread::join);
	return std::accumulate(results.begin(), results.end(), static_cast<Number>(0));
}



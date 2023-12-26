#include "parallel_accumulate.h"

namespace 
{
	std::vector<std::string> get_files(const std::string& folder_name, const size_t file_count)
	{
		std::vector<std::string> files;
		files.reserve(file_count);
		for (size_t i = 0; i < file_count; ++i)
		{
			files.push_back(folder_name + std::to_string(i) + ".txt");
		}
		return files;
	}
}

void execute(const std::string& folder_name, const std::string& template_file_name,  const std::string& logger_settings_file_name, const size_t thread_count)
{
	try
	{
		const size_t length = std::distance(std::filesystem::directory_iterator(folder_name), std::filesystem::directory_iterator{});
		auto files = get_files(folder_name + template_file_name, length);
		std::cout << "Result = " << impl::do_parallel_accumulate_file_range<double>(files.cbegin(), files.cend(), logger_settings_file_name, thread_count);

	}
	catch (std::exception& error)
	{
		std::cerr << error.what() << std::endl;
	}
}

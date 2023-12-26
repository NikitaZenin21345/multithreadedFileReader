#include <thread>
#include <filesystem>
#include "parallel_accumulate.h"



int main()
{
	std::filesystem::path executable_path = std::filesystem::current_path();
	const std::string folder_name = executable_path.string() + "\\lists";
	const std::string template_file_name = "\\in_";
	const std::string logger_settings_file_name = "log_settings.txt";
	const size_t hardware_threads = std::thread::hardware_concurrency();
	execute(folder_name,template_file_name, logger_settings_file_name, hardware_threads);
	return 0;
}

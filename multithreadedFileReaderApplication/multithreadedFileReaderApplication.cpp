#include <thread>
#include <numeric>
#include <functional>
#include <filesystem>
#include <iostream>
#include <iterator>
#include "parallel_file_accumulate.h"


int main()
{
	const std::string folder_name = "C:\\Users\\nikze\\Desktop\\lists";
	const std::string file_name_template = "\\in_";
	const size_t length = std::distance(std::filesystem::directory_iterator(folder_name), std::filesystem::directory_iterator{});
	const size_t hardware_threads = std::thread::hardware_concurrency();
	std::cout << parallel_file_accumulate<double>(10, 1, folder_name, file_name_template, log_strategy::log);
	return 0;
}

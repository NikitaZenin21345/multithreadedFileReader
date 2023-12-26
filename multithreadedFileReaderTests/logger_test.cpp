#include "pch.h"
#include "parallel_file_accumulate.h"
#include "logger.h"
class LoggerTest : public ::testing::Test {
protected:
	virtual void SetUp(void)
	{
		std::ofstream log_output(log_output_name);
		if (!log_output.is_open())
		{
			std::cerr << "Error opening the file." << std::endl;
			FAIL();
		}
		log_output.close();
		log_config = std::ofstream(log_config_name);
		if (!log_config.is_open())
		{
			std::cerr << "Error opening the file." << std::endl;
			FAIL();
		}
		log_config << "log " + log_output_name << std::endl;
		log_config.close();
		std::ifstream input_log_config(log_config_name);
		if(!input_log_config.is_open())
		{
			std::cerr << "Error opening the file." << std::endl;
			FAIL();
		}
		logger::get_logger().settings_logger(input_log_config);
		input_log_config.close();
	}
	virtual void TearDown(void)
	{
		std::remove(log_output_name.c_str());
	}
	std::string log_output_name = "test_output_log.txt";
	std::string log_config_name = "test_log_settings.txt";
	std::ofstream log_config;
};

TEST_F(LoggerTest, AlotOfFiles)
{
	int file_count = 101;
	std::vector<std::string> files;
	std::string operation_name = "plus";
	for (int index = 0; index < file_count; ++index)
	{
		std::string filename = "TESTFile" + std::to_string(index) + ".txt";
		files.push_back(filename);
		std::ofstream newFile(filename);
		if (!newFile.is_open())
		{
			std::cerr << "Error opening the file." << std::endl;
			FAIL();
		}
		newFile << operation_name + " " + std::to_string(index) + " " + std::to_string(index) << std::endl;
		newFile.close();

	}
	auto result = accumulate_block<int>()(0, file_count,files.cbegin());
	int tmp_result = 0;
	std::ifstream out_log_stream(log_output_name);
	if (!out_log_stream.is_open())
	{
		std::cerr << "Error opening the file." << std::endl;
		FAIL();
	}
	for (int index = 0; index < file_count; ++index)
	{
		tmp_result += index + index;
		std::string filename = "TESTFile" + std::to_string(index) + ".txt";
		std::string log_message;
		std::getline(out_log_stream, log_message, '\n');
		EXPECT_EQ(log_message, operation_name + " " + std::to_string(index) + " " + std::to_string(index) + "| Result = " + std::to_string(tmp_result));
		if (std::remove(filename.c_str()) != 0)
		{
			std::cerr << "Error deleting the file." << std::endl;
			FAIL();
		}
	}
	EXPECT_EQ(result, file_count * (file_count - 1));
}

TEST_F(LoggerTest, ErrorInvalidCommandLog)
{
	std::string operation_name = "P_L_U_S";
	std::string filename = "TESTFile" + std::to_string(0) + ".txt";
	std::ofstream newFile(filename);
	if (!newFile.is_open())
	{
		std::cerr << "Error opening the file." << std::endl;
		FAIL();
	}
	newFile << operation_name + " " + std::to_string(4) + " " + std::to_string(7) << std::endl;
	newFile.close();
	std::vector<std::string> files;
	files.push_back(filename);
	auto result =  accumulate_block<int>()(0, 1, files.cbegin());
	std::string log_message;
	std::ifstream out_log_stream(log_output_name);
	if (!out_log_stream.is_open())
	{
		std::cerr << "Error opening the file." << std::endl;
		FAIL();
	}
	std::getline(out_log_stream, log_message, '\n');
	EXPECT_EQ(log_message, "Error in file TESTFile0.txt: invalid command");
	if (std::remove(filename.c_str()) != 0)
	{
		std::cerr << "Error deleting the file." << std::endl;
		FAIL();
	}
}

TEST_F(LoggerTest, ErrorInvalidNumberLog)
{
	std::string operation_name = "divide";
	std::vector<std::string> files;
	std::string filename = "TESTFile" + std::to_string(0) + ".txt";
	files.push_back(filename);
	std::ofstream newFile(filename);
	if (!newFile.is_open())
	{
		std::cerr << "Error opening the file." << std::endl;
		FAIL();
	}
	newFile << operation_name + " 398468759 0" << std::endl;
	newFile.close();
	auto result = accumulate_block<double>()(0, 1, files.cbegin());
	std::ifstream out_log_stream(log_output_name);
	if (!out_log_stream.is_open())
	{
		std::cerr << "Error opening the file." << std::endl;
		FAIL();
	}
	std::string log_message;
	std::getline(out_log_stream, log_message, '\n');
	EXPECT_EQ(log_message, "Error in file TESTFile0.txt: Division on zero");
	if (std::remove(filename.c_str()) != 0)
	{
		std::cerr << "Error deleting the file." << std::endl;
		FAIL();
	}
}
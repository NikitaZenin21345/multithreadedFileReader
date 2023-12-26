#include "pch.h"
#include "parallel_file_accumulate.h"

TEST(AccumulateBlock, OneFileTest)
{
	int first_number = 4;
	int second_number = 5;
	std::string filename = "TESTOneFile0.txt";
	std::ofstream  newFile(filename);
	if (!newFile.is_open())
	{
		std::cerr << "Error opening the file." << std::endl;
		FAIL();
	}
	newFile << "plus "+ std::to_string(first_number) + " " + std::to_string(second_number) << std::endl;
	std::vector<std::string> files = { "TESTOneFile0.txt" };
	const auto result = accumulate_block<int>()(0, 1, files.cbegin());
	EXPECT_EQ(result, first_number + second_number);
	newFile.close();
	if (std::remove(filename.c_str()) != 0) {
		std::cerr << "Error deleting the file." << std::endl;
		FAIL();
	}
}

TEST(AccumulateBlock, OneFileDevisionOnZero)
{
	const std::string filename = "TESTOneFile0.txt";
	std::ofstream  newFile(filename);
	if (!newFile.is_open())
	{
		std::cerr << "Error opening the file." << std::endl;
		FAIL();
	}
	newFile << "divide 33985.234 0" << std::endl;
	newFile.close();
	std::vector<std::string> files = { "TESTOneFile0.txt" };
	auto result = accumulate_block<double>()(0, 1, files.cbegin());
	EXPECT_EQ(result, 0);
	if (std::remove(filename.c_str()) != 0) {
		std::cerr << "Error deleting the file." << std::endl;
		FAIL();
	}
}
TEST(AccumulateBlock, AlotOfFiles)
{
	int file_count = 101;
	std::vector<std::string> files;
	for (int index = 0; index < file_count; ++index)
	{
		std::string filename = "TESTFile" + std::to_string(index) + ".txt";
		files.push_back(filename);
		std::ofstream  newFile(filename);
		
		if (!newFile.is_open())
		{
			std::cerr << "Error opening the file." << std::endl;
			FAIL();
		}
		newFile << "plus " + std::to_string(index) + " " + std::to_string(index) << std::endl;
		newFile.close();
	}
	auto result = accumulate_block<int>()(0, file_count,  files.cbegin());
	for (int index = 0; index < file_count; ++index)
	{
		std::string filename = "TESTFile" + std::to_string(index) + ".txt";
		if (std::remove(filename.c_str()) != 0) {
			std::cerr << "Error deleting the file." << std::endl;
			FAIL();
		}
	}
	EXPECT_EQ(result, file_count*(file_count - 1));
}
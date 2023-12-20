#include "pch.h"
#include "parallel_file_accumulate.h"
#include "logger.h"
//std::string folder_name = "C:\\Users\\nikze\\Desktop\\lists";
//std::string file_name_template = "\\in_";

TEST(AccumulateBlock, OneFileTest)
{
	std::string filename = "TESTOneFile0.txt";
	std::ofstream  newFile(filename);
	if (!newFile.is_open())
	{
		std::cerr << "Error opening the file." << std::endl;
		FAIL();
	}
	newFile << "plus 3 2" << std::endl;
	int result;
	accumulate_block<int>()(0, 1, "", "TESTOneFile", result);
	EXPECT_EQ(result, 5);
	newFile.close();
	if (std::remove(filename.c_str()) != 0) {
		std::cerr << "Error deleting the file." << std::endl;
		FAIL();
	}
}

TEST(AccumulateBlock, OneFileOperation)
{
	std::string filename = "TESTOneFile0.txt";
	std::ofstream  newFile(filename);
	int result;
	if (!newFile.is_open())
	{
		std::cerr << "Error opening the file." << std::endl;
		FAIL();
	}
	newFile << "divide 33985 0" << std::endl;
	accumulate_block<int>()(0, 1, "", "TESTOneFile", result);
	EXPECT_EQ(result, 0);
	newFile.clear();
	newFile << "multiply 33985 0" << std::endl;
	accumulate_block<int>()(0, 1, "", "TESTOneFile", result);
	EXPECT_EQ(result, 0);
	newFile.close();
	if (std::remove(filename.c_str()) != 0) {
		std::cerr << "Error deleting the file." << std::endl;
		FAIL();
	}
}
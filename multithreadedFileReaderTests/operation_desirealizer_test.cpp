#include "pch.h"
#include "operation_deserializer.h"

template<typename Number>
 Number do_operation_from_str(const std::string& operation,const Number first_number,const Number second_number)
{
	 text_file_separated_by_simple_delimiters_parser parser;
	 std::stringstream stream;
	 stream <<operation + " " + std::to_string(first_number) + " " + std::to_string(second_number);
	 auto tokens = parser.get_parsed(stream);
	 auto result = get_operation_result<Number>(tokens);
	 return result;
}

TEST(DesirealizeTest, PlusOperation)
{
	const std::string operation = "plus";
	const int first_number = 1;
	const int second_number = 2;
	EXPECT_EQ(first_number + second_number, do_operation_from_str(operation, first_number, second_number));
}

TEST(DesirealizeTest, MinusOperation)
{
	const std::string operation = "minus";
	const double first_number = 124.432;
	const double second_number = 2.231;
	EXPECT_EQ(first_number - second_number, do_operation_from_str(operation, first_number, second_number));
}

TEST(DesirealizeTest, DivideOperation)
{
	const std::string operation = "divide";
	double first_number = 1.134;
	double second_number = 2.456;
	EXPECT_EQ(first_number / second_number, do_operation_from_str(operation, first_number, second_number));
	first_number = 1.134;
	second_number = 0;
	EXPECT_THROW(do_operation_from_str(operation, first_number, second_number), invalid_number);
}

TEST(DesirealizeTest, MultiplyOperation)
{
	const std::string operation = "multiply";
	double first_number = 1.134;
	double second_number = 2.456;
	EXPECT_EQ(first_number * second_number, do_operation_from_str(operation, first_number, second_number));
	first_number = 1.134;
	second_number = 0;
	EXPECT_EQ(first_number * second_number, do_operation_from_str(operation, first_number, second_number));
}

TEST(DesirealizeTest, AddSqOperation)
{
	const std::string operation = "add_sq";
	const double first_number = 1.134;
	const double second_number = 2.456;
	EXPECT_EQ(std::pow(first_number, 2) + std::pow(second_number, 2), do_operation_from_str(operation, first_number, second_number));
}

TEST(DesirealizeTest, SqAddOperation)
{
	const std::string operation = "sq_add";
	const double first_number = 1.134;
	const double second_number = 2.456;
	EXPECT_EQ(std::pow(first_number + second_number, 2), do_operation_from_str(operation, first_number, second_number));
}

TEST(DesirealizeTest, InvalidCommand)
{
	const std::string operation = "MINUS";
	const double first_number = 1.134;
	const double second_number = 2.456;
	EXPECT_THROW(do_operation_from_str(operation, first_number, second_number), invalid_command);
}

TEST(DesirealizeTest, InvalidNumber)
{
	const std::string operation = "minus";
	const double second_number = 2.456;
	text_file_separated_by_simple_delimiters_parser parser;
	std::stringstream stream;
	stream << operation + " " + "t2h48h3o3jerg" + " " + std::to_string(second_number);
	auto tokens = parser.get_parsed(stream);
	EXPECT_THROW(get_operation_result<double>(tokens), std::invalid_argument);
}
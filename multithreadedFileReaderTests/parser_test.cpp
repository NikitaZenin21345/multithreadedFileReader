#include "pch.h"
#include <iosfwd>
#include <string>
#include <gtest/gtest.h>
#include "parser.h"
class parserTests : public ::testing::Test {
protected:
    virtual void SetUp(void) {

    }
    virtual void TearDown(void)
    {  }
    std::stringstream stream;

};

TEST_F(parserTests, parserString)
{
    const std::string operation_name = "Minus";
    const std::string first_number_name = "9835044";
    const std::string second_number_name = "239064986";
    const std::string newElementName = operation_name + " " + first_number_name + " " + second_number_name;
    stream << newElementName;
    text_file_separated_by_simple_delimiters_parser parser;
    auto parsedString = parser.get_parsed(stream);
    EXPECT_EQ(parsedString.getTokens(0), operation_name);
    EXPECT_EQ(parsedString.getTokens(1), first_number_name);
    EXPECT_EQ(parsedString.getTokens(2), second_number_name);
    stream.clear();
}

TEST_F(parserTests, parseStream)
{
    size_t size = 100;
    for (size_t i = 0; i < size; ++i)
    {
        std::string newElementName = std::to_string(i) + " " + std::to_string(i) + " " + std::to_string(i);
        stream << newElementName << std::endl;
    }
    text_file_separated_by_simple_delimiters_parser parser;
    for (size_t i = 0; i < size ; ++i)
    {
        auto parsedStream = parser.get_parsed(stream);
        EXPECT_EQ(parsedStream.isValid(), true);
        EXPECT_EQ(parsedStream.getTokens(0), std::to_string(i));
        EXPECT_EQ(parsedStream.getTokens(1), std::to_string(i));
        EXPECT_EQ(parsedStream.getTokens(2), std::to_string(i));
    }
}

TEST_F(parserTests, parserExceptions)
{
    const std::string operation_name = "divide";
    const std::string first_number_name = "394875904.3464";
    const std::string second_number_name = "8347583.34523";
    const std::string newElementName = operation_name + " " + first_number_name + second_number_name;
    stream << newElementName;
    text_file_separated_by_simple_delimiters_parser parser;
    auto parsedString = parser.get_parsed(stream);
    EXPECT_THROW(auto noUsed = parsedString.getTokens(4), std::out_of_range);
}
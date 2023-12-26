#pragma once
#include <string>

class invalid_number final : public std::exception
{
	std::string message;
public:
	invalid_number(const std::string& message_) : message(message_) {}
	[[nodiscard]] char const* what() const override
	{
		return message.c_str();
	}
};

class invalid_command final : public std::exception
{
	std::string message;
public:
	invalid_command(const std::string& message_) : message(message_) {}
	[[nodiscard]] char const* what() const override
	{
		return message.c_str();
	}
};
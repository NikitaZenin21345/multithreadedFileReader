#pragma once
#include <iostream>
#include "deserialize_exception.h"
#include "parser.h"

namespace impl
{
	constexpr size_t operation_position = 0;
	constexpr size_t first_number_position = 1;
	constexpr size_t second_number_position = 2;
	enum class command { plus, minus, multiply, divide, add_sq, sq_add, invalid };

	command command_convertor(const std::string& command);

	template<typename Number>
		requires std::is_arithmetic_v<Number>
	bool is_equal(const Number first,const Number second)
	{
		return first == second;
	}

	template<>
	inline bool is_equal(const double first,const double second)
	{
		return std::abs(first - second) < std::numeric_limits<double>::epsilon();
	}

	template<typename Number>
	Number do_operation(Number first, Number second, const command input_command)
	{
		switch (input_command)
		{
		case command::plus:
		{
			return std::plus<Number>()(first, second);
		}
		case command::minus:
		{
			return std::minus<Number>()(first, second);
		}
		case command::multiply:
		{
			return std::multiplies<Number>()(first, second);
		}
		case command::divide:
		{
			if (is_equal<Number>(second, Number{})) { throw invalid_number("Division on zero"); }
			return std::multiplies<Number>()(first, 1 / second);
		}
		case command::add_sq:
		{
			return std::plus<Number>()(static_cast<Number>(std::pow(first, 2)), static_cast<Number>(std::pow(second, 2)));
		}
		case command::sq_add:
		{
			return static_cast<Number>(std::pow(std::plus<Number>()(first, second), 2));
		}
		default:
		{
			return Number{};
		}
		}
	}

	template<typename Number>
	Number get_number([[maybe_unused]] const std::string& number_str)
	{
		return Number{};
	}

	template<>
	inline int get_number(const std::string& number_str)
	{
		return std::stoi(number_str);
	}

	template<>
	inline double get_number(const std::string& number_str)
	{
		return std::stod(number_str);
	}

	template<>
	inline float get_number(const std::string& number_str)
	{
		return std::stof(number_str);
	}

	template<>
	inline unsigned long get_number(const std::string& number_str)
	{
		return std::stoul(number_str);
	}

	template<typename Number>
	class default_str_to_number_convertor
	{
	public:
		Number operator()(const std::string& number_str)
		{
			return get_number<Number>(number_str);
		}
	};
}

template<typename Number, typename Str_to_number_convertor = impl::default_str_to_number_convertor<Number>>
Number get_operation_result(const tokens& tokens_list, Str_to_number_convertor number_convertor = Str_to_number_convertor())
{
	const auto input_command = impl::command_convertor(tokens_list.getTokens(impl::operation_position));
	const auto first_number = number_convertor(tokens_list.getTokens(impl::first_number_position));
	const auto second_number = number_convertor(tokens_list.getTokens(impl::second_number_position));
	return do_operation<Number>(first_number, second_number, input_command);
};

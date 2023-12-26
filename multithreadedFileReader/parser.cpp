#include <string>
#include <vector>
#include <fstream>
#include "parser.h"

namespace
{
	size_t add_token(const std::string& input_str, const size_t start_position, std::vector<std::string>& tokens, const std::string& forbidden)
	{
		size_t start = input_str.find_first_not_of(forbidden, start_position);
		if (start == std::string::npos)
		{
			return std::string::npos;
		}
		size_t end = input_str.find_first_of(forbidden, start + 1);
		if (end == std::string::npos)
		{
			end = input_str.size();
		}
		std::string substr = input_str.substr(start, end - start);
		tokens.emplace_back(std::move(substr));
		return end;
	}
	template<typename Parsed_stream, typename Tokens>
	Tokens parse(Parsed_stream& stream, const std::string& forbidden)
	{
		size_t index = 0;
		std::string input_str;
		std::vector<std::string> tokensList;
		if (std::getline(stream, input_str))
		{
			if (!input_str.empty())
			{
				while (index != std::string::npos)
				{
					index = add_token(input_str, index, tokensList, forbidden);
				}
				return Tokens{ std::move(tokensList), true };
			}
		}
		return Tokens{ std::move(tokensList), false };
	}
}


auto text_file_separated_by_simple_delimiters_parser::get_parsed(std::istream& stream_, const std::string& forbidden_) -> tokens
{
	return parse<std::istream, tokens>(stream_, forbidden_);
}

#pragma once
#include <fstream>
#include "token.h"

template<typename Parsed_stream, typename Tokens>
class text_file_parser
{
public:
	virtual Tokens get_parsed(Parsed_stream& stream_, const std::string& forbidden_) = 0;
	virtual ~text_file_parser() = default;
};

class text_file_separated_by_simple_delimiters_parser final : public text_file_parser<std::istream, tokens>
{
public:
	auto get_parsed(std::istream& stream_, const std::string& forbidden_ = " ") -> tokens override;
};
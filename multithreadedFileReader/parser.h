#pragma once
#include <string>
#include <fstream>
#include "token.h"

template<typename Parsed_stream, typename Tokens>
class parser
{
public:
	virtual Tokens get_parsed(Parsed_stream& stream_, const std::string& forbidden_) = 0;
	virtual ~parser() = default;
};

class text_file_parser final : public parser<std::ifstream, tokens>
{
public:
	auto get_parsed(std::ifstream& stream_, const std::string& forbidden_ = " ") -> tokens override;
};
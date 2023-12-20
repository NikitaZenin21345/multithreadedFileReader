#include "deserializer.h"

impl::command impl::command_convertor(const std::string& command) 
{
	if (command == "plus")
	{
		return command::plus;
	}
	if (command == "minus")
	{
		return command::minus;
	}
	if (command == "multiply")
	{
		return command::multiply;
	}
	if (command == "divide")
	{
		return command::divide;
	}
	if (command == "sq_add")
	{
		return command::sq_add;
	}
	if (command == "add_sq")
	{
		return command::add_sq;
	}
	throw invalid_command("invalid command");
}

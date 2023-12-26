#include "operation_deserializer.h"

#include <unordered_map>

namespace 
{
	std::unordered_map<std::string, impl::command> operations{
		{"plus", impl::command::plus},
		{"minus", impl::command::minus},
		{"multiply", impl::command::multiply},
		{"divide", impl::command::divide},
		{"sq_add", impl::command::sq_add},
		{"add_sq", impl::command::add_sq},
	};

}

impl::command impl::command_convertor(const std::string& command) 
{
	auto found_command = operations.find(command);
	if(found_command == operations.end())
	{
		throw invalid_command("invalid command");
	}
	return found_command->second;
}

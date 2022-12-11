#include <iostream>

#include "Type_Manager.h"

#include "Variable_Base.h"

int main()
{
	Type_Manager::register_type("int",
		{
			[](void* _data_vptr){ delete (int*)_data_vptr; },
			[](void* _variable_vptr, const std::string& _value_as_string){ *(int*)_variable_vptr = std::stoi(_value_as_string); },
			[](void* _variable_vptr, void* _data_ptr){ *(int*)_variable_vptr = *(int*)_data_ptr; }
		}
	);

//	std::string int_as_string = "123";

//	int value;

//	Type_Manager::parse("int", int_as_string, &value);

//	std::cout << value;

	std::map<std::string, std::string> values;
	values.emplace("ass", "123");

	Variable_Test test;
	test.assign_values(values);

	return 0;
}

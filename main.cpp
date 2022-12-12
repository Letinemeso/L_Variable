#include <iostream>

#include "Type_Manager.h"

#include "Variable_Base.h"
#include "Int_Variable.h"
#include "String_Variable.h"
#include "Float_Variable.h"
#include "Bool_Variable.h"

#include "Variable_Manager.h"

int main()
{
	Type_Manager::register_type("int",
		{
			[](void* _data_vptr){ delete (int*)_data_vptr; },
			[](void* _variable_vptr, const std::string& _value_as_string){ *(int*)_variable_vptr = std::stoi(_value_as_string); },
			[](void* _variable_vptr, void* _data_ptr){ *(int*)_variable_vptr = *(int*)_data_ptr; }
		}
	);
	Type_Manager::register_type("float",
		{
			[](void* _data_vptr){ delete (float*)_data_vptr; },
			[](void* _variable_vptr, const std::string& _value_as_string)
									{
										*(float*)_variable_vptr = std::stof(_value_as_string);
									},
			[](void* _variable_vptr, void* _data_ptr){ *(float*)_variable_vptr = *(float*)_data_ptr; }
		}
	);
	Type_Manager::register_type("bool",
		{
			[](void* _data_vptr){ delete (bool*)_data_vptr; },
			[](void* _variable_vptr, const std::string& _value_as_string)
									{
										if(_value_as_string == "true")
											*(bool*)_variable_vptr = true;
										else if(_value_as_string == "false")
											*(bool*)_variable_vptr = false;
									},
			[](void* _variable_vptr, void* _data_ptr){ *(float*)_variable_vptr = *(float*)_data_ptr; }
		}
	);
	Type_Manager::register_type("std::string",
		{
			[](void* _data_vptr){ delete (std::string*)_data_vptr; },
			[](void* _variable_vptr, const std::string& _value_as_string){ *(std::string*)_variable_vptr = _value_as_string; },
			[](void* _variable_vptr, void* _data_ptr){ *(std::string*)_variable_vptr = *(std::string*)_data_ptr; }
		}
	);

//	std::string int_as_string = "123";

//	int value;

//	Type_Manager::parse("int", int_as_string, &value);

//	std::cout << value;


	std::list<Variable_Stub> stubs;
	stubs.push_back({"ass_int", "123"});
	stubs.push_back({"ass_float", "123.567"});
	stubs.push_back({"ass_bool", "true"});
	stubs.push_back({"ass_string", "\"123 123.567 true ass\""});

	Variable_Manager vm;

	vm.add_values(stubs);


	Int_Variable* I = cast_variable<Int_Variable>(vm.get_variable("ass_int"));


	return 0;
}

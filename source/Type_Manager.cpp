#include "Type_Manager.h"

using namespace LV;


Type_Manager::Registred_Types_Map Type_Manager::m_registred_types;



void Type_Manager::register_basic_types()
{
    register_type("int", {
                             [](const std::string& _val)
                             {
                                 unsigned int i=0;
                                 if(_val[0] == '+' || _val[0] == '-')
                                     ++i;
                                 for(; i<_val.size(); ++i)
                                     if(_val[i] < '0' || _val[i] > '9')
                                         return false;
                                 return true;
                             },
                             [](void* _variable_vptr, const LDS::Vector<std::string>& _values_as_string) { *((int*)_variable_vptr) = std::stoi(_values_as_string[0]); }
                         });
    register_type("unsigned int", {
                                      [](const std::string& _val)
                                      {
                                          for(unsigned int i=0; i<_val.size(); ++i)
                                              if(_val[i] < '0' || _val[i] > '9')
                                                  return false;
                                          return true;
                                      },
                                      [](void* _variable_vptr, const LDS::Vector<std::string>& _values_as_string) { *((int*)_variable_vptr) = std::stoi(_values_as_string[0]); }
                                  });
    register_type("LDS::Vector<unsigned int>", {
                                                   [](const std::string& _val)
                                                   {
                                                       for(unsigned int i=0; i<_val.size(); ++i)
                                                       {
                                                           if(_val[i] < '0' || _val[i] > '9')
                                                               return false;
                                                       }
                                                       return true;
                                                   },
                                                   [](void* _variable_vptr, const LDS::Vector<std::string>& _values_as_string)
                                                   {
                                                       LDS::Vector<unsigned int>& vector = *((LDS::Vector<unsigned int>*)_variable_vptr);
                                                       vector.clear();
                                                       vector.resize(_values_as_string.size());
                                                       for(unsigned int i=0; i<_values_as_string.size(); ++i)
                                                           vector.push(std::stoi(_values_as_string[i]));
                                                   }
                                               });
    register_type("bool", {
                              [](const std::string& _val)
                              {
                                  if(_val == "true" || _val == "false" || _val == "+" || _val == "-" || _val == "1" || _val == "0")
                                      return true;
                                  return false;
                              },
                              [](void* _variable_vptr, const LDS::Vector<std::string>& _values_as_string)
                              {
                                  bool& var = *((bool*&)_variable_vptr);

                                  if(_values_as_string[0] == "true" || _values_as_string[0] == "+" || _values_as_string[0] == "1")
                                      var = true;
                                  else if(_values_as_string[0] == "false" || _values_as_string[0] == "-" || _values_as_string[0] == "0")
                                      var = false;
                              }
                          });
    register_type("LDS::Vector<bool>", {
                                           [](const std::string& _val)
                                           {
                                               if(_val == "true" || _val == "false" || _val == "+" || _val == "-" || _val == "1" || _val == "0")
                                                   return true;
                                               return false;
                                           },
                                           [](void* _variable_vptr, const LDS::Vector<std::string>& _values_as_string)
                                           {
                                               LDS::Vector<bool>& vector = *((LDS::Vector<bool>*)_variable_vptr);
                                               vector.clear();
                                               vector.resize(_values_as_string.size());
                                               for(unsigned int i=0; i<_values_as_string.size(); ++i)
                                               {
                                                   if(_values_as_string[i] == "true" || _values_as_string[i] == "+" || _values_as_string[i] == "1")
                                                       vector.push(true);
                                                   else if(_values_as_string[i] == "false" || _values_as_string[i] == "-" || _values_as_string[i] == "0")
                                                       vector.push(false);
                                               }
                                           }
                                       });
    register_type("std::string", {
                                     [](const std::string& /*_val*/) { return true; },
                                     [](void* _variable_vptr, const LDS::Vector<std::string>& _values_as_string) {
                                         *((std::string*)_variable_vptr) = _values_as_string[0];
                                     }
                                 });
    register_type("LDS::Vector<std::string>", {
                                                  [](const std::string& _val)
                                                  {
                                                      return true;
                                                  },
                                                  [](void* _variable_vptr, const LDS::Vector<std::string>& _values_as_string)
                                                  {
                                                      LDS::Vector<std::string>& vector = *((LDS::Vector<std::string>*)_variable_vptr);
                                                      vector.clear();
                                                      vector = _values_as_string;
                                                  }
                                              });
    register_type("float", {
                               [](const std::string& _val)
                               {
                                   if(_val == ".")
                                       return false;

                                   unsigned int dots_count = 0;
                                   unsigned int i=0;
                                   if(_val[0] == '+' || _val[0] == '-')
                                       ++i;
                                   for(; i<_val.size(); ++i)
                                   {
                                       if(_val[i] == '.')
                                       {
                                           ++dots_count;
                                           continue;
                                       }
                                       if(_val[i] < '0' || _val[i] > '9')
                                           return false;
                                   }

                                   if(dots_count > 1)
                                       return false;

                                   return true;
                               },
                               [](void* _variable_vptr, const LDS::Vector<std::string>& _values_as_string) { *((float*)_variable_vptr) = std::stof(_values_as_string[0]); }
                           });
    register_type("LDS::Vector<float>", {
                                            [](const std::string& _val)
                                            {
                                                if(_val == ".")
                                                    return false;

                                                unsigned int dots_count = 0;
                                                unsigned int i=0;
                                                if(_val[0] == '+' || _val[0] == '-')
                                                    ++i;
                                                for(; i<_val.size(); ++i)
                                                {
                                                    if(_val[i] == '.')
                                                    {
                                                        ++dots_count;
                                                        continue;
                                                    }
                                                    if(_val[i] < '0' || _val[i] > '9')
                                                        return false;
                                                }

                                                if(dots_count > 1)
                                                    return false;

                                                return true;
                                            },
                                            [](void* _variable_vptr, const LDS::Vector<std::string>& _values_as_string)
                                            {
                                                LDS::Vector<float>& vector = *((LDS::Vector<float>*)_variable_vptr);
                                                vector.clear();
                                                vector.resize(_values_as_string.size());
                                                for(unsigned int i=0; i<_values_as_string.size(); ++i)
                                                    vector.push(std::stof(_values_as_string[i]));
                                            }
                                        });

    // LDS::Map<std::string, std::string> - had to name differently due to macros not seeing comma as a part of the type
    register_type("String_To_String_Map", {
                                              [](const std::string& _val)
                                              {
                                                  return true;
                                              },
                                              [](void* _variable_vptr, const LDS::Vector<std::string>& _values_as_string)
                                              {
                                                  L_ASSERT(_values_as_string.size() % 2 == 0);

                                                  LDS::Map<std::string, std::string>& map = *((LDS::Map<std::string, std::string>*)_variable_vptr);
                                                  map.clear();
                                                  for(unsigned int i=0; i<_values_as_string.size(); i += 2)
                                                      map.insert(_values_as_string[i], _values_as_string[i + 1]);
                                              }
                                          });
}

void Type_Manager::register_type(const std::string &_type_name, const Type_Utility &_utility, bool _override)
{
    Registred_Types_Map::Iterator maybe_registred_type = m_registred_types.find(_type_name);

    if(!maybe_registred_type.is_ok())
        m_registred_types.insert(_type_name, _utility);
    else if(_override)
        *maybe_registred_type = _utility;
}



bool Type_Manager::validate(const std::string &_type_name, const std::string &_value_as_string)
{
    Registred_Types_Map::Iterator utility_it = m_registred_types.find(_type_name);
    L_ASSERT(utility_it.is_ok());

    const Type_Utility& utility = *utility_it;

	return utility.validation_func(_value_as_string);
}

void Type_Manager::parse(const std::string& _type_name, const LDS::Vector<std::string>& _values_as_string, void* _allocate_to)
{
    Registred_Types_Map::Iterator utility_it = m_registred_types.find(_type_name);
    L_ASSERT(utility_it.is_ok());

    const Type_Utility& utility = *utility_it;

	L_DEBUG_FUNC_NOARG([&]()	//	crashes if validation is failed
	{
		for(unsigned int i=0; i<_values_as_string.size(); ++i)
			L_ASSERT(utility.validation_func(_values_as_string[i]));
	});

	utility.parse_func(_allocate_to, _values_as_string);
}

#include "OMFL_Reader.h"


Variable_Stub OMFL_Reader::M_parse_line(const std::string &_raw_data, unsigned int &_offset) const
{
	Variable_Stub result;

	if(_raw_data.size() == 0)
		return result;

	if(_offset >= _raw_data.size())
		return result;

	unsigned int initial_offset = _offset;

	unsigned int first_equals_pos = initial_offset;

	unsigned int end = initial_offset + 1;
	for(; end < _raw_data.size() && _raw_data[end] != '\n'; ++end)
	{
		if(_raw_data[end] == '=' && first_equals_pos == initial_offset)
			first_equals_pos = end - initial_offset;
	}

	_offset = end + 1;

	std::string line = _raw_data.substr(initial_offset, end - initial_offset);

	std::string word = M_parse_word(line, 0);

	if(word[0] == '#')	//	comment
		return result;

	if(word[0] == '[')	//	section
	{
		M_parse_section(line, result);
		return result;
	}

	if(first_equals_pos == initial_offset)	//	no equals sign found (invalid syntax)
		return result;

	if(!M_variable_name_is_valid(word))	// chess speaks for itself
		return result;

	std::string value = M_parse_word(line, first_equals_pos + 1);

	if(value.size() == 0)	//	variable is empty
		return result;

	result.name = word;

	if(value[0] == '[')	//	variable is array
	{
		M_parse_array(M_parse_all_array_variables(line.substr(first_equals_pos + 1)), result);
		return result;
	}
	if(value[0] == '\"')	//	variable is string
	{
		result.value = M_parse_string(line.substr(first_equals_pos + 1));
		if(result.value.size() == 0)
			result.name = "";
		return result;
	}

	result.value = value;
	return result;
}

void OMFL_Reader::M_parse_section(const std::string &_line, Variable_Stub &_result) const
{
	unsigned int offset = 0;
	for(; offset < _line.size() && _line[offset] != '['; ++offset) { }
	++offset;

	if(offset >= _line.size())
		return;

	unsigned int end = offset + 1;
	for(; end < _line.size() && _line[end] != ']'; ++end) { }

	if(end >= _line.size())
		return;

	if(offset >= end)
		return;

	_result.name = _line.substr(offset, end - offset);
	_result.value = "";
}

std::string OMFL_Reader::M_parse_string(const std::string &_line) const
{
	unsigned int offset = 0;
	for(; offset < _line.size() && _line[offset] != '\"'; ++offset) { }

	if(offset >= _line.size())
		return "";

	unsigned int end = offset + 1;
	for(; end < _line.size() && _line[end] != '\"'; ++end) { }

	if(end >= _line.size())
		return "";

	return _line.substr(offset, end + 1 - offset);
}

void OMFL_Reader::M_parse_array(const std::string &_array, Variable_Stub &_result) const
{
	unsigned int offset = 1;
	while(offset < _array.size())
	{
		std::string variable = M_parse_array_variable(_array, offset);

		if(_array[offset - 1] != ',')
			offset = M_find_next_sumbol_pos(_array, offset, ',') + 1;

		if(variable.size() == 0)
		{
			_result.childs.clear();
			return;
		}

		Variable_Stub stub;
		if(variable[0] == '[')
		{
			M_parse_array(variable, stub);
			if(stub.childs.size() == 0)
			{
				_result.childs.clear();
				return;
			}
		}
		else
			stub.value = variable;

		_result.childs.push_back(stub);
	}

	_result.value = "[]";
}

std::string OMFL_Reader::M_parse_array_variable(const std::string &_array, unsigned int& _offset) const
{
	unsigned int first_symbol_pos = _offset;
	for(; first_symbol_pos < _array.size() && (_array[first_symbol_pos] == ' ' || _array[first_symbol_pos] == '\t'); ++first_symbol_pos) { }

	std::string value;

	if(_array[first_symbol_pos] == '\"')
		value = M_parse_string(_array.substr(first_symbol_pos, _array.size()));
	else if(_array[first_symbol_pos] == '[')
		value = M_parse_all_array_variables(_array.substr(first_symbol_pos, _array.size()));

	if(value.size() > 0)
	{
		_offset = first_symbol_pos + value.size() + 1;
		return value;
	}

	unsigned int comma_pos = first_symbol_pos + 1;
	for(; comma_pos < _array.size() && _array[comma_pos] != ',' && _array[comma_pos] != ']'; ++comma_pos) { }
	_offset = comma_pos + 1;

	if(comma_pos == _array.size())
		return "";

	value = _array.substr(first_symbol_pos, comma_pos - first_symbol_pos);
	return value;
}

std::string OMFL_Reader::M_parse_all_array_variables(const std::string _array) const
{
	unsigned int skip_close_brackets = 0;

	unsigned int offset = 0;
	for(; offset < _array.size() && _array[offset] != '['; ++offset) { }

	if(offset == _array.size())
		return "";

	unsigned int end = offset + 1;
	for(; end < _array.size(); ++end)
	{
		if(_array[end] == '[')
			++skip_close_brackets;

		if(_array[end] == ']')
		{
			if(skip_close_brackets == 0)
				break;
			else
				--skip_close_brackets;
		}
	}

	if(end == _array.size())
		return "";

	return _array.substr(offset, end + 1);
}

std::string OMFL_Reader::M_parse_word(const std::string &_line, unsigned int _offset) const
{
	for(; _offset < _line.size() && (_line[_offset] == ' ' || _line[_offset] == '\t'); ++_offset) { }

	if(_offset == _line.size())
		return "";

	unsigned int end = _offset + 1;
	for(; end < _line.size() && (_line[end] != ' ' && _line[end] != '\t'); ++end) { }

	return _line.substr(_offset, end - _offset);
}

bool OMFL_Reader::M_variable_name_is_valid(const std::string &_name) const
{
	if(_name.size() == 0)
		return false;

	for(unsigned int i=0; i<_name.size(); ++i)
	{
		if( ! ( (_name[i] >= 'A' && _name[i] <= 'Z') || (_name[i] >= 'a' && _name[i] <= 'z') || (_name[i] <= '1' && _name[i] >= '0') || _name[i] == '-' || _name[i] == '_' ) )
			return false;
	}

	return true;
}

unsigned int OMFL_Reader::M_find_next_sumbol_pos(const std::string &_str, unsigned int _offset, char _symbol) const
{
	for(; _offset < _str.size() && _str[_offset] != _symbol; ++_offset) { }
	return _offset;
}



void OMFL_Reader::parse(const std::string& _raw_data)
{
	unsigned int offset = 0;

	while(offset < _raw_data.size())
	{
		Variable_Stub stub = M_parse_line(_raw_data, offset);

		if(stub.is_section())
		{
			m_current_section = stub.name;
			continue;
		}

		if(!stub.ok())
			continue;

		Parsed_Data_t::iterator section_it = m_parsed_data.find(m_current_section);
		if(section_it == m_parsed_data.end())
			section_it = m_parsed_data.emplace(m_current_section, std::list<Variable_Stub>()).first;

		section_it->second.push_back(stub);
	}
}


const OMFL_Reader::Parsed_Data_t& OMFL_Reader::parsed_data() const
{
	return m_parsed_data;
}

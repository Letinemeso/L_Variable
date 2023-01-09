#include "MDL_Reader.h"

using namespace LV;


MDL_Reader::MDL_Reader()
{

}

MDL_Reader::~MDL_Reader()
{

}



void MDL_Reader::parse_file(const std::string &_path)
{
	M_parse_file_ex(_path, false);
}

const std::list<MDL_Variable_Stub>& MDL_Reader::stubs() const
{
	return m_stubs;
}



std::string MDL_Reader::M_extract_from_file(const std::string &_path)
{
	std::ifstream file(_path + ".mdl", std::ifstream::binary);
	L_ASSERT(file.is_open());

	file.seekg(0, std::ifstream::end);
	unsigned int length = file.tellg();
	file.seekg(0, std::ifstream::beg);

	std::string result;
	result.resize(length);
	for(unsigned int i=0; i<length; ++i)
		result[i] = file.get();

	file.close();

	return result;
}


void MDL_Reader::M_parse_file_ex(const std::string &_path, bool _append)
{
	if(!_append)
		m_stubs.clear();

	std::string raw_data = M_extract_from_file(_path);
	unsigned int raw_size = raw_data.size();

	unsigned int offset = 0;
	while(offset < raw_data.size())
		m_stubs.push_back(M_parse_stub(raw_data, offset));
}

unsigned int MDL_Reader::M_find_symbol(const std::string &_str, unsigned int _offset, char _symbol) const
{
	for(; _offset < _str.size(); ++_offset)
		if(_str[_offset] == _symbol)
			break;
	return _offset;
}

MDL_Variable_Stub MDL_Reader::M_parse_stub(const std::string &_raw, unsigned int &_offset) const
{
	unsigned int offset = M_find_symbol(_raw, _offset, '\n');
	std::string line = _raw.substr(_offset, offset - _offset);
	_offset = offset + 1;

	MDL_Variable_Stub stub;
	stub.type = M_parse_type(line);
	L_ASSERT(stub.type.size() > 0);
	stub.name = M_parse_name(line);
	L_ASSERT(stub.name.size() > 0);

	std::string data = M_extract_variable_data(_raw, _offset);
	stub.fields = M_parse_fields(data);

	_offset += data.size() + 1;

	return stub;
}


bool MDL_Reader::M_line_is_empty(const std::string &_line) const
{
	for(unsigned int i=0; i<_line.size(); ++i)
	{
		if(_line[i] == '*')
			return true;
		if(_line[i] == ' ' || _line[i] == '\t' || _line[i] == '\n' || _line[i] == '\r')
			continue;
		return false;
	}

	return true;
}

bool MDL_Reader::M_line_is_opener(const std::string &_line) const
{
	bool found_opener = false;
	for(unsigned int i=0; i<_line.size(); ++i)
	{
		if(_line[i] == '*')
			return false;
		if(_line[i] == ' ' || _line[i] == '\t' || _line[i] == '\n' || _line[i] == '\r')
			continue;
		if(_line[i] == '\\' && found_opener == false)
			found_opener = true;
		else
			return false;
	}

	return found_opener;
}

bool MDL_Reader::M_line_is_closer(const std::string &_line) const
{
	bool found_closer = false;
	for(unsigned int i=0; i<_line.size(); ++i)
	{
		if(_line[i] == '*')
			return false;
		if(_line[i] == ' ' || _line[i] == '\t' || _line[i] == '\n' || _line[i] == '\r')
			continue;
		if(_line[i] == '/' && found_closer == false)
			found_closer = true;
		else
			return false;
	}

	return found_closer;
}

std::string MDL_Reader::M_extract_variable_data(const std::string &_raw, unsigned int& _offset) const
{
	unsigned int data_start = M_find_symbol(_raw, _offset, '\\');

	while(true)
	{
		std::string line = M_extract_line(_raw, data_start);
		data_start += line.size() + 1;
		if(M_line_is_opener(line))
			break;
	}

	unsigned int data_end = data_start + 1;
	unsigned int skip_closers = 0;
	while(true)
	{
		std::string line = M_extract_line(_raw, data_end);
		data_end += line.size() + 1;
		if(M_line_is_opener(line))
		{
			++skip_closers;
			continue;
		}
		if(M_line_is_closer(line))
		{
			if(skip_closers > 0)
			{
				--skip_closers;
				continue;
			}

			data_end -= line.size();
			break;
		}
	}

	L_ASSERT(data_start < _raw.size() && data_end < _raw.size());

	_offset = data_end + 1;
	return _raw.substr(data_start, data_end - data_start);
}

std::string MDL_Reader::M_extract_line(const std::string &_raw, unsigned int _offset) const
{
	unsigned int end = M_find_symbol(_raw, _offset, '\n');
	return _raw.substr(_offset, end - _offset);
}


std::string MDL_Reader::M_parse_type(const std::string &_line) const
{
	unsigned int start = M_find_symbol(_line, 0, '<') + 1;
	unsigned int end = M_find_symbol(_line, start, '>');

	L_ASSERT(start < _line.size() && end < _line.size());

	return _line.substr(start, end - start);
}

std::string MDL_Reader::M_parse_name(const std::string &_line) const
{
	unsigned int start = M_find_symbol(_line, 0, '|') + 1;
	unsigned int end = M_find_symbol(_line, start, '|');

	L_ASSERT(start < _line.size() && end < _line.size());

	return _line.substr(start, end - start);
}

MDL_Variable_Stub::fields_t MDL_Reader::M_parse_fields(const std::string &_raw_data) const
{
	unsigned int offset = 0;

	MDL_Variable_Stub::fields_t result;

	while(offset < _raw_data.size())
	{
		unsigned int line_size = M_find_symbol(_raw_data, offset, '\n') - offset;
		std::string line = _raw_data.substr(offset, line_size);
		offset += line_size + 1;

		if(M_line_is_empty(line))
			continue;

		std::string name = M_parse_name(line);
		L_ASSERT(name.size() > 0);

		std::string simple_data_raw_str = M_extract_variable_data(_raw_data, offset);

		LDS::Vector<std::string> values = M_parse_simple_data(simple_data_raw_str);

		result.emplace(name, values);

//		offset += simple_data_raw_str.size() + 1;
	}

	return result;
}

unsigned int MDL_Reader::M_parse_amount(const std::string &_line) const
{
	unsigned int result = 0;
	for(unsigned int i=0; i<_line.size(); ++i)
	{
		if(_line[i] == '\"')
			++result;
	}

	L_ASSERT(result % 2 == 0);
	L_ASSERT(result > 0);

	return result / 2;
}

LDS::Vector<std::string> MDL_Reader::M_parse_simple_data(const std::string &_line) const
{
	unsigned int amount = M_parse_amount(_line);
	LDS::Vector<std::string> result;
	result.resize(amount);

	unsigned int offset = 0;
	for(unsigned int i=0; i<amount; ++i)
	{
		offset = M_find_symbol(_line, offset, '\"') + 1;
		unsigned int end = M_find_symbol(_line, offset, '\"');

		std::string str_value;
		if(end > offset)
			str_value = _line.substr(offset, end - offset);

		result.push(str_value);

		offset = end + 1;
	}

	return result;
}
































































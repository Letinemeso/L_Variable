#include "MDL_Reader.h"

#include <Stuff/File.h>

using namespace LV;


MDL_Reader::MDL_Reader()
{

}

MDL_Reader::~MDL_Reader()
{

}



void MDL_Reader::parse_predefines_file(const std::string& _path)
{
    M_parse_file(_path, m_predefines);
}

void MDL_Reader::parse_file(const std::string &_path)
{
    M_parse_file(_path, m_stubs);
}

void MDL_Reader::add_stub(const std::string& _name, const MDL_Variable& _stub)
{
    L_ASSERT(!m_stubs.find(_name).is_ok());

    m_stubs.insert(_name, _stub);
}

void MDL_Reader::add_stub(const std::string& _name, MDL_Variable&& _stub)
{
    L_ASSERT(!m_stubs.find(_name).is_ok());

    m_stubs.insert(_name, (MDL_Variable&&)_stub);
}

void MDL_Reader::clear_predefines()
{
    m_predefines.clear();
}

void MDL_Reader::clear()
{
	m_stubs.clear();
}

void MDL_Reader::save_to_file(const std::string &_path) const
{
	std::ofstream file(_path + ".mdl", std::ofstream::binary);
	L_ASSERT(file.is_open());

	file << "# This file was auto generated #\n\n";

    for(Stub_Map::Const_Iterator stub_it = m_stubs.iterator(); !stub_it.end_reached(); ++stub_it)
    {
        M_save_stub_to_file(file, stub_it.key(), *stub_it);
        file << "\n";
    }

	file.close();
}



const MDL_Reader::Stub_Map& MDL_Reader::stubs() const
{
	return m_stubs;
}

const MDL_Variable& MDL_Reader::get_stub(const std::string &_name) const
{
    Stub_Map::Const_Iterator it = m_stubs.find(_name);
    L_ASSERT_WITH_INFO(it.is_ok(), "Object with name \"" + _name + "\" not found");
    return *it;
}



void MDL_Reader::M_parse_file(const std::string& _path, Stub_Map& _result_storage)
{
    std::string raw_data = M_extract_from_file(_path);
    unsigned int raw_size = raw_data.size();

    unsigned int offset = 0;
    while(offset < raw_data.size())
    {
        std::string line = M_extract_line(raw_data, offset);
        offset += line.size() + 1;

        while(M_line_is_empty(line) && offset < raw_size)
        {
            line = M_extract_line(raw_data, offset);
            offset += line.size() + 1;
        }

        if(offset >= raw_size)
            break;

        MDL_Variable stub;
        std::string name = M_parse_name(line);
        L_ASSERT(name.size() > 0);

        L_ASSERT(!_result_storage.find(name).is_ok());

        std::string data = M_extract_variable_data(raw_data, offset);
        stub = M_parse_stub(data);

        _result_storage.insert((std::string&&)name, (MDL_Variable&&)stub);
    }
}


void MDL_Reader::M_save_stub_to_file(std::ofstream &_file, const std::string& _name, const MDL_Variable &_stub, unsigned int _nesting_level) const
{
    auto output_tabs = [&](unsigned int _amount)
    {
        for(unsigned int i = 0; i < _amount; ++i)
            _file << '\t';
    };

    output_tabs(_nesting_level);

    if(_nesting_level > 0)
        _file << "||" << _name << "||" << "\n";
    else
        _file << "|" << _name << "|" << "\n";

    output_tabs(_nesting_level);

    _file << "\\\n";

    for(LDS::Map<std::string, LDS::Vector<std::string>>::Const_Iterator fields_it = _stub.fields().iterator(); !fields_it.end_reached(); ++fields_it)
    {
        output_tabs(_nesting_level + 1);

        _file << "|" << fields_it.key() << "|" << "\n";

        output_tabs(_nesting_level + 1);

        _file << "\\\n";

        output_tabs(_nesting_level + 2);

        for(LDS::Vector<std::string>::Const_Iterator values_it = fields_it->const_iterator(); !values_it.end_reached(); ++values_it)
            _file << "\"" << *values_it << "\" ";

        _file << "\n";

        output_tabs(_nesting_level + 1);

        _file << "/\n";
    }


    for(LDS::Map<std::string, MDL_Variable>::Const_Iterator child_it = _stub.childs().iterator(); !child_it.end_reached(); ++child_it)
        M_save_stub_to_file(_file, child_it.key(), *child_it, _nesting_level + 1);

    output_tabs(_nesting_level);

    _file << "/\n";
}


void MDL_Reader::M_crop_file_name(std::string& _name) const
{
    unsigned int offset = 0;
    for(; offset < _name.size(); ++offset)
    {
        if(_name[offset] != ' ')
            break;
    }

    if(offset != 0)
        _name = _name.substr(offset);

    unsigned int size = _name.size();
    for(; size > 1; --size)
    {
        if(_name[size - 1] !=  ' ')
            break;
    }

    if(offset != _name.size())
        _name = _name.substr(0, size);
}


std::string MDL_Reader::M_extract_from_file(const std::string &_path) const
{
    LST::File file(_path + ".mdl");
    L_ASSERT_WITH_INFO(file.exists(), "Unable to open \"" + file.get_path() + ".mdl\"");
    std::string source = file.extract_block();

    M_preprocess_commentaries(source);
    M_preprocess_includes(_path, source);
    M_preprocess_value_includes(_path, source);
    M_preprocess_predefines_includes(_path, source);

    return source;
}

void MDL_Reader::M_preprocess_commentaries(std::string &_raw) const
{
    bool is_in_quotes = false;

    for(unsigned int i = 0; i < _raw.size(); ++i)
	{
        if(_raw[i] == '"')
        {
            is_in_quotes = !is_in_quotes;
            continue;
        }

        if(is_in_quotes)
            continue;

		if(_raw[i] != '#')
			continue;

        unsigned int closing_hash_index = i + 1;
        for(; closing_hash_index < _raw.size(); ++closing_hash_index)
        {
            if(_raw[closing_hash_index] == '"')
            {
                is_in_quotes = !is_in_quotes;
                continue;
            }

            if(is_in_quotes)
                continue;

            if(_raw[closing_hash_index] == '#')
                break;
        }

        L_ASSERT(closing_hash_index < _raw.size());  //  commentary is not openned

        _raw = _raw.substr(0, i) + _raw.substr(closing_hash_index + 1);
	}
}

void MDL_Reader::M_preprocess_includes(const std::string& _root_path, std::string& _raw) const
{
    static const std::string include_openner("<<<");
    static const std::string include_closer(">>>");

    const LST::Function<std::string(const std::string&, const std::string&)> extract_replacement_func = [this](const std::string& _root_path, const std::string& _raw_parsed)
    {
        std::string file_name = _raw_parsed;
        M_crop_file_name(file_name);
        std::string path = _root_path + file_name;

        return M_extract_from_file(path);
    };

    M_preprocess_replacement(_root_path, _raw, include_openner, include_closer, extract_replacement_func);
}

void MDL_Reader::M_preprocess_value_includes(const std::string& _root_path, std::string& _raw) const
{
    static const std::string include_openner("<\"<");
    static const std::string include_closer(">\">");

    const LST::Function<std::string(const std::string&, const std::string&)> extract_replacement_func = [this](const std::string& _root_path, const std::string& _raw_parsed)
    {
        std::string file_name = _raw_parsed;
        M_crop_file_name(file_name);
        std::string path = _root_path + file_name;

        LST::File file(path);
        return '"' + file.extract_block() + '"';
    };

    M_preprocess_replacement(_root_path, _raw, include_openner, include_closer, extract_replacement_func);
}

void MDL_Reader::M_preprocess_predefines_includes(const std::string& _root_path, std::string& _raw) const
{
    static const std::string include_openner("<%<");
    static const std::string include_closer(">%>");

    const LST::Function<std::string(const std::string&, const std::string&)> extract_replacement_func = [this](const std::string& _root_path, const std::string& _raw_parsed)
    {
        LDS::Vector<std::string> path = M_parse_simple_data(_raw_parsed);
        L_ASSERT_WITH_INFO(path.size() >= 2, "Incorrect predefines: \"" + _raw_parsed + "\"");

        Stub_Map::Const_Iterator it = m_predefines.find(path[0]);
        L_ASSERT_WITH_INFO(it.is_ok(), "Could not find predefine group \"" + path[0] + "\"");
        for(unsigned int i = 1; i < path.size() - 1; ++i)
        {
            it = it->childs().find(path[i]);
            L_ASSERT_WITH_INFO(it.is_ok(), "Could not find predefine group \"" + path[i] + "\"");
        }

        MDL_Variable::Fields_Map::Const_Iterator fields_it = it->fields().find(path[path.size() - 1]);
        const MDL_Variable::Field_Data_Vector& fields = *fields_it;
        L_ASSERT(fields.size() > 0);

        std::string result;
        for(unsigned int i = 0; i < fields.size(); ++i)
            result += '"' + fields[i] + '"';

        return result;
    };

    M_preprocess_replacement(_root_path, _raw, include_openner, include_closer, extract_replacement_func);
}

void MDL_Reader::M_preprocess_replacement(const std::string& _root_path, std::string& _raw,
                                          const std::string& _opener, const std::string& _closer,
                                          const LST::Function<std::string(const std::string&, const std::string&)>& _extract_replacement) const
{
    bool is_in_quotes = false;

    for(unsigned int i = 0; i < _raw.size() - _closer.size(); ++i)
    {
        if(_raw[i] == '"')
        {
            is_in_quotes = !is_in_quotes;
            continue;
        }

        if(is_in_quotes)
            continue;

        if(!M_compare_substrings(_raw, _opener, i, 0, _opener.size()))
            continue;

        unsigned int closing_hash_index = i + _opener.size();
        for(; closing_hash_index < _raw.size() - _closer.size(); ++closing_hash_index)
        {
            if(_raw[closing_hash_index] == '"')
            {
                is_in_quotes = !is_in_quotes;
                continue;
            }

            if(is_in_quotes)
                continue;

            if(M_compare_substrings(_raw, _closer, closing_hash_index, 0, _closer.size()))
                break;
        }

        L_ASSERT(closing_hash_index < _raw.size());  //  commentary is not openned

        unsigned int replace_begin = i;
        unsigned int replace_end = closing_hash_index + _closer.size();

        std::string path = _root_path.substr(0, M_find_symbol_from_end(_root_path, (int)_root_path.size(), '/') + 1);
        std::string extracted = _raw.substr(i + _opener.size(), closing_hash_index - i - _closer.size());

        std::string raw_nested = _extract_replacement(path, extracted);

        _raw = _raw.substr(0, replace_begin) + raw_nested + _raw.substr(replace_end);

        i += raw_nested.size();
    }
}


unsigned int MDL_Reader::M_find_symbol(const std::string &_str, unsigned int _offset, char _symbol) const
{
	for(; _offset < _str.size(); ++_offset)
		if(_str[_offset] == _symbol)
			break;
	return _offset;
}

unsigned int MDL_Reader::M_find_symbol_from_end(const std::string &_str, int _offset, char _symbol) const
{
    for(; _offset > 0; --_offset)
        if(_str[_offset] == _symbol)
            break;
    if(_offset == -1)
        return _str.size();
    return _offset;
}


bool MDL_Reader::M_line_is_empty(const std::string &_line) const
{
    for(unsigned int i = 0; i < _line.size(); ++i)
	{
		if(_line[i] == ' ' || _line[i] == '\t' || _line[i] == '\n' || _line[i] == '\r')
			continue;
		return false;
	}

	return true;
}

bool MDL_Reader::M_line_is_opener(const std::string &_line) const
{
	bool found_opener = false;
    for(unsigned int i = 0; i < _line.size(); ++i)
	{
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
    for(unsigned int i = 0; i < _line.size(); ++i)
	{
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
	unsigned int data_start = _offset;

	M_skip_past_opener(_raw, data_start);

	unsigned int data_end = data_start;
	M_skip_past_closer(_raw, data_end, true);

	for(; data_end != 0; --data_end)
	{
		if(data_end > _raw.size())
			continue;
		if(_raw[data_end] == '/')
			break;
	}
	L_ASSERT(data_end > 0);
	--data_end;

	L_ASSERT(data_start < _raw.size() && data_end < _raw.size());

	_offset = data_end;
	M_skip_past_closer(_raw, _offset, false);

	return _raw.substr(data_start, data_end - data_start);
}

void MDL_Reader::M_skip_past_opener(const std::string &_raw_data, unsigned int &_offset, bool _ignore_lines) const
{
    bool slash_inside_value = false;

	while(_offset < _raw_data.size())
	{
        std::string line = M_extract_line(_raw_data, _offset);
        _offset += line.size() + 1;

        unsigned int quotes_amount = 0;
        for(unsigned int i = 0; i < line.size(); ++i)
            quotes_amount += (unsigned int)(line[i] == '\"');
        if(quotes_amount % 2 == 1)
            slash_inside_value = !slash_inside_value;

        if(M_line_is_opener(line) && !slash_inside_value)
			return;
		L_ASSERT(_ignore_lines ||  M_line_is_empty(line));
	}
	L_ASSERT(false);	//	opener wasn't found
}

void MDL_Reader::M_skip_past_closer(const std::string &_raw_data, unsigned int &_offset, bool _ignore_lines) const
{
	unsigned int openers = 0;
    bool backslash_inside_value = false;

	while(_offset < _raw_data.size())
	{
		std::string line = M_extract_line(_raw_data, _offset);
		_offset += line.size() + 1;

        unsigned int quotes_amount = 0;
        for(unsigned int i = 0; i < line.size(); ++i)
            quotes_amount += (unsigned int)(line[i] == '\"');
        if(quotes_amount % 2 == 1)
            backslash_inside_value = !backslash_inside_value;

        if(M_line_is_closer(line) && !backslash_inside_value)
		{
			if(openers == 0)
				return;
			else
				--openers;
			continue;
		}

		L_ASSERT(_ignore_lines ||  M_line_is_empty(line));

        if(M_line_is_opener(line) && !backslash_inside_value)
			++openers;
	}
	L_ASSERT(false);	//	closer wasn't found
}

std::string MDL_Reader::M_extract_line(const std::string &_raw, unsigned int _offset) const
{
	unsigned int end = M_find_symbol(_raw, _offset, '\n');
	return _raw.substr(_offset, end - _offset);
}


std::string MDL_Reader::M_parse_name(const std::string &_line) const
{
	unsigned int start = M_find_symbol(_line, 0, '|') + 1;
//	unsigned int end = M_find_symbol(_line, start, '|');
    unsigned int end = M_find_symbol_from_end(_line, _line.size() - 1, '|');

	if(!(start < _line.size() && end < _line.size()))
		return {};

	L_DEBUG_FUNC_NOARG([&]()
	{
		std::string line = _line;
        for(unsigned int i = start-1; i < end + 1; ++i)
			line[i] = ' ';
		L_ASSERT(M_line_is_empty(line));
	});

	return _line.substr(start, end - start);
}

bool MDL_Reader::M_compare_substrings(const std::string& _first, const std::string& _second, unsigned int _offset_1, unsigned int _offset_2, unsigned int _compare_size) const
{
    L_ASSERT(_offset_1 + _compare_size <= _first.size());
    L_ASSERT(_offset_2 + _compare_size <= _second.size());

    for(unsigned int i = 0; i < _compare_size; ++i)
        if(_first[i + _offset_1] != _second[i + _offset_2])
            return false;
    return true;
}

MDL_Variable MDL_Reader::M_parse_stub(const std::string &_raw_data) const
{
	unsigned int offset = 0;

    MDL_Variable result;

	while(offset < _raw_data.size())
	{
		std::string line = M_extract_line(_raw_data, offset);
		offset += line.size() + 1;

		if(M_line_is_empty(line))
			continue;

		std::string name = M_parse_name(line);

        if(name[0] == '|' && name[name.size() - 1] == '|')
        {
            name = M_parse_name(name);

            L_ASSERT(name.size() > 0);
            L_ASSERT(!result.childs().find(name).is_ok());

            std::string raw_child_data = M_extract_variable_data(_raw_data, offset);
            MDL_Variable child = M_parse_stub(raw_child_data);

            result.child_names_order().push(name);
            result.childs().insert((std::string&&)name, (MDL_Variable&&)child);
        }
        else
        {
            std::string simple_data_raw_str = M_extract_variable_data(_raw_data, offset);
            LDS::Vector<std::string> values = M_parse_simple_data(simple_data_raw_str);

            MDL_Variable::Fields_Map::Iterator maybe_field_it = result.fields().find(name);

            if(maybe_field_it.is_ok())
                *maybe_field_it = (LDS::Vector<std::string>&&)values;
            else
                result.fields().insert(name, (LDS::Vector<std::string>&&)values);
        }
	}

	return result;
}

unsigned int MDL_Reader::M_parse_amount(const std::string &_raw_data) const
{
	unsigned int result = 0;
    for(unsigned int i = 0; i < _raw_data.size(); ++i)
	{
		if(_raw_data[i] == '\"')
			++result;
	}

	L_ASSERT(result % 2 == 0);

	return result / 2;
}

LDS::Vector<std::string> MDL_Reader::M_parse_simple_data(const std::string &_raw_data) const
{
	unsigned int amount = M_parse_amount(_raw_data);
	L_ASSERT(amount > 0);

	LDS::Vector<std::string> result;
	result.resize(amount);

	unsigned int offset = 0;
    for(unsigned int i = 0; i < amount; ++i)
	{
		offset = M_find_symbol(_raw_data, offset, '\"') + 1;
		unsigned int end = M_find_symbol(_raw_data, offset, '\"');

		std::string str_value;
		if(end > offset)
			str_value = _raw_data.substr(offset, end - offset);

		result.push(str_value);

		offset = end + 1;
	}

    L_DEBUG_FUNC_NOARG([&]()
    {
        bool quotes_openned = false;

        for(unsigned int i = 0; i < _raw_data.size(); ++i)
        {
            if(_raw_data[i] == '"')
            {
                quotes_openned = !quotes_openned;
                continue;
            }

            if(quotes_openned)
                continue;

            L_ASSERT(_raw_data[i] == ' ' || _raw_data[i] == '\t' || _raw_data[i] == '\n' || _raw_data[i] == '\r');
        }
    });

	return result;
}

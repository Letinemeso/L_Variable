#pragma once

#include <L_Debug/L_Debug.h>

#include <MDL_Variable.h>


namespace LV
{

	class MDL_Reader final
	{
	public:
        using Variable_Map = LDS::Map<std::string, MDL_Variable>;

	private:
        Variable_Map m_predefines;
        Variable_Map m_variables;

	public:
		MDL_Reader();
		~MDL_Reader();

		MDL_Reader(const MDL_Reader& _other) = delete;
		MDL_Reader(MDL_Reader&& _other) = delete;

	public:
        void parse_predefines_file(const std::string& _path);
		void parse_file(const std::string& _path);
        void parse_predefines_source(const std::string& _source);
        void parse_source(const std::string& _source);
        void add_stub(const std::string& _name, const MDL_Variable& _stub);
        void add_stub(const std::string& _name, MDL_Variable&& _stub);
        void clear_predefines();
		void clear();
		void save_to_file(const std::string& _path) const;

	public:
        const Variable_Map& variables() const;
        const MDL_Variable& get_variable(const std::string& _name) const;

    private:
        void M_parse_source(const std::string& _source, Variable_Map& _result_storage);

        void M_save_stub_to_file(std::ofstream& _file, const std::string& _name, const MDL_Variable& _stub, unsigned int _nesting_level = 0) const;

        void M_crop_file_name(std::string& _name) const;

        std::string M_extract_from_file(const std::string& _path) const;
        void M_preprocess_commentaries(std::string& _raw) const;
        void M_preprocess_includes(const std::string& _root_path, std::string& _raw) const;
        void M_preprocess_value_includes(const std::string& _root_path, std::string& _raw) const;
        void M_preprocess_predefines_includes(const std::string& _root_path, std::string& _raw) const;
        void M_preprocess_replacement(const std::string& _root_path, std::string& _raw,
                                      const std::string& _opener, const std::string& _closer,
                                      const LST::Function<std::string(const std::string&, const std::string&)>& _extract_replacement) const;

        void M_apply_template_if_specified(MDL_Variable& _variable) const;

        unsigned int M_find_symbol(const std::string& _str, unsigned int _offset, char _symbol) const;
        unsigned int M_find_symbol_from_end(const std::string& _str, int _offset, char _symbol) const;

		bool M_line_is_empty(const std::string& _line) const;
		bool M_line_is_opener(const std::string& _line) const;
		bool M_line_is_closer(const std::string& _line) const;
		std::string M_extract_variable_data(const std::string& _raw, unsigned int& _offset) const;
		void M_skip_past_opener(const std::string& _raw_data, unsigned int& _offset, bool _ignore_lines = false) const;
		void M_skip_past_closer(const std::string& _raw_data, unsigned int& _offset, bool _ignore_lines = false) const;
		std::string M_extract_line(const std::string& _raw, unsigned int _offset) const;

        std::string M_parse_name(const std::string& _line) const;
        bool M_compare_substrings(const std::string& _first, const std::string& _second, unsigned int _offset_1, unsigned int _offset_2, unsigned int _compare_size) const;
        MDL_Variable M_parse_stub(const std::string& _raw_data) const;
		unsigned int M_parse_amount(const std::string& _raw_data) const;
        LDS::Vector<std::string> M_parse_simple_data(const std::string& _raw_data) const;

	};

}

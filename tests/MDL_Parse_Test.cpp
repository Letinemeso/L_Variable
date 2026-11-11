#include <Testing/Testing.h>

#include <MDL_Variable.h>
#include <MDL_Reader.h>


INIT_TEST(MDL_Parse_Test__Fields)
{
    std::string mdl_source =
R"(

|model|
\
    |type|
    \
        "type_str"
    /

    |values|
    \
        "value_0" "value_1" "value_2"
    /
/

)";

    LV::MDL_Reader reader;
    reader.parse_source(mdl_source);

    const LV::MDL_Reader::Variable_Map& variables = reader.variables();
    EXPECT_EQUAL(variables.size(), 1);

    const LV::MDL_Reader::Variable_Map::Const_Iterator vars_it = variables.find("model");
    EXPECT_EQUAL(vars_it.is_ok(), true);
    if(!vars_it.is_ok())
        return;

    const LV::MDL_Variable& variable = *vars_it;
    EXPECT_EQUAL(variable.fields().size(), 2);

    LV::MDL_Variable::Fields_Map::Const_Iterator fields_it = variable.fields().find("type");
    EXPECT_EQUAL(fields_it.is_ok(), true);
    if(!fields_it.is_ok())
        return;

    const LV::MDL_Variable::Field_Data_Vector& field_vec_0 = *fields_it;
    EXPECT_EQUAL(field_vec_0.size(), 1);
    if(field_vec_0.size() != 1)
        return;
    EXPECT_EQUAL(field_vec_0[0], "type_str");

    fields_it = variable.fields().find("values");
    EXPECT_EQUAL(fields_it.is_ok(), true);
    if(!fields_it.is_ok())
        return;

    const LV::MDL_Variable::Field_Data_Vector& field_vec_1 = *fields_it;
    EXPECT_EQUAL(field_vec_1.size(), 3);
    if(field_vec_1.size() != 3)
        return;
    EXPECT_EQUAL(field_vec_1[0], "value_0");
    EXPECT_EQUAL(field_vec_1[1], "value_1");
    EXPECT_EQUAL(field_vec_1[2], "value_2");
}

INIT_TEST(MDL_Parse_Test__Merge)
{
    std::string mdl_source_template =
        R"(

|model_template|
\
    |type|
    \
        "type_str"
    /

    |replace_values|
    \
        "123"
    /

    ||child||
    \
        |child_field|
        \
            "child_field_text"
        /

        |child_replace_data|
        \
            "33"
        /
    /
/

)";

    std::string mdl_source =
        R"(

|model|
\
    |template|
    \
        "model_template"
    /

    |replace_values|
    \
        "432" "321"
    /

    |additional_values|
    \
        "value_0" "value_1" "value_2"
    /

    ||child||
    \
        |child_replace_data|
        \
            "11" "12"
        /
        |child_additional_data|
        \
            "1112"
        /
    /
/

)";

    LV::MDL_Reader reader;
    reader.parse_predefines_source(mdl_source_template);
    reader.parse_source(mdl_source);

    const LV::MDL_Reader::Variable_Map& variables = reader.variables();
    EXPECT_EQUAL(variables.size(), 1);

    const LV::MDL_Reader::Variable_Map::Const_Iterator vars_it = variables.find("model");
    EXPECT_EQUAL(vars_it.is_ok(), true);
    if(!vars_it.is_ok())
        return;

    const LV::MDL_Variable& variable = *vars_it;

    LV::MDL_Variable::Fields_Map::Const_Iterator fields_it = variable.fields().find("type");
    EXPECT_EQUAL(fields_it.is_ok(), true);
    if(!fields_it.is_ok())
        return;

    const LV::MDL_Variable::Field_Data_Vector& type_vec = *fields_it;
    EXPECT_EQUAL(type_vec.size(), 1);
    if(type_vec.size() != 1)
        return;
    EXPECT_EQUAL(type_vec[0], "type_str");

    fields_it = variable.fields().find("additional_values");
    EXPECT_EQUAL(fields_it.is_ok(), true);
    if(!fields_it.is_ok())
        return;

    const LV::MDL_Variable::Field_Data_Vector& additional_values_vec = *fields_it;
    EXPECT_EQUAL(additional_values_vec.size(), 3);
    if(additional_values_vec.size() != 3)
        return;
    EXPECT_EQUAL(additional_values_vec[0], "value_0");
    EXPECT_EQUAL(additional_values_vec[1], "value_1");
    EXPECT_EQUAL(additional_values_vec[2], "value_2");

    fields_it = variable.fields().find("replace_values");
    EXPECT_EQUAL(fields_it.is_ok(), true);
    if(!fields_it.is_ok())
        return;

    const LV::MDL_Variable::Field_Data_Vector& replace_values_vec = *fields_it;
    EXPECT_EQUAL(replace_values_vec.size(), 2);
    if(replace_values_vec.size() != 2)
        return;
    EXPECT_EQUAL(replace_values_vec[0], "432");
    EXPECT_EQUAL(replace_values_vec[1], "321");


    LV::MDL_Variable::Childs_Map::Const_Iterator child_it = variable.childs().find("child");
    EXPECT_EQUAL(child_it.is_ok(), true);
    if(!child_it.is_ok())
        return;
    const LV::MDL_Variable& child_variable = *child_it;

    fields_it = child_variable.fields().find("child_field");
    EXPECT_EQUAL(fields_it.is_ok(), true);
    if(!fields_it.is_ok())
        return;

    const LV::MDL_Variable::Field_Data_Vector& child_field_vec = *fields_it;
    EXPECT_EQUAL(child_field_vec.size(), 1);
    if(child_field_vec.size() != 1)
        return;
    EXPECT_EQUAL(child_field_vec[0], "child_field_text");

    fields_it = child_variable.fields().find("child_replace_data");
    EXPECT_EQUAL(fields_it.is_ok(), true);
    if(!fields_it.is_ok())
        return;

    const LV::MDL_Variable::Field_Data_Vector& child_replace_data_vec = *fields_it;
    EXPECT_EQUAL(child_replace_data_vec.size(), 2);
    if(child_replace_data_vec.size() != 2)
        return;
    EXPECT_EQUAL(child_replace_data_vec[0], "11");
    EXPECT_EQUAL(child_replace_data_vec[1], "12");

    fields_it = child_variable.fields().find("child_additional_data");
    EXPECT_EQUAL(fields_it.is_ok(), true);
    if(!fields_it.is_ok())
        return;

    const LV::MDL_Variable::Field_Data_Vector& child_additional_data_vec = *fields_it;
    EXPECT_EQUAL(child_additional_data_vec.size(), 1);
    if(child_additional_data_vec.size() != 1)
        return;
    EXPECT_EQUAL(child_additional_data_vec[0], "1112");



}

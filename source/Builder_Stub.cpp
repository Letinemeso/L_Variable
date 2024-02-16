#include <Builder_Stub.h>

using namespace LV;


LV::Variable_Base* Builder_Stub::construct() const
{
    LV::Variable_Base* result = M_construct_product();
    result->assign_values({});
    M_init_constructed_product(result);

    if(m_extract_data_func)
        m_extract_data_func(result);

    return result;
}

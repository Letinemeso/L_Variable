#include <Builder_Stub.h>

using namespace LV;


INIT_FIELDS(LV::Builder_Stub, LV::Variable_Base)
FIELDS_END


Builder_Stub::~Builder_Stub()
{

}

LV::Variable_Base* Builder_Stub::construct() const
{
    LV::Variable_Base* result = M_construct_product();
    result->assign_values({});
    result->on_values_assigned();
    M_init_constructed_product(result);
    return result;
}

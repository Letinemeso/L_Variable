#pragma once

#include <Data_Structures/Vector.h>
#include <Data_Structures/Map.h>


namespace LV
{

    class MDL_Variable
    {
    public:
        using Field_Data_Vector = LDS::Vector<std::string>;
        using Fields_Map = LDS::Map<std::string, Field_Data_Vector>;
        using Childs_Map = LDS::Map<std::string, MDL_Variable>;
        using Child_Names_Order_Vector = LDS::Vector<std::string>;

    private:
        Fields_Map m_fields;
        Childs_Map m_childs;
        Child_Names_Order_Vector m_child_names_order;

    public:
        MDL_Variable();
        MDL_Variable(const MDL_Variable& _other);
        MDL_Variable(MDL_Variable&& _other);
        void operator=(const MDL_Variable& _other);
        void operator=(MDL_Variable&& _other);

    public:
        inline Fields_Map& fields() { return m_fields; }
        inline const Fields_Map& fields() const { return m_fields; }
        inline Childs_Map& childs() { return m_childs; }
        inline const Childs_Map& childs() const { return m_childs; }
        inline Child_Names_Order_Vector& child_names_order() { return m_child_names_order; }
        inline const Child_Names_Order_Vector& child_names_order() const { return m_child_names_order; }

    public:
        void merge_with(const MDL_Variable& _other);
        void merge_with(MDL_Variable&& _other);

    };

}

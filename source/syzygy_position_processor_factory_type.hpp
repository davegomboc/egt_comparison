#if !defined (EGT_COMPARISON_SYZYGY_POSITION_PROCESSOR_FACTORY_TYPE_HPP)
#define EGT_COMPARISON_SYZYGY_POSITION_PROCESSOR_FACTORY_TYPE_HPP


#include <memory>

#include "syzygy_position_processor_enum.hpp"
#include "syzygy_position_processor_type.hpp"


class syzygy_position_processor_factory_type {

public:
    std::unique_ptr<syzygy_position_processor_type> create_ptr(
     syzygy_position_processor_enum processor);
};


#endif // EGT_COMPARISON_SYZYGY_POSITION_PROCESSOR_FACTORY_TYPE_HPP

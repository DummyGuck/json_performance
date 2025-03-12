#pragma once

#include "qvariant.h"
#include "glaze/glaze.hpp"
#include <iostream>
#include <string>

struct fixed_name_object_t {
  std::string name0{};
  std::string name1{};
};

template <>
struct glz::meta<fixed_name_object_t> {
    using T = fixed_name_object_t;
    static constexpr auto value = object(
        &T::name0,
        &T::name1
        );
};

Q_DECLARE_METATYPE(fixed_name_object_t);

Q_DECLARE_METATYPE(std::string);

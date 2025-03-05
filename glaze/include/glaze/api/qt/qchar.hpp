#pragma once

#include <concepts>

#include <QChar>

#include <glaze/core/common.hpp>

template<typename T>
concept qchar_type = std::same_as<T, QChar>;

template <class T>
    requires qchar_type<T>
struct glz::meta<T>
{
    static constexpr auto custom_write = true;
    //static constexpr auto custom_read = true;

    //static constexpr auto value = object("QDateTime", custom<&T::fromStdString, &T::toString>);
};

namespace glz::detail{

template <class T>
    requires qchar_type<T>
struct to<JSON, T> {
    template <auto Opts, class B>
    GLZ_ALWAYS_INLINE static void op(auto&& value, is_context auto&& ctx, B&& b, auto&& ix) noexcept {
        write<Opts.format>::template op<Opts>(value.toLatin1(), ctx, b, ix);
    }
};


template <class T>
    requires qchar_type<T>
struct from<JSON, T> {
    template <glz::opts Opts, class... Args>
    GLZ_ALWAYS_INLINE static void op(auto& value, is_context auto&& ctx, Args&&... args) noexcept
    {
        char c;
        read<Opts.format>::template op<Opts>(c, ctx, args...);
        value = T(c);
    }
};

}


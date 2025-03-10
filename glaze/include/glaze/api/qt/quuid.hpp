#pragma once

#include <concepts>

#include <QString>
#include <QStringView>
#include <QUuid>

#include <glaze/core/common.hpp>

template<typename T>
concept quuidType = std::same_as<T, QUuid>;

template <class T>
    requires quuidType<T>
struct glz::meta<T>
{
    static constexpr auto custom_write = true;
    static constexpr auto custom_read = true;
};

namespace glz::detail {


template <class T>
    requires quuidType<T>
struct to<JSON, T>
{
    template <auto Opts, class B>
    GLZ_ALWAYS_INLINE static void op(auto&& value, is_context auto&& ctx, B&& b, auto&& ix) noexcept {
        //std::string str{value.toString().toStdString()};
        //write<Opts.format>::template op<Opts>(str, ctx, b, ix);
    }
};

template <class T>
    requires quuidType<T>
struct from<JSON, T> {
    template <glz::opts Opts, class... Args>
    GLZ_ALWAYS_INLINE static void op(auto& value, is_context auto&& ctx, Args&&... args) noexcept
    {
        //std::string str;
        //read<Opts.format>::template op<Opts>(str, ctx, args...);
        //value.fromString(QString::fromStdString(str));
    }
};
/*
template <class T>
    requires quuidType<T>
struct to_binary<T> final
{
    template <auto Opts>
    GLZ_ALWAYS_INLINE static void op(auto&& value, is_context auto&& ctx, auto&& b, auto&& ix) noexcept
    {
        write<Opts.format>::template op<Opts>(value.toString(), ctx, b, ix);
    }
};

template <class T>
    requires quuidType<T>
struct from_binary<T> final
{
    template <auto Opts>
    GLZ_ALWAYS_INLINE static void op(auto&& value, is_context auto&& ctx, auto&& it, auto&& end) noexcept
    {
        std::string str;
        read<Opts.format>::template op<Opts>(str, ctx, it, end);
        value(QString::fromStdString(str));
    }
};
*/
}

#pragma once

#include <map>
#include <concepts>

#include <QString>
#include <QStringView>

#include <glaze/core/common.hpp>

template<typename T>
concept qstring_type = std::same_as<T, QString>;

template <qstring_type T>
struct glz::meta<T>
{
    static constexpr auto custom_write = true;
    static constexpr auto custom_read = true;
};

namespace glz::detail {


template <qstring_type T>
struct to<JSON, T>
{
    template <auto Opts, class B>
    GLZ_ALWAYS_INLINE static void op(auto&& value, is_context auto&& ctx, B&& b, auto&& ix) noexcept {
        //std::string str{std::move()};
        write<Opts.format>::template op<Opts>(std::move(value.toStdString()), ctx, b, ix);
    }
};

template <qstring_type T>
struct from<JSON, T> {
    template <auto Opts, class It, class End>
    static void op(auto& value, is_context auto&& ctx, It&& it, End&& end)
    {
        std::string str{};
        read<Opts.format>::template op<Opts>(str, ctx, it, end);
        value = QString::fromStdString(str);
    }
};

/*
template <class T>
    requires qstring_type<T>
struct to_binary<T> final
{
    template <auto Opts>
    GLZ_ALWAYS_INLINE static void op(auto&& value, is_context auto&& ctx, auto&& b, auto&& ix) noexcept
    {
        constexpr uint8_t tag = tag::string;

        dump_type(tag, b, ix);
        const auto n = value.size() * 2;
        dump_compressed_int<Opts>(n, b, ix);

        if (ix + n > b.size()) [[unlikely]] {
            b.resize((std::max)(b.size() * 2, ix + n));
        }

        std::memcpy(b.data() + ix, value.data(), n);
        ix += n;
    }
};

template <class T>
    requires qstring_type<T>
struct from_binary<T> final
{
    template <auto Opts>
    GLZ_ALWAYS_INLINE static void op(auto&& value, is_context auto&& ctx, auto&& it, auto&& end) noexcept
    {
        std::string str;
        read<Opts.format>::template op<Opts>(str, ctx, it, end);
        value = std::move(QString::fromStdString(str));
    }
};
*/
}

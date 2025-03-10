#pragma once

#include <map>
#include <concepts>

#include <QDateTime>

#include <glaze/core/common.hpp>
#include "glaze/core/wrappers.hpp"

template<typename T>
concept qdate_type = std::same_as<T, QDate>;

template<typename T>
concept qtime_type = std::same_as<T, QTime>;

template<typename T>
concept qdatetime_type = std::same_as<T, QDateTime>;


template<typename T>
concept qtimelike = qdatetime_type<T> || qdate_type<T> || qtime_type<T>;

template <qtimelike T>
struct glz::meta<T>
{
    static constexpr auto custom_write = true;
    static constexpr auto custom_read = true;

   // static constexpr auto value = object("QDateTime", custom<&T::fromStdString, &T::toString>);
};

static const QString datetime_format = "yyyy.MM.dd hh:mm:ss.zzz";
static const QString date_format = "yyyy.MM.dd";
static const QString time_format = "hh:mm:ss.zzz";

namespace glz::detail {


template <qdatetime_type T>
struct to<JSON, T>
{
    template <auto Opts, class B>
    GLZ_ALWAYS_INLINE static void op(auto&& value, is_context auto&& ctx, B&& b, auto&& ix) noexcept {
        write<Opts.format>::template op<Opts>(value.toString(datetime_format), ctx, b, ix);
    }
};

template <qdatetime_type T>
struct from<JSON, T> {
    template <glz::opts Opts, class... Args>
    GLZ_ALWAYS_INLINE static void op(auto& value, is_context auto&& ctx, Args&&... args) noexcept
    {
        QString str;
        read<Opts.format>::template op<Opts>(str, ctx, args...);
        value = T::fromString(str, datetime_format);
    }
};

template <qdate_type T>
struct to<JSON, T>
{
    template <auto Opts, class B>
    GLZ_ALWAYS_INLINE static void op(auto&& value, is_context auto&& ctx, B&& b, auto&& ix) noexcept {
        write<Opts.format>::template op<Opts>(value.toString(date_format), ctx, b, ix);
    }
};

template <qdate_type T>
struct from<JSON, T> {
    template <glz::opts Opts, class... Args>
    GLZ_ALWAYS_INLINE static void op(auto& value, is_context auto&& ctx, Args&&... args) noexcept
    {
        QString str;
        read<Opts.format>::template op<Opts>(str, ctx, args...);
        value = T::fromString(str, date_format);
    }
};


template <qtime_type T>
struct to<JSON, T>
{
    template <auto Opts, class B>
    GLZ_ALWAYS_INLINE static void op(auto&& value, is_context auto&& ctx, B&& b, auto&& ix) noexcept {
        write<Opts.format>::template op<Opts>(value.toString(time_format), ctx, b, ix);
    }
};

template <qtime_type T>
struct from<JSON, T> {
    template <glz::opts Opts, class... Args>
    GLZ_ALWAYS_INLINE static void op(auto& value, is_context auto&& ctx, Args&&... args) noexcept
    {
        QString str;
        read<Opts.format>::template op<Opts>(str, ctx, args...);
        value = T::fromString(str, time_format);
    }
};

}
/*
template <class T>
    requires qdate_type<T> || qtime_type<T>
struct to_binary<T> final {
    template <auto Opts>
    GLZ_ALWAYS_INLINE static void op(auto&& value, is_context auto&& ctx, auto&& b, auto&& ix) noexcept {
        write<Opts.format>::template op<Opts>(value.toString(), ctx, b, ix);
    }
};

template <class T>
    requires qdate_type<T> || qtime_type<T>
struct from_binary<T> final {
    template <auto Opts>
    GLZ_ALWAYS_INLINE static void op(auto&& value, is_context auto&& ctx, auto&& it, auto&& end) noexcept {
        QString str;
        read<Opts.format>::template op<Opts>(str, ctx, it, end);
        value = T::fromString(str);
    }
};

template <class T>
    requires qdatetime_type<T>
struct to_binary<T>
{
    template <auto Opts, class B>
    GLZ_ALWAYS_INLINE static void op(auto&& value, is_context auto&& ctx, B&& b, auto&& ix) noexcept {
        write<Opts.format>::template op<Opts>(value.toMSecsSinceEpoch(), ctx, b, ix);
    }
};

template <class T>
    requires qdatetime_type<T>
struct from_binary<T> {
    template <glz::opts Opts, class... Args>
    GLZ_ALWAYS_INLINE static void op(auto& value, is_context auto&& ctx, Args&&... args) noexcept
    {
        qint64 msec;
        read<Opts.format>::template op<Opts>(msec, ctx, args...);
        value = std::move(QDateTime::fromMSecsSinceEpoch(msec));
    }
};
}
*/

#pragma once

#include <concepts>
#include <glaze/core/reflect.hpp>

#include <QVariant>
#include <QDataStream>
#include <QDebug>
#include <QBuffer>

#include <glaze/core/common.hpp>
#include <glaze/json/read.hpp>

#include <glaze/api/qt/qchar.hpp>
#include <glaze/api/qt/qdatetime.hpp>
#include <glaze/api/qt/qstring.hpp>
#include <glaze/api/qt/quuid.hpp>

template<typename T>
concept qvariantType = std::same_as<T, QVariant>;

template <class T>
    requires qvariantType<T>
struct glz::meta<T>
{
    static constexpr auto custom_write = true;
    static constexpr auto custom_read = true;
};

namespace glz::detail {


template <qvariantType T>
struct to<JSON, T>
{
    template <auto Opts, class VariantType>
    static void tojson(VariantType&& val, int type, is_context auto&& ctx, auto&& b, auto&& ix) {
        dump(std::to_string(static_cast<int>(type)), b, ix);
        dump<'@'>(b, ix);
        to<JSON, VariantType>::template op<opening_handled<Opts>()>(val, ctx, b, ix);
    }

    template <auto Opts, class... Args>
    static void op(auto&& value, is_context auto&& ctx, auto&& b, auto&& ix)
    {
        switch (value.type()) {
        case QVariant::Bool:
            tojson<Opts, bool>(value.template value<bool>(), QVariant::Bool, ctx, b, ix);
            break;
        case QVariant::Int:
            tojson<Opts, int>(value.template value<int>(), QVariant::Int, ctx, b, ix);
            break;
        case QVariant::UInt:
            tojson<Opts, uint>(value.template value<uint>(), QVariant::UInt, ctx, b, ix);
            break;
        case QVariant::Double:
            tojson<Opts, double>(value.template value<double>(), QVariant::Double, ctx, b, ix);
            break;
        case QVariant::Char:
            tojson<Opts, QChar>(value.template value<QChar>(), QVariant::Char, ctx, b, ix);
            break;
        case QVariant::String:
            tojson<Opts, QString>(value.template value<QString>(), QVariant::String, ctx, b, ix);
            break;
        case QVariant::Date:
            tojson<Opts, QDate>(value.template value<QDate>(), QVariant::Date, ctx, b, ix);
            break;
        case QVariant::Time:
            tojson<Opts, QTime>(value.template value<QTime>(), QVariant::Time, ctx, b, ix);
            break;
        case QVariant::DateTime:
            tojson<Opts, QDateTime>(value.template value<QDateTime>(), QVariant::DateTime, ctx, b, ix);
            break;
        case QVariant::Uuid:
            tojson<Opts, QUuid>(value.template value<QUuid>(), QVariant::Uuid, ctx, b, ix);
            break;
        case QVariant::UserType: {
            dump(std::to_string(static_cast<int>(value.userType())), b, ix);
            dump<'@'>(b, ix);
            dump(std::move(value.template value<std::string>()), b, ix);
            break;
        }
        default:
            qDebug() << "Invalid or not supported QVariant type: " << value.type();
            break;
        }
    }
};

template <class T>
    requires qvariantType<T>
struct from<JSON, T> {
    template <auto Opts, class... Args>
    static void fromjson(int type_info, auto&& value, is_context auto&& ctx, auto&& it, auto&& end)
    {
        switch (type_info) {
        case QVariant::Bool:{
            bool val;
            from<JSON, bool>::template op<Opts>(val, ctx, it, end);
            value = val;
            break;
        }
        case QVariant::Int: {
            int val;
            from<JSON, int>::template op<Opts>(val, ctx, it, end);
            value = val;
            break;
        }
        case QVariant::UInt: {
            uint val;
            from<JSON, uint>::template op<Opts>(val, ctx, it, end);
            value = val;
            break;
        }
        case QVariant::Double: {
            double val;
            from<JSON, double>::template op<Opts>(val, ctx, it, end);
            value = val;
            break;
        }
        case QVariant::Char: {
            QChar val;
            from<JSON, QChar>::template op<Opts>(val, ctx, it, end);
            value = val;
            break;
        }
        case QVariant::String: {
            QString val;
            from<JSON, QString>::template op<Opts>(val, ctx, it, end);
            value = val;
            break;
        }
        case QVariant::Date: {
            QDate val;
            from<JSON, QDate>::template op<Opts>(val, ctx, it, end);
            value = val;
            break;
        }
        case QVariant::Time: {
            QTime val;
            from<JSON, QTime>::template op<Opts>(val, ctx, it, end);
            value = val;
            break;
        }
        case QVariant::DateTime: {
            QDateTime val;
            from<JSON, QDateTime>::template op<Opts>(val, ctx, it, end);
            value = val;
            break;
        }
        case QVariant::Uuid: {
            QUuid val;
            from<JSON, QUuid>::template op<Opts>(val, ctx, it, end);
            value = val;
            break;
        }
        default:{
            auto start = it;
            GLZ_MATCH_OPEN_BRACE;
            if(ctx.error != error_code::expected_brace) {
                ctx.error = error_code::syntax_error;
            }
            int openBraces = 1;
            bool escaped = false;
            while(openBraces != 0) {
                if(it == end) {
                    ctx.error = error_code::syntax_error;
                    return;
                }
                if(!escaped && *it == '\\') {
                    escaped = true;
                }
                else {
                    if(!escaped && *it == '{')
                        ++openBraces;
                    if(!escaped && *it == '}')
                        --openBraces;
                    escaped = false;
                }
                ++it;
            }
            ctx.error = error_code::none;
            value.setValue(std::string{start, it});
            if(!value.convert(type_info)) {
                ctx.error = error_code::no_matching_variant_type;
                break;
            }
        }
        }
    }

    template <auto Options>
    static void op(auto&& value, is_context auto&& ctx, auto&& it, auto&& end)
    {
        constexpr auto Opts = ws_handled_off<Options>();
        if constexpr (not has_ws_handled(Options)) {
            GLZ_SKIP_WS();
        }
        auto tistart = it;
        while(*it != '@' && *it != '\0') {
            if(*it == '\0') [[unlikely]] {
                ctx.error = error_code::unexpected_end;
                return;
            }
            ++it;
        }
        ++it;

        int type_info;
        auto ti = std::from_chars(tistart, it, type_info);
        if(ti.ec == std::errc::invalid_argument) {
            ctx.error = error_code::no_matching_variant_type;
        }
        GLZ_SKIP_WS();
        auto start = it;
        fromjson<Opts>(type_info, value, ctx, it, end);
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

#pragma once

#include <concepts>

#include "glaze/core/opts.hpp"
//#include "glaze/core/refl.hpp"
#include "glaze/json/ptr.hpp"
#include "glaze/util/wdump.hpp"

#include <QMap>

#include "glaze/core/meta.hpp"

template<typename T>
concept qmap_type = std::same_as<T, QMap<typename T::key_type, typename T::mapped_type>>;

template <class T>
    requires qmap_type<T>
struct glz::meta<T>
{
    static constexpr auto custom_write = true;
    static constexpr auto custom_read = true;
};
namespace glz {
template <class T>
    requires qmap_type<T>
struct reflect<T>
{
    static constexpr auto size = 0;
};

namespace detail {
template <class T>
    requires qmap_type<T>
struct to<JSON, T>
{
    template <glz::opts Opts, class... Args>
        requires(!Opts.concatenate)
    GLZ_ALWAYS_INLINE static void op(auto&& value, is_context auto&& ctx, Args&&... args) noexcept
    {
        write_array_to_json<Opts>(value, ctx, args...);
    }

    template <glz::opts Opts, class... Args>
        requires(!!Opts.concatenate)
    GLZ_ALWAYS_INLINE static void op(auto&& value, is_context auto&& ctx, Args&&... args) noexcept
    {
        if constexpr (!glz::has_opening_handled(Opts)) {
            dump<'{'>(args...);
        }

        if (!empty_range(value)) {
            if constexpr (!glz::has_opening_handled(Opts)) {
                if constexpr (Opts.prettify) {
                    ctx.indentation_level += Opts.indentation_width;
                    dump<'\n'>(args...);
                    dumpn<Opts.indentation_char>(ctx.indentation_level, args...);
                }
            }

            auto write_first_entry = [&ctx, &args...](auto&& it) {
                if (skip_member<Opts>(it.value())) {
                    return true;
                }
                write_pair_content<Opts>(it.key(), it.value(), ctx, args...);
                return false;
            };

            auto it = value.begin();
            [[maybe_unused]] bool starting = write_first_entry(it);
            for (++it; it != value.end(); ++it) {
                if (skip_member<Opts>(it.value())) {
                    continue;
                }

                if constexpr (Opts.skip_null_members) {
                    if (!starting) {
                        write_array_entry_separator<Opts>(ctx, args...);
                    }
                }
                else {
                    write_array_entry_separator<Opts>(ctx, args...);
                }

                write_pair_content<Opts>(it.key(), it.value(), ctx, args...);
                starting = false;
            }

            if constexpr (!glz::has_closing_handled(Opts)) {
                if constexpr (Opts.prettify) {
                    ctx.indentation_level -= Opts.indentation_width;
                    dump<'\n'>(args...);
                    dumpn<Opts.indentation_char>(ctx.indentation_level, args...);
                }
            }
        }

        if constexpr (!glz::has_closing_handled(Opts)) {
            dump<'}'>(args...);
        }
    }
};

template <class T>
    requires qmap_type<T>
struct from<JSON, T> {
    template <auto Options>
    static void op(auto&& value, is_context auto&& ctx, auto&& it, auto&& end)
    {
        static constexpr auto num_members = reflect<T>::size;
        static constexpr auto Opts = opening_handled_off<ws_handled_off<Options>()>();
        if constexpr (!has_opening_handled(Options)) {
            if constexpr (!has_ws_handled(Options)) {
                GLZ_SKIP_WS();
            }
            GLZ_MATCH_OPEN_BRACE;
            GLZ_INVALID_END();
            GLZ_ADD_LEVEL;
        }
        const auto ws_start = it;
        GLZ_SKIP_WS();
        const size_t ws_size = size_t(it - ws_start);

        decltype(auto) fields = [&]() -> decltype(auto) {
            return nullptr;
        }();

        size_t read_count{}; // for partial_read

        bool first = true;
        while (true) {
            if (*it == '}') {
                ++it;
                GLZ_VALID_END();
                return;
            }
            else if (first) {
                first = false;
            }
            else {
                GLZ_MATCH_COMMA;
                GLZ_INVALID_END();

                if constexpr ((not Opts.minified) && (num_members > 1 || not Opts.error_on_unknown_keys)) {
                    if (ws_size && ws_size < size_t(end - it)) {
                        skip_matching_ws(ws_start, it, ws_size);
                    }
                }

                GLZ_SKIP_WS();
            }

            // For types like std::map, std::unordered_map

            auto reading = [&](auto&& key) {
                if constexpr (Opts.partial_read) {
                    if (auto element = value.find(key); element != value.end()) {
                        ++read_count;
                        read<JSON>::op<ws_handled<Opts>()>(element->second, ctx, it, end);
                    }
                    else {
                        skip_value<JSON>::op<Opts>(ctx, it, end);
                    }
                }
                else {
                    read<JSON>::op<ws_handled<Opts>()>(value[key], ctx, it, end);
                }
            };

            // using Key = std::conditional_t<heterogeneous_map<T>, sv, typename T::key_type>;
            using Key = typename T::key_type;
            if constexpr (std::is_same_v<Key, std::string>) {
                static thread_local Key key;
                read<JSON>::op<Opts>(key, ctx, it, end);
                if (bool(ctx.error)) [[unlikely]]
                    return;

                GLZ_PARSE_WS_COLON;

                reading(key);
                if constexpr (Opts.partial_read) {
                    if (read_count == value.size()) {
                        return;
                    }
                }
                if (bool(ctx.error)) [[unlikely]]
                    return;
            }
            else if constexpr (str_t<Key>) {
                Key key;
                read<JSON>::op<Opts>(key, ctx, it, end);
                if (bool(ctx.error)) [[unlikely]]
                    return;

                GLZ_PARSE_WS_COLON;

                reading(key);
                if constexpr (Opts.partial_read) {
                    if (read_count == value.size()) {
                        return;
                    }
                }
                if (bool(ctx.error)) [[unlikely]]
                    return;
            }
            else {
                Key key_value{};
                if constexpr (glaze_enum_t<Key>) {
                    read<JSON>::op<Opts>(key_value, ctx, it, end);
                }
                else if constexpr (std::is_arithmetic_v<Key>) {
                    // prefer over quoted_t below to avoid double parsing of quoted_t
                    read<JSON>::op<opt_true<Opts, &opts::quoted_num>>(key_value, ctx, it, end);
                }
                else {
                    read<JSON>::op<opt_false<Opts, &opts::raw_string>>(quoted_t<Key>{key_value}, ctx, it, end);
                }
                if (bool(ctx.error)) [[unlikely]]
                    return;

                GLZ_PARSE_WS_COLON;

                reading(key_value);
                if constexpr (Opts.partial_read) {
                    if (read_count == value.size()) {
                        return;
                    }
                }
                if (bool(ctx.error)) [[unlikely]]
                    return;
            }
            GLZ_SKIP_WS();
        }

    }
};
/*
template <class T>
    requires qmap_type<T>
struct to_binary<T> final
{
    template <auto Opts, class... Args>
    GLZ_FLATTEN static auto op(auto&& value, is_context auto&& ctx, Args&&... args) noexcept
    {
        using Key = typename T::key_type;

        constexpr uint8_t type = str_t<Key> ? 0 : (std::is_signed_v<Key> ? 0b000'01'000 : 0b000'10'000);
        constexpr uint8_t byte_cnt = str_t<Key> ? 0 : byte_count<Key>;
        constexpr uint8_t tag = tag::object | type | (byte_cnt << 5);
        dump_type(tag, args...);

        dump_compressed_int<Opts>(value.size(), args...);
        for (auto&& it = value.keyValueBegin(); it != value.keyValueEnd(); ++it ) {
            write<binary>::no_header<Opts>(it->first, ctx, args...);
            write<binary>::op<Opts>(it->second, ctx, args...);
        }
    }
};

template <class T>
    requires qmap_type<T>
struct from_binary<T> {
    template <auto Opts, class... Args>
    GLZ_ALWAYS_INLINE static void op(auto& value, is_context auto&& ctx, auto&& it, auto&& end) noexcept
    {
        using Key = typename T::key_type;

        constexpr uint8_t type = str_t<Key> ? 0 : (std::is_signed_v<Key> ? 0b000'01'000 : 0b000'10'000);
        constexpr uint8_t byte_cnt = str_t<Key> ? 0 : byte_count<Key>;
        constexpr uint8_t header = tag::object | type | (byte_cnt << 5);

        const auto tag = uint8_t(*it);
        if (tag != header) [[unlikely]] {
            ctx.error = error_code::syntax_error;
            return;
        }

        ++it;

        const auto n = int_from_compressed(it, end);

        if constexpr (std::is_arithmetic_v<std::decay_t<Key>>) {
            Key key;
            for (size_t i = 0; i < n; ++i) {
                read<binary>::op<opt_true<Opts, opts::internal::no_header>>(key, ctx, it, end);
                read<binary>::op<Opts>(value[key], ctx, it, end);
            }
        }
        else {
            static thread_local Key key;
            for (size_t i = 0; i < n; ++i) {
                read<binary>::op<opt_true<Opts, opts::internal::no_header>>(key, ctx, it, end);
                read<binary>::op<Opts>(value[key], ctx, it, end);
            }
        }
    }
};
*/
}
}

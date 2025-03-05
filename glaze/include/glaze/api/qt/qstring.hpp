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
        std::string str{value.toStdString()};
        write<Opts.format>::template op<Opts>(std::move(str), ctx, b, ix);
    }
};

template <qstring_type T>
struct from<JSON, T> {
    template <auto Opts, class It, class End>
        requires(has_is_padded(Opts))
    static void op(auto& value, is_context auto&& ctx, It&& it, End&& end)
    {
        if constexpr (Opts.number) {
            auto start = it;
            skip_number<Opts>(ctx, it, end);
            if (bool(ctx.error)) [[unlikely]] {
                return;
            }
            value.append(start, size_t(it - start));
        }
        else {
            if constexpr (!has_opening_handled(Opts)) {
                if constexpr (!has_ws_handled(Opts)) {
                    GLZ_SKIP_WS();
                }

                GLZ_MATCH_QUOTE;
                GLZ_INVALID_END();
            }

            if constexpr (not Opts.raw_string) {
                static constexpr auto string_padding_bytes = 8;

                auto start = it;
                while (true) {
                    if (it >= end) [[unlikely]] {
                        ctx.error = error_code::unexpected_end;
                        return;
                    }

                    uint64_t chunk;
                    std::memcpy(&chunk, it, 8);
                    const uint64_t test_chars = has_quote(chunk);
                    if (test_chars) {
                        it += (countr_zero(test_chars) >> 3);

                        auto* prev = it - 1;
                        while (*prev == '\\') {
                            --prev;
                        }
                        if (size_t(it - prev) % 2) {
                            break;
                        }
                        ++it; // skip the escaped quote
                    }
                    else {
                        it += 8;
                    }
                }

                auto n = size_t(it - start);
                value.resize(n + string_padding_bytes);

                auto* p = value.data();

                while (true) {
                    if (start >= it) {
                        break;
                    }

                    std::memcpy(p, start, 8);
                    uint64_t swar;
                    std::memcpy(&swar, p, 8);

                    constexpr uint64_t lo7_mask = repeat_byte8(0b01111111);
                    const uint64_t lo7 = swar & lo7_mask;
                    const uint64_t backslash = (lo7 ^ repeat_byte8('\\')) + lo7_mask;
                    const uint64_t less_32 = (swar & repeat_byte8(0b01100000)) + lo7_mask;
                    uint64_t next = ~((backslash & less_32) | swar);

                    next &= repeat_byte8(0b10000000);
                    if (next == 0) {
                        start += 8;
                        p += 8;
                        continue;
                    }

                    next = countr_zero(next) >> 3;
                    start += next;
                    if (start >= it) {
                        break;
                    }

                    if ((*start & 0b11100000) == 0) [[unlikely]] {
                        ctx.error = error_code::syntax_error;
                        return;
                    }
                    ++start; // skip the escape
                    if (*start == 'u') {
                        ++start;
                        p += next;
                        const auto mark = start;
                        const auto offset = handle_unicode_code_point(start, p, end);
                        if (offset == 0) [[unlikely]] {
                            ctx.error = error_code::unicode_escape_conversion_failure;
                            return;
                        }
                        n += offset;
                        // escape + u + unicode code points
                        n -= 2 + uint32_t(start - mark);
                    }
                    else {
                        p += next;
                        *p = char_unescape_table[uint8_t(*start)];
                        if (*p == 0) [[unlikely]] {
                            ctx.error = error_code::invalid_escape;
                            return;
                        }
                        ++p;
                        ++start;
                        --n;
                    }
                }

                value.resize(n);
                ++it;
            }
            else {
                // raw_string
                auto start = it;
                skip_string_view<Opts>(ctx, it, end);
                if (bool(ctx.error)) [[unlikely]]
                    return;

                value.assign(start, size_t(it - start));
                ++it;
            }
        }
    }

    template <auto Opts, class It, class End>
        requires(not has_is_padded(Opts))
    static void op(auto& value, is_context auto&& ctx, It&& it, End&& end)
    {
        if constexpr (Opts.number) {
            auto start = it;
            skip_number<Opts>(ctx, it, end);
            if (bool(ctx.error)) [[unlikely]] {
                return;
            }
            if (start == it) [[unlikely]] {
                ctx.error = error_code::parse_number_failure;
                return;
            }
            value.append(start, size_t(it - start));
        }
        else {
            if constexpr (!has_opening_handled(Opts)) {
                if constexpr (!has_ws_handled(Opts)) {
                    GLZ_SKIP_WS();
                }

                GLZ_MATCH_QUOTE;
                GLZ_INVALID_END();
            }

            if constexpr (not Opts.raw_string) {
                static constexpr auto string_padding_bytes = 8;

                if (size_t(end - it) >= 8) {
                    auto start = it;
                    const auto end8 = end - 8;
                    while (true) {
                        if (it >= end8) [[unlikely]] {
                            break;
                        }

                        uint64_t chunk;
                        std::memcpy(&chunk, it, 8);
                        const uint64_t test_chars = has_quote(chunk);
                        if (test_chars) {
                            it += (countr_zero(test_chars) >> 3);

                            auto* prev = it - 1;
                            while (*prev == '\\') {
                                --prev;
                            }
                            if (size_t(it - prev) % 2) {
                                goto continue_decode;
                            }
                            ++it; // skip the escaped quote
                        }
                        else {
                            it += 8;
                        }
                    }

                    while (it[-1] == '\\') [[unlikely]] {
                        // if we ended on an escape character then we need to rewind
                        // because we lost our context
                        --it;
                    }

                    for (; it < end; ++it) {
                        if (*it == '"') {
                            auto* prev = it - 1;
                            while (*prev == '\\') {
                                --prev;
                            }
                            if (size_t(it - prev) % 2) {
                                goto continue_decode;
                            }
                        }
                    }

                    ctx.error = error_code::unexpected_end;
                    return;

                continue_decode:

                    const auto available_padding = size_t(end - it);
                    auto n = size_t(it - start);
                    if (available_padding >= 8) [[likely]] {
                        value.resize(n + string_padding_bytes);

                        auto* p = value.data();

                        while (true) {
                            if (start >= it) {
                                break;
                            }

                            std::memcpy(p, start, 8);
                            uint64_t swar;
                            std::memcpy(&swar, p, 8);

                            constexpr uint64_t lo7_mask = repeat_byte8(0b01111111);
                            const uint64_t lo7 = swar & lo7_mask;
                            const uint64_t backslash = (lo7 ^ repeat_byte8('\\')) + lo7_mask;
                            const uint64_t less_32 = (swar & repeat_byte8(0b01100000)) + lo7_mask;
                            uint64_t next = ~((backslash & less_32) | swar);

                            next &= repeat_byte8(0b10000000);
                            if (next == 0) {
                                start += 8;
                                p += 8;
                                continue;
                            }

                            next = countr_zero(next) >> 3;
                            start += next;
                            if (start >= it) {
                                break;
                            }

                            if ((*start & 0b11100000) == 0) [[unlikely]] {
                                ctx.error = error_code::syntax_error;
                                return;
                            }
                            ++start; // skip the escape
                            if (*start == 'u') {
                                ++start;
                                p += next;
                                const auto mark = start;
                                const auto offset = handle_unicode_code_point(start, p, end);
                                if (offset == 0) [[unlikely]] {
                                    ctx.error = error_code::unicode_escape_conversion_failure;
                                    return;
                                }
                                n += offset;
                                // escape + u + unicode code points
                                n -= 2 + uint32_t(start - mark);
                            }
                            else {
                                p += next;
                                *p = char_unescape_table[uint8_t(*start)];
                                if (*p == 0) [[unlikely]] {
                                    ctx.error = error_code::invalid_escape;
                                    return;
                                }
                                ++p;
                                ++start;
                                --n;
                            }
                        }

                        value.resize(n);
                        ++it;
                    }
                    else {
                        // For large inputs this case of running out of buffer is very rare
                        value.resize(n);
                        auto* p = value.data();

                        it = start;
                        while (it < end) [[likely]] {
                            *p = *it;
                            if (*it == '"') {
                                value.resize(size_t(p - value.data()));
                                ++it;
                                return;
                            }
                            else if (*it == '\\') {
                                ++it; // skip the escape
                                if (*it == 'u') {
                                    ++it;
                                    if (!handle_unicode_code_point(it, p, end)) [[unlikely]] {
                                        ctx.error = error_code::unicode_escape_conversion_failure;
                                        return;
                                    }
                                }
                                else {
                                    *p = char_unescape_table[uint8_t(*it)];
                                    if (*p == 0) [[unlikely]] {
                                        ctx.error = error_code::invalid_escape;
                                        return;
                                    }
                                    ++p;
                                    ++it;
                                }
                            }
                            else {
                                ++it;
                                ++p;
                            }
                        }

                        ctx.error = error_code::unexpected_end;
                    }
                }
                else {
                    // For short strings

                    std::array<char, 8> buffer{};

                    auto* p = buffer.data();

                    while (it < end) [[likely]] {
                        *p = *it;
                        if (*it == '"') {
                            value.assign(buffer.data(), size_t(p - buffer.data()));
                            ++it;
                            GLZ_VALID_END();
                            return;
                        }
                        else if (*it == '\\') {
                            ++it; // skip the escape
                            GLZ_INVALID_END();
                            if (*it == 'u') {
                                ++it;
                                GLZ_INVALID_END();
                                if (!handle_unicode_code_point(it, p, end)) [[unlikely]] {
                                    ctx.error = error_code::unicode_escape_conversion_failure;
                                    return;
                                }
                            }
                            else {
                                *p = char_unescape_table[uint8_t(*it)];
                                if (*p == 0) [[unlikely]] {
                                    ctx.error = error_code::invalid_escape;
                                    return;
                                }
                                ++p;
                                ++it;
                            }
                        }
                        else {
                            ++it;
                            ++p;
                        }
                    }

                    ctx.error = error_code::unexpected_end;
                }
            }
            else {
                // raw_string
                auto start = it;
                skip_string_view<Opts>(ctx, it, end);
                if (bool(ctx.error)) [[unlikely]]
                    return;

                value.assign(start, size_t(it - start));
                ++it;
            }
        }
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

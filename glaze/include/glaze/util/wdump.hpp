// Glaze Library
// For the license information refer to glaze.hpp

#pragma once

#include <bit>
#include <cstddef>
#include <cstring>
#include <glaze/util/inline.hpp>
#include <glaze/util/string_literal.hpp>
#include <span>
#include <string_view>

#include "glaze/concepts/container_concepts.hpp"
#include "glaze/core/opts.hpp"
#include "glaze/util/convert.hpp"

namespace glz::detail
{
template <class T, class V = std::remove_cvref_t<T>>
concept qchar_sized = sizeof(T) == 2;

template <uint32_t N, is_wide_buffer B>
GLZ_ALWAYS_INLINE void maybe_pad(B& b, size_t ix) noexcept(not vector_like<B>)
{
    if constexpr (vector_like<B>) {
        if (const auto k = ix + N; k > b.size()) [[unlikely]] {
            b.resize(2 * k);
        }
    }
}

template <is_wide_buffer B>
GLZ_ALWAYS_INLINE void maybe_pad(const size_t n, B& b, size_t ix) noexcept(not vector_like<B>)
{
    if constexpr (vector_like<B>) {
        if (const auto k = ix + n; k > b.size()) [[unlikely]] {
            b.resize(2 * k);
        }
    }
}

template <bool Checked = true, is_wide_buffer B>
GLZ_ALWAYS_INLINE void dump(const qchar_sized auto c, B& b, size_t& ix) noexcept(not vector_like<B>)
{
    if constexpr (Checked && vector_like<B>) {
        if (ix == b.size()) [[unlikely]] {
            b.resize(b.size() == 0 ? 128 : b.size() * 2);
        }
    }
    assign_maybe_cast(c, b, ix);
    ++ix;
}

template <auto c, bool Checked = true, is_wide_buffer B>
GLZ_ALWAYS_INLINE void dump(B& b, size_t& ix) noexcept(not vector_like<B>)
{
    if constexpr (Checked && vector_like<B>) {
        if (ix == b.size()) [[unlikely]] {
            b.resize(b.size() == 0 ? 128 : b.size() * 2);
        }
    }
    assign_maybe_cast<c>(b, ix);
    ++ix;
}

template <string_literal str, bool Checked = true, is_wide_buffer B>
GLZ_ALWAYS_INLINE void dump(B& b, size_t& ix) noexcept(not vector_like<B>)
{
    static constexpr auto s = str.sv();
    static constexpr auto n = s.size();

    if constexpr (vector_like<B>) {
        if constexpr (Checked) {
            const auto k = ix + n;
            if (k > b.size()) [[unlikely]] {
                b.resize(2 * k);
            }
        }
    }
    std::wmemcpy(&b[ix], s.data(), n);
    ix += n;
}

template <bool Checked = true, is_wide_buffer B>
GLZ_ALWAYS_INLINE void dump(const sv str, B& b, size_t& ix) noexcept(not vector_like<B>)
{
    const auto n = str.size();
    if constexpr (vector_like<B>) {
        if constexpr (Checked) {
            const auto k = ix + n;
            if (ix + n > b.size()) [[unlikely]] {
                b.resize(2 * k);
            }
        }
    }
    std::wmemcpy(&b[ix], str.data(), n);
    ix += n;
}

template <auto c, is_wide_buffer B>
GLZ_ALWAYS_INLINE void dumpn(size_t n, B& b, size_t& ix) noexcept(not vector_like<B>)
{
    if constexpr (vector_like<B>) {
        const auto k = ix + n;
        if (k > b.size()) [[unlikely]] {
            b.resize(2 * k);
        }
    }
    std::wmemset(&b[ix], c, n);
    ix += n;
}

template <auto c, is_wide_buffer B>
GLZ_ALWAYS_INLINE void dumpn_unchecked(size_t n, B& b, size_t& ix) noexcept
{
    std::wmemset(&b[ix], c, n);
    ix += n;
}

template <char IndentChar, is_wide_buffer B>
GLZ_ALWAYS_INLINE void dump_newline_indent(size_t n, B& b, size_t& ix) noexcept(not vector_like<B>)
{
    if constexpr (vector_like<B>) {
        if (const auto k = ix + n + write_padding_bytes; k > b.size()) [[unlikely]] {
            b.resize(2 * k);
        }
    }

    assign_maybe_cast<'\n'>(b, ix);
    ++ix;
    std::wmemset(&b[ix], IndentChar, n);
    ix += n;
}

template <const sv& str, bool Checked = true, is_wide_buffer B>
GLZ_ALWAYS_INLINE void dump(B& b, size_t& ix) noexcept(not vector_like<B> && not Checked)
{
    static constexpr auto s = str;
    static constexpr auto n = s.size();

    if constexpr (vector_like<B>) {
        if constexpr (Checked) {
            const auto k = ix + n;
            if (k > b.size()) [[unlikely]] {
                b.resize(2 * k);
            }
        }
    }
    std::wmemcpy(&b[ix], s.data(), n);
    ix += n;
}

template <bool Checked = true, is_wide_buffer B>
GLZ_ALWAYS_INLINE void dump_not_empty(const sv str, B& b, size_t& ix) noexcept(not vector_like<B> && not Checked)
{
    const auto n = str.size();
    if constexpr (vector_like<B>) {
        if constexpr (Checked) {
            const auto k = ix + n;
            if (k > b.size()) [[unlikely]] {
                b.resize(2 * k);
            }
        }
    }
    std::wmemcpy(&b[ix], str.data(), n);
    ix += n;
}

template <bool Checked = true, is_wide_buffer B>
GLZ_ALWAYS_INLINE void dump_maybe_empty(const sv str, B& b, size_t& ix) noexcept(not vector_like<B> && not Checked)
{
    const auto n = str.size();
    if (n) {
        if constexpr (vector_like<B>) {
            if constexpr (Checked) {
                const auto k = ix + n;
                if (k > b.size()) [[unlikely]] {
                    b.resize(2 * k);
                }
            }
        }
        std::wmemcpy(&b[ix], str.data(), n);
        ix += n;
    }
}

template <is_wide_buffer B>
GLZ_ALWAYS_INLINE void dump(const vector_like auto& bytes, B& b, size_t& ix) noexcept(not vector_like<B>)
{
    const auto n = bytes.size();
    if constexpr (vector_like<B>) {
        const auto k = ix + n;
        if (k > b.size()) [[unlikely]] {
            b.resize(2 * k);
        }
    }
    std::wmemcpy(&b[ix], bytes.data(), n);
    ix += n;
}

template <size_t N, is_wide_buffer B>
GLZ_ALWAYS_INLINE void dump(const std::array<uint8_t, N>& bytes, B& b, size_t& ix) noexcept(not vector_like<B>)
{
    if constexpr (vector_like<B>) {
        const auto k = ix + N;
        if (k > b.size()) [[unlikely]] {
            b.resize(2 * k);
        }
    }
    std::wmemcpy(&b[ix], bytes.data(), N);
    ix += N;
}
}

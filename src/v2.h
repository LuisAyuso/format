//
//  v2.h
//  format
//
//  Created by Luis Ayuso on 11/02/17.
//  Copyright © 2017 Luis Ayuso. All rights reserved.
//
#pragma once

#include <tuple>


namespace format{
namespace strutils{


    template <size_t N>
    constexpr size_t len(const char (&arr)[N]  ){
        return N;
    }

    constexpr size_t in_range(size_t i, size_t n){
        return i<n;
    }

    template <size_t N>
    constexpr size_t nth_char(const char (&arr)[N], size_t i){
        return !in_range(i,N)? '\0': arr[i];
    }

} // namespace strutils


    enum class SpecifierKind{
        // signed
        Dec,
        // unsinged
        Oct,
        UInt,
        Hex, 
        // doubles
        Exp,
        Float,
        // char
        Char,
        Str,
        Ptr,
        //
        FieldLen,
        // 
        None
    };

    constexpr bool is_format_char(char c){
        return c == 'h'? true:
               c == 'l'? true:
               c == 'L'? true:
               c == 'q'? true:
               c == 'j'? true:
               c == 'z'? true:
               c == 't'? true:
               c == '.'? true:
               c == ' '? true:
               c == '+'? true:
               c == '-'? true:
               c == '\''? true:
               c == 'I'? true:

               c == '0'? true:
               c == '1'? true:
               c == '2'? true:
               c == '3'? true:
               c == '4'? true:
               c == '5'? true:
               c == '6'? true:
               c == '7'? true:
               c == '8'? true:
               c == '9'? true:
               false;
    }

    template <size_t N>
    constexpr SpecifierKind get_specifier_after(const char (&arr)[N], size_t i){
        using sk = SpecifierKind;
        using namespace strutils;
        return is_format_char(nth_char(arr,i+1))? get_specifier_after(arr, i+1):  
                nth_char(arr, i+1) == 'd'? sk::Dec:
                nth_char(arr, i+1) == 'i'? sk::Dec:

                nth_char(arr, i+1) == 'o'? sk::Oct:
                nth_char(arr, i+1) == 'u'? sk::UInt:
                nth_char(arr, i+1) == 'x'? sk::Hex:
                nth_char(arr, i+1) == 'X'? sk::Hex:

                nth_char(arr, i+1) == 'e'? sk::Exp:
                nth_char(arr, i+1) == 'E'? sk::Exp:
                nth_char(arr, i+1) == 'f'? sk::Float:
                nth_char(arr, i+1) == 'F'? sk::Float:
                nth_char(arr, i+1) == 'g'? sk::Float:
                nth_char(arr, i+1) == 'G'? sk::Float:
                nth_char(arr, i+1) == 'a'? sk::Float:
                nth_char(arr, i+1) == 'A'? sk::Float:

                nth_char(arr, i+1) == 'c'? sk::Char:
                nth_char(arr, i+1) == 's'? sk::Str:
                nth_char(arr, i+1) == 'p'? sk::Ptr:

                nth_char(arr, i+1) == '*'? sk::FieldLen:
                    sk::None;
    }

    template <size_t N>
    constexpr SpecifierKind get_specifier(const char (&arr)[N], size_t i, SpecifierKind current = SpecifierKind::None){
        using sk = SpecifierKind;
        using namespace strutils;
        return current == sk::FieldLen ||  nth_char(arr, i) == '%'? 
                    get_specifier_after(arr, i):
                    sk::None;

    }

// ===============================================================
//     code to generate the validation
// ===============================================================

    template<typename T>
    void fail(T){
        static_assert(std::is_same<T, int>::value, "");
    }

    template<size_t I>
    using Idx = std::integral_constant<size_t, I>;

    template <size_t N, size_t I=0>
    struct indices{
        using type = decltype( std::tuple_cat(std::tuple<Idx<I>>(), indices<N,I+1>::type() ));
    };

    template <size_t N>
    struct indices<N,N>{
        using type = std::tuple<>;
    };

    template<SpecifierKind K>
    using Specifier = std::integral_constant<SpecifierKind, K>;

    template<size_t N>
    constexpr static decltype(auto) get_sp(const char (&arr)[N], size_t i){
        return Specifier<SpecifierKind::None>{};
    }
    
    template<size_t FS, size_t AS, typename ARGS, size_t I=0, size_t J=0>
    struct validate_aux{
        template<size_t N>
        constexpr static decltype(auto) fmt(const char (&arr)[N]) {
            return 
                // here, do the magic
                // ...
                validate_aux<N, AS, ARGS, I+1, J>::fmt(arr);
        }
    };

    template<size_t FS, size_t AS, typename ARGS, size_t J>
    struct validate_aux<FS,AS,ARGS, FS, J>{
        template<size_t N>
        constexpr static bool fmt(const char (&arr)[N]) {
            return true;
        }
    };

    template<typename ARGS>
    struct validate{
        template<size_t N>
        constexpr static bool fmt(const char (&arr)[N]) {
            return validate_aux<N, std::tuple_size<ARGS>::value, ARGS>::fmt(arr);
        }
    };


} // namespace format

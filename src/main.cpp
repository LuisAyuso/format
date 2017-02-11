//
//  main.cpp
//  format
//
//  Created by Luis Ayuso on 16/02/15.
//  Copyright (c) 2015 Luis Ayuso. All rights reserved.
//

#include <iostream>

#include "v2.h"

using namespace format;

int main(int argc, const char * argv[]) {
    
    {
        using namespace format::strutils;
        static_assert(len("hola") == 5, "len check");

        static_assert(in_range(1,2), "range check");
        static_assert(!in_range(2,2), "range check");
        static_assert(!in_range(3,2), "range check");

        static_assert(nth_char("hola",0) == 'h', "nth char");
        static_assert(nth_char("hola",1) == 'o', "nth char");
        static_assert(nth_char("hola",2) == 'l', "nth char");
        static_assert(nth_char("hola",3) == 'a', "nth char");
        static_assert(nth_char("hola",5) == '\0', "nth char");
        static_assert(nth_char("hola",6) == '\0', "nth char");
        static_assert(nth_char("hola",1006) == '\0', "nth char");
    }

    {
        using sk = SpecifierKind;
        static_assert(get_specifier("%d",0) == sk::Dec, "SpecifierKind");
        static_assert(get_specifier("%i",0) == sk::Dec, "SpecifierKind");

        static_assert(get_specifier("%o",0) == sk::Oct, "SpecifierKind");
        static_assert(get_specifier("%u",0) == sk::UInt, "SpecifierKind");
        static_assert(get_specifier("%x",0) == sk::Hex, "SpecifierKind");
        static_assert(get_specifier("%X",0) == sk::Hex, "SpecifierKind");

        static_assert(get_specifier("%e",0) == sk::Exp, "SpecifierKind");
        static_assert(get_specifier("%E",0) == sk::Exp, "SpecifierKind");
        static_assert(get_specifier("%f",0) == sk::Float, "SpecifierKind");
        static_assert(get_specifier("%F",0) == sk::Float, "SpecifierKind");
        static_assert(get_specifier("%g",0) == sk::Float, "SpecifierKind");
        static_assert(get_specifier("%G",0) == sk::Float, "SpecifierKind");
        static_assert(get_specifier("%a",0) == sk::Float, "SpecifierKind");
        static_assert(get_specifier("%A",0) == sk::Float, "SpecifierKind");

        static_assert(get_specifier("%c",0) == sk::Char, "SpecifierKind");
        static_assert(get_specifier("%s",0) == sk::Str, "SpecifierKind");
        static_assert(get_specifier("%p",0) == sk::Ptr, "SpecifierKind");

        static_assert(get_specifier("%*",0) == sk::FieldLen, "SpecifierKind");
    
        // false
        static_assert(get_specifier("%%",0) == sk::None, "SpecifierKind");
        static_assert(get_specifier("",0) == sk::None, "SpecifierKind");
        static_assert(get_specifier("-",0) == sk::None, "SpecifierKind");
        static_assert(get_specifier("a",0) == sk::None, "SpecifierKind");
        static_assert(get_specifier("%j",0) == sk::None, "SpecifierKind");
    }

    // field length
    {
        using sk = SpecifierKind;
        static_assert(get_specifier("%*d",0) == sk::FieldLen, "FieldLen");
        static_assert(get_specifier_after("%*d",1) == sk::Dec, "FieldLen");
    }

    // format
    {
        using sk = SpecifierKind;
        static_assert(is_format_char('0'), "skip format");
        static_assert(get_specifier("%0d",0) == sk::Dec, "skip format");
        static_assert(get_specifier("%0ld",0) == sk::Dec, "skip format");
        static_assert(get_specifier("%*0ld",0) == sk::FieldLen, "skip format");
        static_assert(get_specifier_after("%*0ld",1) == sk::Dec, "skip format");
        static_assert(get_specifier_after("%*.*d",1) == sk::FieldLen, "skip format");
        static_assert(get_specifier_after("%*.*d",3) == sk::Dec, "skip format");
    }

    return 0;
}

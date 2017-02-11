//
//  v1.h
//  format
//
//  Created by Luis Ayuso on 17/02/15.
//  Copyright (c) 2015 Luis Ayuso. All rights reserved.
//

#ifndef format_v1_h
#define format_v1_h

namespace {
    
    template<unsigned N> constexpr
    const char nth_char(unsigned i, const char (&arr)[N]){
        return (i < N)? arr[i]: throw -1;
    }
    
    /**
     * denotes end of recursion
     */
    constexpr bool done() {
        return true;
    }
    constexpr bool error() {
        return false;
    }
    
    /**
     * forward declaration
     */
    template<unsigned Token, unsigned N, typename ...ARGS> constexpr
    bool validate(const char(&arr)[N], ARGS...args);
    
    template<unsigned Token, unsigned N, typename ...ARGS> constexpr
    bool check_field(const char(&arr)[N], int x, ARGS...args);
    
    template<unsigned Token, unsigned N, typename ...ARGS> constexpr
    bool check_field(const char(&arr)[N], float x, ARGS...args);
    
    template<unsigned Token, unsigned N, typename ...ARGS> constexpr
    bool check_field(const char(&arr)[N], double x, ARGS...args);
    
    template<unsigned Token, unsigned N, typename ...ARGS> constexpr
    bool check_field(const char(&arr)[N], const char *x, ARGS...args);
    
    template<unsigned Token, unsigned N> constexpr
    bool check_field(const char(&arr)[N]);
    
    
    /**
     * dispatcher, function templates do not accept partial specialisation
     */
    template <unsigned Token, unsigned N>
    struct next{
        static_assert(Token < N, "this dispatcher handles recursive case");
        
        template<typename ...ARGS>
        bool advance(const char(&arr)[N], ARGS...args){
            return validate<Token>(arr, args...);
        }
        template<typename ...ARGS>
        bool consume(const char(&arr)[N], ARGS...args){
            return check_field<Token>(arr, args...);
        }
    };
    
    /**
     * dispatcher, base case
     */
    template <unsigned N>
    struct next<N, N>{
        template<typename ...ARGS>
        bool advance(const char(&arr)[N], ARGS...args){
            return done();
        }
        template<typename ...ARGS>
        bool consume(const char(&arr)[N], ARGS...args){
            return error();
        }
    };
    
    template<unsigned Token, unsigned N> constexpr
    bool check_field(const char(&arr)[N]){
        return false;
    }
    template<unsigned Token, unsigned N, typename ...ARGS> constexpr
    bool check_field(const char(&arr)[N], int x, ARGS...args){
        return  (nth_char(Token, arr) == 'd')? validate<Token>(arr, args...): false;
    }
    template<unsigned Token, unsigned N, typename ...ARGS> constexpr
    bool check_field(const char(&arr)[N], float x, ARGS...args){
        return  (nth_char(Token, arr) == 'f')? validate<Token>(arr, args...): false;
    }
    template<unsigned Token, unsigned N, typename ...ARGS> constexpr
    bool check_field(const char(&arr)[N], double x, ARGS...args){
        return  (nth_char(Token, arr) == 'f')? validate<Token>(arr, args...): false;
    }
    
    template<unsigned Token, unsigned N, typename ...ARGS> constexpr
    bool check_field(const char(&arr)[N], const char *x, ARGS...args){
        return  (nth_char(Token, arr) == 's')? validate<Token>(arr, args...): false;
    }
    
    
    /**
     * validate recursive function
     */
    template<unsigned Token, unsigned N, typename ...ARGS> constexpr
    bool validate(const char(&arr)[N], ARGS...args) {
        return (nth_char(Token, arr) == '%')?
            next<Token+1, N>().consume(arr, args...):
            next<Token+1, N>().advance(arr, args...);
    }
    
    
    /**
     * wrapper object
     */
    template<unsigned N>
    class format_t{
        const char (&cad)[N];
        static_assert(N < 256, "strings must be smaller than 265 chars, compiler does not handle more");
    public:
        format_t(const char (&c)[N])
        :cad(c)
        {  }
        
        template<typename ...ARGS>
        void operator()(ARGS...args){
            validate<0>(cad, args...);
        }
    };
    
    
    template<unsigned N>
    format_t<N> format(const char (&c)[N]){
        return format_t<N>(c);
    }
    
}

#endif

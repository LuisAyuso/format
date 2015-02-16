//
//  main.cpp
//  format
//
//  Created by Luis Ayuso on 16/02/15.
//  Copyright (c) 2015 Luis Ayuso. All rights reserved.
//

#include <iostream>

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
    template<unsigned Token, unsigned N, typename ...ARGS>
    bool validate(const char(&arr)[N], ARGS...args);
    
    template<unsigned Token, unsigned N> constexpr
    bool check_field(const char(&arr)[N], int x);
    
    template<unsigned Token, unsigned N, typename ...ARGS> constexpr
    bool check_field(const char(&arr)[N], int x, ARGS...args);
  
    /**
     * dispatcher, function templates do not accept partial spetialisation
     */
    template <unsigned Token, unsigned N>
    struct next{
        template<typename ...ARGS>
        bool advance(const char(&arr)[N], ARGS...args){
            return validate<Token>(arr, args...);
        }
        template<typename ...ARGS>
        bool consume(const char(&arr)[N], ARGS...args){
            return check_field<Token>(arr, args...);
        }
    };
   
    template<unsigned Token, unsigned N> constexpr
    bool check_field(const char(&arr)[N], int x){
        return  (nth_char(Token, arr) == 'd') && next<Token+1,N>().advance(arr);
    }
    
    template<unsigned Token, unsigned N, typename ...ARGS> constexpr
    bool check_field(const char(&arr)[N], int x, ARGS...args){
        return  (nth_char(Token, arr) == 'd') && next<Token+1,N>().advance(arr, args...);
    }

    
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

    /**
     * validate recursive function
     */
    template<unsigned Token, unsigned N, typename ...ARGS>
    bool validate(const char(&arr)[N], ARGS...args) {
        static_assert(Token < N, "out of range, something went wrong on the dispatcher");
        
        
        std::cout << Token << " " << N << " " << nth_char(Token, arr) << std::endl;
        if (nth_char(Token, arr) == '%'){
            return next<Token+1, N>().consume(arr, args...);
        }
        else{
            return next<Token+1, N>().advance(arr, args...);
        }
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
            std::cout << validate<0>(cad, 0, args...) << std::endl;
        }
    };
    
    
    template<unsigned N>
    format_t<N> format(const char (&c)[N]){
        return format_t<N>(c);
    }
    
}


int main(int argc, const char * argv[]) {
    format("hello world")(6);
}

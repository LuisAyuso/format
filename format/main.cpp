//
//  main.cpp
//  format
//
//  Created by Luis Ayuso on 16/02/15.
//  Copyright (c) 2015 Luis Ayuso. All rights reserved.
//

#include <iostream>

#include "v1.h"


int main(int argc, const char * argv[]) {
    format("%d %d %d %f %s")(2, 3, 4, 5.5, "a");
    
}

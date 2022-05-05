#include "../io/interface.h"
#include <stdio.h>


#define IO_TYPE "console"

int main() {
    set_to_api_mode();
    print_hello();
    return 0;
}

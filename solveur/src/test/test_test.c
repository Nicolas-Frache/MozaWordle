#include "snow.h"

describe(test_elementaire) {
    it("breaks the rules of math (to demonstrate failed tests)") {
        assert(1 == 2, "Oh noes!");
    }
    it("It's working !") {
        assert(1 == 1, "Oh -yes!");
    }
}
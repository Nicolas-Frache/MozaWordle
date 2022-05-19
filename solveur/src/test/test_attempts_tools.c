#include "snow.h"
#include "../solver/attempts_tools.h"

describe(attempts_tools) {
    list_attempts_t *attempts;

    before_each() {
        attempts = create_list_attempts();
    }
    after_each() {
        destroy_list_attempts(attempts);
    }

    it("attempts_creation") {
        asserteq(attempts->nb_tries, 0, "Empty list should have 0 elements");
    }
}
#include "snow.h"
#include "../dict/dict.h"
#define UNUSED(x) (void)(x)

describe(attempts_tools) {
    list_attempts_t *attempts;
    int results_1[] = {11, 12, 13, 14, 15};
    int results_2[] = {2, 2, 2, 2, 2};

    before_each() {
        attempts = create_list_attempts();
    }
    after_each() {
        destroy_list_attempts(attempts);
    }

    it("creation") {
        asserteq(attempts->nb_tries, 0, "Empty list should have 0 elements");
        asserteq(attempts->head, NULL, "Empty list should have no first element");
    }
}

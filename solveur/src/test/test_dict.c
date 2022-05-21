#include "snow.h"
#include "../dict/dict.h"
#define UNUSED(x) (void)(x)

void reset_attempts(list_attempts_t *attempts){
    destroy_list_attempts(attempts);
    attempts = create_list_attempts();
}

describe(Dict) {
    list_attempts_t *attempts;
    int r_all_match_2_2_2[] = {2, 2, 2};
    int r_none_match_0_0_0[] = {0, 0, 0};
    int r_wrong_place_1_0_0[] = {1, 0, 0};
    int r_wrong_place_0_0_1[] = {0, 0, 1};

    before_each() {
        attempts = create_list_attempts();
    }
    after_each() {
        destroy_list_attempts(attempts);
    }

    subdesc(match){
        it("match with no attempt") {
            asserteq(is_matching_word_specific_attempts("hello", attempts), true, "any string should match with no attempt")
            ;asserteq(is_matching_word_specific_attempts("", attempts), true, "empty string should match with no attempt")
            ;
        }
        it("match with one attempt") {
            append_attempt(attempts, "abc", r_all_match_2_2_2);
            asserteq(is_matching_word_specific_attempts("abc", attempts), true, "should match")
            ;asserteq(is_matching_word_specific_attempts("aba", attempts), false, "should not match")
            ;asserteq(is_matching_word_specific_attempts("zzz", attempts), false, "should not match")
            ;
            reset_attempts(attempts);
            append_attempt(attempts, "abc", r_wrong_place_1_0_0);
            asserteq(is_matching_word_specific_attempts("azz", attempts), false, "should not match")
            ;asserteq(is_matching_word_specific_attempts("zaz", attempts), true, "should match")
            ;asserteq(is_matching_word_specific_attempts("zza", attempts), true, "should match")
            ;asserteq(is_matching_word_specific_attempts("zzz", attempts), false, "should not match")
            ;
            reset_attempts(attempts);
            append_attempt(attempts, "aba", r_wrong_place_1_0_0);
            asserteq(is_matching_word_specific_attempts("zza", attempts), false, "should not match")
            ;asserteq(is_matching_word_specific_attempts("zaz", attempts), true, "should match")
            ;asserteq(is_matching_word_specific_attempts("zzz", attempts), false, "should not match")
            ;
            reset_attempts(attempts);
            append_attempt(attempts, "abc", r_none_match_0_0_0);
            asserteq(is_matching_word_specific_attempts("zzz", attempts), true, "should match")
            ;asserteq(is_matching_word_specific_attempts("azz", attempts), false, "should not match")
            ;asserteq(is_matching_word_specific_attempts("bzz", attempts), false, "should not match")
            ;asserteq(is_matching_word_specific_attempts("czz", attempts), false, "should not match")
            ;
        }
        it("match with two attempts"){
            append_attempt(attempts, "abb", r_wrong_place_1_0_0);
            append_attempt(attempts, "bba", r_wrong_place_0_0_1);
            asserteq(is_matching_word_specific_attempts("zza", attempts), false, "should not match")
            ;asserteq(is_matching_word_specific_attempts("azz", attempts), false, "should not match")
            ;asserteq(is_matching_word_specific_attempts("zaz", attempts), true, "should match")
            ;asserteq(is_matching_word_specific_attempts("zzz", attempts), false, "should not match")
            ;
        }
    }
}






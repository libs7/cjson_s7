#include "gopt.h"
#include "log.h"
#include "unity.h"
#include "utarray.h"
#include "utstring.h"
#include "config.h"

#include "common.h"

#include "libs7.h"

s7_scheme *s7;

extern struct option options[];

s7_pointer root, jo, k, keys, ja, idx;
s7_pointer flag, res, actual, expected;
s7_pointer len, m, map, v, vec;

char *expected_str;

bool verbose;
bool debug;

char *cmd;

#define APPLY_MAP(m, o) \
    s7_apply_function(s7, m, s7_list(s7, 1, o))

void setUp(void) {
    /* log_info("setup"); */
}

void tearDown(void) {
    /* log_info("teardown"); */
}

/* (define tlt (json:read "v = [0, 1, 2]")) */
/* tlt is (a nameless) object, NOT a kv pair!  */
void work(void) {
    jo = JSON_READ("{ \"t\": { \"i\": 1, \"s\": \"Hello\"}}");
    actual = APPLY_1("json:map?", jo);
    TEST_ASSERT_EQUAL(s7_t(s7), actual);
    k = s7_make_string(s7, "x");
    flag = APPLY_2("json:map-ref", jo, k);
    actual = APPLY_1("json:map?", flag);
    TEST_ASSERT_EQUAL(s7_t(s7), actual);

    /* jo = JSON_READ("{\"m\": true}"); */
    /* actual = APPLY_1("json:map?", jo); */
    /* TEST_ASSERT_EQUAL(s7_t(s7), actual); */

    /* k = s7_make_string(s7, "m"); */
    /* flag = APPLY_2("json:map-ref", jo, k); */
    /* actual = APPLY_1("boolean?", flag); */
    /* TEST_ASSERT_EQUAL(s7_t(s7), actual); */

}

int main(int argc, char **argv)
{
    s7 = initialize("work", argc, argv);

    libs7_load_clib(s7, "cjson");

    UNITY_BEGIN();

    RUN_TEST(work);

    return UNITY_END();
    s7_quit(s7);
    s7_free(s7);
}

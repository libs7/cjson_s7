#include "gopt.h"
#include "liblogc.h"
#include "unity.h"
#include "utarray.h"
#include "utstring.h"

#include "libs7.h"

#include "macros.h"
#include "s7plugin_test_config.h"

#include "json_map_to_scm_test.h"

#if defined(PROFILE_fastbuild)
#define TRACE_FLAG  cjson_s7_trace
#define DEBUG_LEVEL cjson_s7_debug
extern bool    TRACE_FLAG;
extern int     DEBUG_LEVEL;

#define S7_DEBUG_LEVEL libs7_debug
extern int  libs7_debug;
extern bool s7plugin_trace;
extern int  s7plugin_debug;
#endif

s7_scheme *s7;

extern struct option options[];

s7_pointer flag, t, tt, k, a, suba, ht;
s7_pointer idx, i0, i1, i3;
s7_pointer subt, subt1, subt2;
s7_pointer sexp, expected, actual;
s7_pointer expected_s7str, actual_s7str;
s7_pointer res, len, m;
s7_pointer tmp, tmp1, tmp2;

char *expected_str, *actual_str, *tmp_str;
char *cmd, *sexp_str;

bool verbose;
bool debug;

void setUp(void) {}
void tearDown(void) {}

/* WARNING WARNING: we convert string keys to s7 keywords! */

void map_to_ht_ints(void)
{
    tt = JSON_READ("{\"fld1\": 1, \"fld2\": 2}");
    ht = APPLY_1("json:map->hash-table", tt);
    flag = APPLY_1("hash-table?", ht);
    LOG_S7_DEBUG(0, "ht:", ht);
    TEST_ASSERT_TRUE(s7_boolean(s7, flag));
    flag = APPLY_1("c-pointer?", ht);
    TEST_ASSERT_FALSE(s7_boolean(s7, flag));

    sexp_str = "(hash-table :fld1 1 :fld2 2)";
    expected = EVAL(sexp_str);
    flag = APPLY_1("hash-table?", expected);
    TEST_ASSERT_TRUE(s7_boolean(s7, flag));

    flag = s7_apply_function(s7, s7_name_to_value(s7, "equal?"),
                             s7_list(s7, 2, expected, ht));
    TEST_ASSERT_TRUE(s7_boolean(s7, flag));
}

void map_to_ht_reals(void)
{
    tt = JSON_READ("{\"fld1\": 1.1, \"fld2\": 2.2}");
    ht = APPLY_1("json:map->hash-table", tt);
    flag = APPLY_1("hash-table?", ht);
    LOG_S7_DEBUG(0, "ht:", ht);
    TEST_ASSERT_TRUE(s7_boolean(s7, flag));
    flag = APPLY_1("c-pointer?", ht);
    TEST_ASSERT_FALSE(s7_boolean(s7, flag));

    sexp_str = "(hash-table :fld1 1.1 :fld2 2.2)";
    expected = EVAL(sexp_str);
    flag = APPLY_1("hash-table?", expected);
    TEST_ASSERT_TRUE(s7_boolean(s7, flag));

    flag = s7_apply_function(s7, s7_name_to_value(s7, "equal?"),
                             s7_list(s7, 2, expected, ht));
    TEST_ASSERT_TRUE(s7_boolean(s7, flag));
}

void map_to_ht_bools(void)
{
    tt = JSON_READ("{\"fld1\": true, \"fld2\": false}");
    ht = APPLY_1("json:map->hash-table", tt);
    flag = APPLY_1("hash-table?", ht);
    LOG_S7_DEBUG(0, "ht:", ht);
    TEST_ASSERT_TRUE(s7_boolean(s7, flag));
    flag = APPLY_1("c-pointer?", ht);
    TEST_ASSERT_FALSE(s7_boolean(s7, flag));

    // NB: #f value removes key from ht
    // lookup of missing key returns #f
    // but (keys ht) will not list the key

    sexp_str = "(hash-table :fld1 #t)";
    expected = EVAL(sexp_str);
    flag = APPLY_1("hash-table?", expected);
    TEST_ASSERT_TRUE(s7_boolean(s7, flag));

    flag = s7_apply_function(s7, s7_name_to_value(s7, "equal?"),
                             s7_list(s7, 2, expected, ht));
    TEST_ASSERT_TRUE(s7_boolean(s7, flag));
}

void map_to_ht_nulls(void)
{
    tt = JSON_READ("{\"fld1\": true, \"fld2\": null}");
    ht = APPLY_1("json:map->hash-table", tt);
    flag = APPLY_1("hash-table?", ht);
    LOG_S7_DEBUG(0, "ht:", ht);
    TEST_ASSERT_TRUE(s7_boolean(s7, flag));
    flag = APPLY_1("c-pointer?", ht);
    TEST_ASSERT_FALSE(s7_boolean(s7, flag));

    // NB: #f value removes key from ht
    // lookup of missing key returns #f
    // but (keys ht) will not list the key

    sexp_str = "(hash-table :fld1 #t :fld2 ())";
    expected = EVAL(sexp_str);
    flag = APPLY_1("hash-table?", expected);
    TEST_ASSERT_TRUE(s7_boolean(s7, flag));

    flag = s7_apply_function(s7, s7_name_to_value(s7, "equal?"),
                             s7_list(s7, 2, expected, ht));
    TEST_ASSERT_TRUE(s7_boolean(s7, flag));
}

void map_to_ht_strings(void)
{
    tt = JSON_READ("{\"fld1\": \"hello\", \"fld2\": \"world\"}");
    ht = APPLY_1("json:map->hash-table", tt);
    flag = APPLY_1("hash-table?", ht);
    LOG_S7_DEBUG(0, "ht:", ht);
    TEST_ASSERT_TRUE(s7_boolean(s7, flag));
    flag = APPLY_1("c-pointer?", ht);
    TEST_ASSERT_FALSE(s7_boolean(s7, flag));

    sexp_str = "(hash-table :fld1 \"hello\" :fld2 \"world\")";
    expected = EVAL(sexp_str);
    flag = APPLY_1("hash-table?", expected);
    TEST_ASSERT_TRUE(s7_boolean(s7, flag));

    flag = s7_apply_function(s7, s7_name_to_value(s7, "equal?"),
                             s7_list(s7, 2, expected, ht));
    TEST_ASSERT_TRUE(s7_boolean(s7, flag));
}

void nested_table_to_hash_table(void)
{
    tt = JSON_READ("{\"a\": {\"b\": 0}}");
    ht = APPLY_1("json:map->hash-table", tt);
    LOG_S7_DEBUG(0, "ht:", ht);
    flag = APPLY_1("hash-table?", ht);
    TEST_ASSERT_TRUE(s7_boolean(s7, flag));
    flag = APPLY_1("c-pointer?", ht);
    TEST_ASSERT_FALSE(s7_boolean(s7, flag));

    sexp_str = "(hash-table :a (hash-table :b 0))";
    expected = EVAL(sexp_str);
    LOG_S7_DEBUG(0, "expected:", expected);
    flag = APPLY_1("hash-table?", expected);
    TEST_ASSERT_TRUE(s7_boolean(s7, flag));

    flag = s7_apply_function(s7, s7_name_to_value(s7, "equal?"),
                             s7_list(s7, 2, expected, ht));
    TEST_ASSERT_TRUE(s7_boolean(s7, flag));

    res = APPLY_1("object->string", ht);
    LOG_S7_DEBUG(0, "res:", res);
    TEST_ASSERT_EQUAL_STRING(sexp_str, s7_string(res));

    /* // verify value at "a" of toml-table (not ht) is a toml-table */
    k = s7_make_string(s7, "a");
    subt = APPLY_2("json:map-ref", tt, k);
    LOG_S7_DEBUG(0, "subt:", subt);
    flag = APPLY_1("json:map?", subt);
    TEST_ASSERT_TRUE(s7_boolean(s7, flag));

    // verify value at "a" of hash-table is a hash-table
    k = s7_make_keyword(s7, "a");
    subt = APPLY_2("hash-table-ref", ht, k);
    flag = APPLY_1("hash-table?", subt);
    TEST_ASSERT_TRUE(s7_boolean(s7, flag));

    // verify value at "b" of "a" hash-table is 0
    k = s7_make_keyword(s7, "b");
    res = APPLY_2("hash-table-ref", subt, k);
    flag = APPLY_1("number?", res);
    TEST_ASSERT_TRUE(s7_boolean(s7, flag));
    TEST_ASSERT_EQUAL_INT(0, s7_integer(res));
}

int main(int argc, char **argv)
{
    s7 = s7_plugin_initialize("interpolation", argc, argv);

    libs7_load_plugin(s7, "cjson");

    UNITY_BEGIN();

    RUN_TEST(map_to_ht_ints);
    RUN_TEST(map_to_ht_reals);
    RUN_TEST(map_to_ht_bools);
    RUN_TEST(map_to_ht_nulls);
    RUN_TEST(map_to_ht_strings);
    RUN_TEST(nested_table_to_hash_table);

    return UNITY_END();
    s7_quit(s7);
    s7_free(s7);
}

/*
 *  zc - zip crack library
 *  Copyright (C) 2017 Marc Ferland
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <check.h>
#include <stdlib.h>

#include "libzc.h"

START_TEST(test_zc_crk_ptext_find_password_0)
{
    char pw[14];
    struct zc_key internal_rep = { .key0 = 0x12345678, .key1 = 0x23456789, .key2 = 0x34567890 };
    ck_assert_int_eq(zc_crk_ptext_find_password(&internal_rep, pw, sizeof(pw)), 0);
}
END_TEST

START_TEST(test_zc_crk_ptext_find_password_1)
{
    char pw[14];
    struct zc_key internal_rep = { .key0 = 0x64799c96, .key1 = 0xb303049c, .key2 = 0xa253270a };
    ck_assert_int_eq(zc_crk_ptext_find_password(&internal_rep, pw, sizeof(pw)), 1);
    ck_assert_str_eq(pw, "a");
}
END_TEST

START_TEST(test_zc_crk_ptext_find_password_2)
{
    char pw[14];
    struct zc_key internal_rep = { .key0 = 0x23bd1e23, .key1 = 0x2b7993bc, .key2 = 0x4ccb4379 };
    ck_assert_int_eq(zc_crk_ptext_find_password(&internal_rep, pw, sizeof(pw)), 2);
    ck_assert_str_eq(pw, "aa");
}
END_TEST

START_TEST(test_zc_crk_ptext_find_password_3)
{
    char pw[14];
    struct zc_key internal_rep = { .key0 = 0x98f19da2, .key1 = 0x1cd05dd7, .key2 = 0x3d945e94 };
    ck_assert_int_eq(zc_crk_ptext_find_password(&internal_rep, pw, sizeof(pw)), 3);
    ck_assert_str_eq(pw, "aaa");
}
END_TEST

START_TEST(test_zc_crk_ptext_find_password_4)
{
    char pw[14];
    struct zc_key internal_rep = { .key0 = 0x2f56297, .key1 = 0x64329027, .key2 = 0xbd806642 };
    ck_assert_int_eq(zc_crk_ptext_find_password(&internal_rep, pw, sizeof(pw)), 4);
    ck_assert_str_eq(pw, "aaaa");
}
END_TEST

START_TEST(test_zc_crk_ptext_find_password_5)
{
    char pw[14];
    struct zc_key internal_rep = { .key0 = 0x54dca24b, .key1 = 0x1b079a3b, .key2 = 0x120a6936 };
    ck_assert_int_eq(zc_crk_ptext_find_password(&internal_rep, pw, sizeof(pw)), 5);
    ck_assert_str_eq(pw, "aaaaa");
}
END_TEST

Suite *plaintext_password_suite()
{
    Suite *s = suite_create("plaintext_password");

    TCase *tc_core = tcase_create("Core");
    tcase_add_test(tc_core, test_zc_crk_ptext_find_password_0);
    tcase_add_test(tc_core, test_zc_crk_ptext_find_password_1);
    tcase_add_test(tc_core, test_zc_crk_ptext_find_password_2);
    tcase_add_test(tc_core, test_zc_crk_ptext_find_password_3);
    tcase_add_test(tc_core, test_zc_crk_ptext_find_password_4);
    /* tcase_add_test(tc_core, test_zc_crk_ptext_find_password_5); */
    tcase_set_timeout(tc_core, 60);
    suite_add_tcase(s, tc_core);

    return s;
}

int main()
{
    int number_failed;
    Suite *s;
    SRunner *sr;

    s = plaintext_password_suite();
    sr = srunner_create(s);

    srunner_run_all(sr, CK_NORMAL);
    number_failed = srunner_ntests_failed(sr);
    srunner_free(sr);

    return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}

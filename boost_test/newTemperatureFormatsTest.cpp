/*
 * Copyright 2015 BrewPi/Elco Jacobs.
 *
 * This file is part of BrewPi.
 *
 * BrewPi is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * BrewPi is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with BrewPi.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <boost/test/unit_test.hpp>

#include "newTemperatureFormats.h"
#include <cstdio>

BOOST_AUTO_TEST_SUITE( temperature_suite )

BOOST_AUTO_TEST_CASE( temperature_test )
{
    temp_diff x(3.0);
    temp_diff y(3.0);
    temp_diff z(9.0);

    BOOST_CHECK_EQUAL(x*y, z);

    temp t(0.0);
    temp_diff d(-48.0);
    BOOST_CHECK_EQUAL(t, d); // temperatures are stored with -48C offset
}

BOOST_AUTO_TEST_CASE( temperature_diff_assignment )
{
    temp_diff a = 5.0;
    temp_diff b = a;
    BOOST_CHECK_EQUAL(a, b);


    temp_long c = 5.0;
    temp_long d = c;
    BOOST_CHECK_EQUAL(c, d);

    temp_precise e = 5.0;
    temp_precise f = e;
    BOOST_CHECK_EQUAL(e, f);
}

BOOST_AUTO_TEST_CASE( temperature_assignment )
{
    temp a = 5.0;
    temp b = a;
    BOOST_CHECK_EQUAL(a, b);


    temp_long c = 5.0;
    temp_long d = c;
    BOOST_CHECK_EQUAL(c, d);

    temp_precise e = 5.0;
    temp_precise f = e;
    BOOST_CHECK_EQUAL(e, f);
}

BOOST_AUTO_TEST_CASE( conversion_between_normal_long_and_precise_temp)
{
    // normal variable to convert from
    temp t0 = 1.0;
    temp_diff td0 = 1.0;

    // conversion to longer format
    temp_long tl0 = 1.0;
    temp_long tl1 = toLong(t0);
    temp_long tl2 = toLong(td0);

    BOOST_CHECK_EQUAL(tl0, tl1);
    BOOST_CHECK_EQUAL(tl0, tl2);

    // conversion to more fraction bits
    temp_precise tp0 = 1.0;
    temp_precise tp1 = toPrecise(t0);
    temp_precise tp2 = toPrecise(td0);

    BOOST_CHECK_EQUAL(tp0, tp1);
    BOOST_CHECK_EQUAL(tp0, tp2);
}

BOOST_AUTO_TEST_CASE( conversion_from_small_to_normal_temp_diff)
{
    // normal variable to convert from
    temp_diff_small ts0 = 0.5;
    temp_diff td0 = 0.5;

    // conversion to longer format
    temp_diff td1 = fromSmall(ts0);

    BOOST_CHECK_EQUAL(td0, td1);

    // check addition after conversion

    temp_diff td2 = td0 + fromSmall(ts0);
    BOOST_CHECK_EQUAL(td2, temp_diff(1.0));
}

BOOST_AUTO_TEST_CASE(temp_diff_conversion_to_fixed_length_string){
    for(double d = -64; d < 64; d += 0.1){
        temp_diff t = d;
        char s1[12];
        char s2[12];
        t.toString(s1, 3, 8);
        snprintf(s2, 12, "%7.03f", double(t)); // prints like " -3.000"
        BOOST_REQUIRE_MESSAGE(strcmp(s1, s2) == 0, "\"" << s1 << "\" should be \"" << s2 << "\"" << " converting " << t);
    }
}

BOOST_AUTO_TEST_CASE(temp_conversion_to_fixed_length_string){
    for(double d = -16; d < 112; d += 0.1){
        temp t = d;
        char s1[12];
        char s2[12];
        t.toString(s1, 3, 8);
        snprintf(s2, 12, "%7.03f", double(t)+48.0); // take into account the internal offset of 48C.
        BOOST_REQUIRE_MESSAGE(strcmp(s1, s2) == 0, "\"" << s1 << "\" should be \"" << s2 << "\"" << " converting " << t);
    }
}

BOOST_AUTO_TEST_CASE(temp_diff_conversion_to_variable_length_string){
    for(double d = -64; d < 64; d += 0.1){
        temp_diff t = d;
        char s1[12];
        char s2[12];
        char * s3 = t.toString(s1, 3, 8);
        snprintf(s2, 12, "%.03f", double(t)); // prints like "-3.000"
        BOOST_REQUIRE_MESSAGE(strcmp(s3, s2) == 0, "\"" << s1 << "\" should be \"" << s2 << "\"" << " converting " << t);
    }
}

BOOST_AUTO_TEST_CASE(temp_conversion_to_variable_length_string){
    for(double d = -16; d < 112; d += 0.1){
        temp t = d;
        char s1[12];
        char s2[12];
        char * s3 = t.toString(s1, 3, 8);
        snprintf(s2, 12, "%.03f", double(t)+48.0); // take into account the internal offset of 48C.
        BOOST_REQUIRE_MESSAGE(strcmp(s3, s2) == 0, "\"" << s1 << "\" should be \"" << s2 << "\"" << " converting " << t);
    }
}

BOOST_AUTO_TEST_CASE(temp_long_conversion_to_fixed_length_string){
    for(double d = -200; d < 200; d += 0.1){
        temp_long t = d;
        char s1[12];
        char s2[12];
        t.toString(s1, 3, 9);
        snprintf(s2, 12, "%8.03f", double(t));
        BOOST_REQUIRE_MESSAGE(strcmp(s1, s2) == 0, "\"" << s1 << "\" should be \"" << s2 << "\"" << " converting " << t);
    }
}

BOOST_AUTO_TEST_CASE(temp_long_conversion_to_variable_length_string){
    for(double d = -200; d < 200; d += 0.1){
        temp_long t = d;
        char s1[12];
        char s2[12];
        char * s3 = t.toString(s1, 4, 10);
        snprintf(s2, 12, "%.04f", double(t));
        BOOST_REQUIRE_MESSAGE(strcmp(s3, s2) == 0, "\"" << s1 << "\" should be \"" << s2 << "\"" << " converting " << t);
    }
}

BOOST_AUTO_TEST_CASE(temp_precise_conversion_to_fixed_length_string){
    for(double d = -128; d < 128; d += 0.1){
        temp_precise t = d;
        char s1[12];
        char s2[12];
        t.toString(s1, 3, 9);
        snprintf(s2, 12, "%8.03f", double(t));
        BOOST_REQUIRE_MESSAGE(strcmp(s1, s2) == 0, "\"" << s1 << "\" should be \"" << s2 << "\"" << " converting " << t);
    }
}

BOOST_AUTO_TEST_CASE(temp_precise_conversion_to_variable_length_string){
    for(double d = -128; d < 128; d += 0.1){
        temp_precise t = d;
        char s1[12];
        char s2[12];
        char * s3 = t.toString(s1, 5, 12);
        snprintf(s2, 12, "%.05f", double(t));
        BOOST_REQUIRE_MESSAGE(strcmp(s3, s2) == 0, "\"" << s1 << "\" should be \"" << s2 << "\"" << " converting " << t);
    }
}

BOOST_AUTO_TEST_CASE(temp_diff_small_conversion_to_fixed_length_string){
    for(double d = -8; d < 8; d += 0.0625){
        temp_diff_small t = d;
        char s1[12];
        char s2[12];
        t.toString(s1, 3, 9);
        snprintf(s2, 12, "%8.03f", round(double(t) * 1000.0) / 1000.0); // don't let snprintf do the rounding, it uses bankers rounding / round to even
        BOOST_REQUIRE_MESSAGE(strcmp(s1, s2) == 0, "\"" << s1 << "\" should be \"" << s2 << "\"" << " converting " << t);
    }
}

BOOST_AUTO_TEST_CASE(temp_diff_small_conversion_to_variable_length_string){
    for(double d = -8; d < 8; d += 0.0625){
        temp_diff_small t = d;
        char s1[12];
        char s2[12];
        char * s3 = t.toString(s1, 3, 9);
        snprintf(s2, 12, "%.03f", round(double(t) * 1000.0) / 1000.0); // don't let snprintf do the rounding, it uses bankers rounding / round to even
        BOOST_REQUIRE_MESSAGE(strcmp(s3, s2) == 0, "\"" << s1 << "\" should be \"" << s2 << "\"" << " converting " << t);
    }
}



BOOST_AUTO_TEST_SUITE_END()



/*
 * The MIT License (MIT)
 *
 * Copyright © 2015 Franklin "Snaipe" Mathieu <http://snai.pe/>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */
#ifndef CRITERION_H_
# define CRITERION_H_

#include <stddef.h>

struct criterion_test_extra_data {
    size_t sentinel_;
    void (*init)(void);
    void (*fini)(void);
};

struct criterion_test {
    const char *name;
    const char *category;
    void (*test)(void);
    const struct criterion_test_extra_data *data;
};

extern __attribute__ ((weak)) void criterion_init(void);
extern __attribute__ ((weak)) void criterion_fini(void);

# define SECTION_(Name) __attribute__((section(Name)))
# define IDENTIFIER_(Category, Name, Suffix) \
    Category ## _ ## Name ## _ ## Suffix
# define TEST_PROTOTYPE_(Category, Name) \
    void IDENTIFIER_(Category, Name, impl)(void)

# define Test(Category, Name, Args...)                                         \
    TEST_PROTOTYPE_(Category, Name);                                           \
    struct criterion_test_extra_data IDENTIFIER_(Category, Name, extra) = {    \
        .sentinel_ = 0,                                                        \
        Args                                                                   \
    };                                                                         \
    SECTION_("criterion_tests")                                                \
    const struct criterion_test IDENTIFIER_(Category, Name, meta) = {          \
        .name     = #Name,                                                     \
        .category = #Category,                                                 \
        .test     = IDENTIFIER_(Category, Name, impl),                         \
        .data     = &IDENTIFIER_(Category, Name, extra)                        \
    };                                                                         \
    TEST_PROTOTYPE_(Category, Name)

#endif /* !CRITERION_H_ */

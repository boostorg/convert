// Copyright (c) 2009-2014 Vladimir Batov.
// Use, modification and distribution are subject to the Boost Software License,
// Version 1.0. See http://www.boost.org/LICENSE_1_0.txt.

#ifndef BOOST_CONVERT_HAS_MEMFUN_NAME_HPP
#define BOOST_CONVERT_HAS_MEMFUN_NAME_HPP

#include <boost/type_traits/detail/yes_no_type.hpp>

// This macro allows to check if a type has a member function named "memfun_name"... regardless of the signature.
// If takes advantage of the following behavior related to function resolution.
// Say, both, foo and base, declare a method with the same name "func":
//
//      struct   foo { int func () { return 0; } };
//      struct  base { int func (int, int) { return 0; } };
//      struct mixin : public foo, public base {};
//
// Now, if we inherit from both -- foo and base -- classes, then the following calls will fail
//      ((mixin*( 0)->func();
//      ((mixin*( 0)->func(5, 5);
// with the error message (gcc): request for member ‘func’ is ambiguous
// regardless is we provide any arguments or not even though one might expect that arg-based signature
// resolution might kick in. The only wqay to deploy those methods is:
//
//      ((mixin*( 0)->foo::func();
//      ((mixin*( 0)->base::func(5, 5);
//
// C2. The actual signature of memfun_name is not taken into account.
//     If type::memfun_name(any-signature) exists, then
//     the introduced base::memfun_name will cause
//     mixin->memfun_name() call to fail to compile (due to ambiguity).
// C3. &U::memfun_name (a.k.a. &mixin::memfun_name)
//     has the type of func_type only if type::memfun_name does not exist.
//     If type::memfun_name does exist, then mixin::memfun_name is ambiguous
//     and "yes_type test (...)" kicks in instead.

#define DECLARE_HAS_MEMFUN_NAME(class_name, memfun_name)                                                \
                                                                                                        \
    template <typename type>                                                                            \
    class class_name                                                                                    \
    {                                                                                                   \
        typedef ::boost::type_traits::yes_type yes_type;                                                \
        typedef ::boost::type_traits:: no_type  no_type;                                                \
                                                                                                        \
        struct  base { void memfun_name(/*C2*/){} };                                                    \
        struct mixin : public base, public type {};                                                     \
                                                                                                        \
        template <typename U, U> struct aux {};                                                         \
                                                                                                        \
        typedef void (base::*func_type)();                                                              \
        typedef mixin* mixin_ptr;                                                                       \
                                                                                                        \
        template <typename U> static no_type  test (U*, aux<func_type, &U::memfun_name>* =0); /*C3*/    \
        /*******************/ static yes_type test (...);                                               \
                                                                                                        \
        public:                                                                                         \
                                                                                                        \
        BOOST_STATIC_CONSTANT(bool, value = (sizeof(yes_type) == sizeof(test(mixin_ptr(0)))));          \
    }

#endif // BOOST_CONVERT_HAS_MEMFUN_NAME_HPP

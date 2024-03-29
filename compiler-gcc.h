/*	--*- c -*--
 * Copyright (C) 2013 Enrico Scholz <enrico.scholz@informatik.tu-chemnitz.de>
@PROJECT_LICENSE@
 */

#ifndef H_UTUN_INCLUDE_COMPILER_GCC_H
#define H_UTUN_INCLUDE_COMPILER_GCC_H

#define likely(_cond)		(__builtin_expect(!!(_cond),1))
#define unlikely(_cond)		(__builtin_expect(!!(_cond),0))


#define _global_		__attribute__((__externally_visible__,__used__))
#define _noreturn_		__attribute__((__noreturn__))
#define _packed_		__attribute__((__packed__))
#define _aligned_(_a)		__attribute__((__aligned__(_a)))
#define _force_inline_		__attribute__((__always_inline__))
#define _must_be_checked_	__attribute__((__warn_unused_result__))
#define _unused_		__attribute__((__unused__))
#define _const_			__attribute__((__const__))
#define _pure_			__attribute__((__pure__))
#define _hidden_		__attribute__((__visibility__("hidden")))
#define _malloc_		__attribute__((__malloc__))

#ifndef DEBUG
#  define __do_die(_code)			\
	do {					\
                __builtin_trap();               \
        } while (1 == 1)
#elif defined(__arm__)
#  define __do_die(_code)					\
	do {							\
		__asm__ __volatile__ ("bkpt %0\n"		\
				      : : "I"((_code) % 256)	\
				      : "memory");		\
		__builtin_unreachable();			\
	} while (1 == 1)
#else
#  define __do_die(_code) do {                  \
                __builtin_trap();               \
        } while (1 == 1)
#endif

#define __is_type(_var,_type)				\
	(__builtin_types_compatible_p(__typeof__(_var),_type))

#define __must_be_array(_a)	\
	BUILD_BUG_ON_ZERO(__builtin_types_compatible_p(__typeof__(_a),	\
						       __typeof__(&(_a)[0])))

#define __must_be_builtin_constant(_cond) \
	((sizeof(char[1 - (2*(!(__builtin_constant_p(_cond))))])) - 1u)

#define barrier()		do {		\
		__asm__("" ::: "memory");	\
	} while (0==1)

/* uses compiler specific and c99 extensions */
#  define container_of(_ptr, _type, _attr) __extension__		\
	({								\
		__typeof__( ((_type *)0)->_attr) *_tmp_mptr = (_ptr);	\
		(_type *)((uintptr_t)_tmp_mptr - offsetof(_type, _attr)); \
	})

#define sign_cast(_totype, _fromtype, _v) __extension__			\
	({								\
		_fromtype	_tmp = (_v);				\
		BUILD_BUG_ON(sizeof(_totype) != sizeof(_fromtype));	\
		reinterpret_cast(_totype)(_tmp);			\
	})

#define const_cast2(_type, _v) __extension__	\
	({					\
		const _type	_tmp = (_v);	\
		(_type)(_tmp);			\
	})

#ifdef __clang
#  define HAVE_BUILTIN_MUL_OVERFLOW	(__has_attribute(__builtin_mul_overflow))
#  define HAVE_BUILTIN_SUB_OVERFLOW	(__has_attribute(__builtin_sub_overflow))
#elif __GNUC__ >= 5
#  define HAVE_BUILTIN_MUL_OVERFLOW	1
#  define HAVE_BUILTIN_SUB_OVERFLOW	1
#endif

#ifndef __STDC_VERSION__
#  warning "Unknown C version"
#elif __STDC_VERSION__ >= 201112L
#  if __GNUC__ >= 5 || defined(__clang__)
#    define COMPILER_HAS_C11_GENERIC	1
#  endif
#endif

#endif	/* H_UTUN_INCLUDE_COMPILER_GCC_H */

/*	--*- c -*--
 * Copyright (C) 2013 Enrico Scholz <enrico.scholz@informatik.tu-chemnitz.de>
@PROJECT_LICENSE@
 */

#ifndef H_UTUN_INCLUDE_COMPILER_GCC_H
#define H_UTUN_INCLUDE_COMPILER_GCC_H

#define likely(_cond)		(__builtin_expect(!!(_cond),1))
#define unlikely(_cond)		(__builtin_expect(!!(_cond),0))


#define __global		__attribute__((__externally_visible__,__used__))
#define __noreturn		__attribute__((__noreturn__))
#define __packed		__attribute__((__packed__))
#define __aligned(_a)		__attribute__((__aligned__(_a)))
#define __force_inline		__attribute__((__always_inline__))
#define __must_be_checked	__attribute__((__warn_unused_result__))
#define __unused		__attribute__((__unused__))

#if defined __arm__
#  define __die_code0(_code) __asm__ __volatile__ (".byte 0xff,0xff,0xff,0xff")
#  define __die_code1(_code) __asm__ __volatile__ ("bkpt %0" : : "I"((_code) % 256))
#endif

#ifndef DEBUG
#define __do_die(_code) do {						\
		__die_code0(_code);					\
		__builtin_unreachable();				\
	} while (1 == 1)
#else
#define __do_die(_code) do {						\
		__die_code1(_code);					\
		__builtin_unreachable();				\
	} while (1 == 1)
#endif

//	*(unsigned int volatile *)(0xdead0000u) = 0xdead0000u;

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

#endif	/* H_UTUN_INCLUDE_COMPILER_GCC_H */

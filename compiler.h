/*	--*- c -*--
 * Copyright (C) 2013 Enrico Scholz <enrico.scholz@informatik.tu-chemnitz.de>
@PROJECT_LICENSE@
 */

#ifndef H_UTUN_USB_COMPILER_H
#define H_UTUN_USB_COMPILER_H

/* used to encapsulate non misra code; workaround will expand to an empty
 * body */

#ifndef MISRA_COMPLIANCE
/* expansion violates 19.4 */
# define _cast(_exp)		(_exp))
# define const_cast(_t)		((_t)_cast
# define static_cast(_t)	((_t)_cast
# define precision_cast(_t)	((_t)_cast
# define reinterpret_cast(_t)	((_t)_cast
#else
# define const_cast(_t)		(_t)
# define static_cast(_t)	(_t)
# define precision_cast(_t)	(_t)
# define reinterpret_cast(_t)	(_t)
#endif	/* MISRA_COMPLIANCE */

#ifndef _lint
# define BUILD_BUG_ON_ZERO(_cond)	\
	((sizeof(char[1 - (2*(!(!(_cond))))])) - 1u + \
	 __must_be_builtin_constant(_cond))

# define must_not_be_const(_var)	do { \
	} while (0==1)

/* do not write (void)(_var) here; gcc provides better diagnostic than flex
 * and differs e.g. between calculated-but-not-used (-> bad) and
 * only-declared-but-not-used (-> common) */
# define variable_is_used(_var)		do { \
	} while (0==1)

#else
# define BUILD_BUG_ON_ZERO(_cond)	(0u)

#define must_not_be_const(_var)	do {		\
		(void)(_var);			\
	} while (0==1)

/*lint -emacro(530,variable_is_used)*/
# define variable_is_used(_var)		do { \
		(void)(_var);		     \
	} while (0==1)
#endif


#ifdef DEBUG
# define IS_DEBUG_MODE	(1==1)
#else
# define IS_DEBUG_MODE	(0==1)
#endif

/* evaluates to a non-null statement without sideeffects (rule 14.2) */
/*lint -emacro(514,BUILD_BUG_ON)*/
/*lint -emacro(717,BUILD_BUG_ON)*/
#define BUILD_BUG_ON(_cond) do {				\
		(void)(BUILD_BUG_ON_ZERO(_cond));		\
	} while (0)

/*lint -emacro(835,ARRAY_SIZE) -emacro(866,ARRAY_SIZE)*/
#define ARRAY_SIZE(_a) \
	(((size_t)(__must_be_array(_a))) + ((sizeof(_a) / sizeof((_a)[0]))))

/*lint -emacro(923,BUG)*/
/*lint -sem(BUG,r_no)*/
/*lint -sem(BUG,pure)*/
#define BUG()	do {							\
		if (IS_DEBUG_MODE) {					\
			barrier();					\
		}							\
		__do_die(IS_DEBUG_MODE ? (int)(__LINE__) : (int)0);	\
	} while (1==1)

/*lint -emacro(717,BUG_ON)*/
/*lint -function(__assert,BUG_ON)*/
#define BUG_ON(_cond) do {				\
		if (unlikely(_cond)) {			\
			BUG();				\
		}					\
	} while (0==1)

#define ASSERT(_cond)	BUG_ON(!(_cond))

/*lint -emacro(928,ASSERT_IN_ARRAY)*/
/*lint -emacro(946,ASSERT_IN_ARRAY)*/
/*lint -emacro(947,ASSERT_IN_ARRAY)*/
/*lint -emacro(960,ASSERT_IN_ARRAY)*/
#define ASSERT_IN_ARRAY(_ptr, _a) do {					\
		ptrdiff_t _ptr_pos_in_a;				\
		BUG_ON((_ptr) < &((_a)[0]));				\
		BUG_ON((_ptr) >= &((_a)[ARRAY_SIZE(_a)]));		\
		_ptr_pos_in_a = (reinterpret_cast(unsigned char *)(_ptr) - \
				 reinterpret_cast(unsigned char *)(&((_a)[0]))); \
		BUG_ON((static_cast(size_t)(_ptr_pos_in_a) %		\
			sizeof((_a)[0])) != 0u);			\
	} while (0==1)

#define ROUNDUP_DIV(_val, _div) \
	(((_val) + ((_div) - 1u)) / ((_div)))

#define ROUNDUP(_val, _div) \
	(ROUNDUP_DIV(_val, _div) * (_div))


#define UNIQUE_SYMBOL__(_prefix, _line, _suffix)	\
	_prefix ## _line ## _suffix

#define UNIQUE_SYMBOL_(_prefix, _line, _suffix)		\
	UNIQUE_SYMBOL__(_prefix, _line, _suffix)

#define UNIQUE_SYMBOL(_prefix, _suffix)			\
	UNIQUE_SYMBOL_(_prefix, __LINE__, _suffix)

/*lint -emacro(546,fn_ptr)*/
#define fn_ptr(_symbol)		(&(_symbol))

#define stringify(_m)	#_m
#define stringify2(_m)	stringify(_m)

#ifdef _lint
# include "compiler-lint.h"
#elif defined(__GNUC__)
# include "compiler-gcc.h"
#else
# error "Unsupporter compiler"
#endif


#endif	/* H_UTUN_USB_COMPILER_H */

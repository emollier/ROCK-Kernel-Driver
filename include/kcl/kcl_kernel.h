#ifndef AMDKCL_KERNEL_H
#define AMDKCL_KERNEL_H

#ifndef u64_to_user_ptr
#define u64_to_user_ptr(x) (	\
{					\
	typecheck(u64, x);		\
	(void __user *)(uintptr_t)x;	\
}					\
)
#endif

#ifndef __GFP_RETRY_MAYFAIL
#define __GFP_RETRY_MAYFAIL __GFP_REPEAT
#endif

#endif /* AMDKCL_KERNEL_H */

dnl #
dnl # commit v4.9-rc2-1-g8ef4227615e1
dnl # x86/io: add interface to reserve io memtype for a resource range. (v1.1)
dnl #
AC_DEFUN([AC_AMDGPU_ARCH_IO_RESERVE_FREE_MEMTYPE_WC_REAL], [
	AC_KERNEL_TRY_COMPILE_SYMBOL([
		#include <linux/io.h>
	], [
		arch_io_reserve_memtype_wc(0, 0);
		arch_io_free_memtype_wc(0, 0);
	], [arch_io_reserve_memtype_wc arch_io_free_memtype_wc], [arch/x86/mm/pat.c], [
		AC_DEFINE(HAVE_ARCH_IO_RESERVE_FREE_MEMTYPE_WC, 1,
			[arch_io_{reserve/free}_memtype_wc() are available])
	])
])

AC_DEFUN([AC_AMDGPU_ARCH_IO_RESERVE_FREE_MEMTYPE_WC], [
	AC_KERNEL_DO_BACKGROUND([
		AC_KERNEL_TEST_HEADER_FILE_EXIST([drm/drm_backport.h], [
			AC_KERNEL_TRY_COMPILE([
				#include <drm/drm_backport.h>
			], [
				arch_io_reserve_memtype_wc(0, 0);
				arch_io_free_memtype_wc(0, 0);
			], [
				AC_DEFINE(HAVE_ARCH_IO_RESERVE_FREE_MEMTYPE_WC, 1,
					[arch_io_{reserve/free}_memtype_wc() are available])
			], [
				AC_AMDGPU_ARCH_IO_RESERVE_FREE_MEMTYPE_WC_REAL
			])
		], [
			AC_AMDGPU_ARCH_IO_RESERVE_FREE_MEMTYPE_WC_REAL
		])
	])
])

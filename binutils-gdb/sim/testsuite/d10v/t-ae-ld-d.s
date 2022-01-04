# mach: all
# output:
# sim: --environment operating

.include "t-macros.i"

	start

	PSW_BITS = 0
	point_dmap_at_imem
	check_interrupt (VEC_AE&DMAP_MASK)+DMAP_BASE PSW_BITS test_ld

	ld r8,@0x4000
test_ld:
	ld r8,@0x4001
	nop
	exit47

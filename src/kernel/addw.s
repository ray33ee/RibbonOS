.global addw
.global subw

addw:
	add 		r0, r0, r1
	blx			lr

subw:
	sub 		r0, r0, r1
	blx 		lr
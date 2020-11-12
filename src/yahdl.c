#include <stdlib.h>
#include <stdio.h>

#include "vm.h"

int
main()
{
	short l[] = {OP_PUSH | (1 << 4), OP_PUSH | (1 << 4), OP_NAND, OP_RET};

	printf("%d", vm(0, (short *)l) & 1);
}

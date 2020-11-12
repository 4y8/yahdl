CC ?= cc

yahdl: src/yahdl.c src/vm.c src/lexer.c src/gram.c
	 $(CC) src/yahdl.c src/vm.c src/lexer.c src/gram.c -o yahdl -Wall

CC ?= cc

yahdl: src/yahdl.c src/vm.c src/lexer.c src/gram.c src/compile.c src/debug.c
	 $(CC) src/yahdl.c src/vm.c src/lexer.c src/gram.c src/compile.c src/debug.c -o yahdl -Wall

test: yahdl
	echo '$(shell ./yahdl)' | jq -M

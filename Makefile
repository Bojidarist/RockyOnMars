all:
	mkdir -p ./out
	${GBDKDIR}bin/lcc -Wa-l -Wl-m -Wl-j -c -o ./out/main.o ./src/main.c
	${GBDKDIR}bin/lcc -Wa-l -Wl-m -Wl-j -o ./out/main.gb ./out/main.o

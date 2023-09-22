all:
	mkdir -p ./out
	${GBDKDIR}bin/lcc -Wa-l -Wl-m -Wl-j -c -o ./out/RockyOnMars.o ./src/main.c
	${GBDKDIR}bin/lcc -Wa-l -Wl-m -Wl-j -o ./out/RockyOnMars.gb ./out/RockyOnMars.o

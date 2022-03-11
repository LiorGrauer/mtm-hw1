CC = gcc
CFLAGS = -std=c99 -g -Wall -pedantic-errors -Werror -DNDEBUG

chess: chess.o tournament.o game.o player.o 
	$(CC) $(CFLAGS) chess.o tournament.o game.o player.o tests/chessSystemTestsExample_maarof.c -L./libmap_for_csl3 -lmap -o chess
    
chess.o: chessSystem.c chessUtil.h chessSystem.h tournament.h mtm_map/map.h chessUtil.h game.h player.h mtm_map/map.h
	$(CC) -c $(CFLAGS) -o chess.o chessSystem.c
   
tournament.o: tournament.c tournament.h mtm_map/map.h chessUtil.h game.h player.h
	$(CC) -c $(CFLAGS) tournament.c

game.o: game.c game.h mtm_map/map.h chessUtil.h player.h
	$(CC) -c $(CFLAGS) game.c
	
player.o: player.c player.h mtm_map/map.h chessUtil.h
	$(CC) -c $(CFLAGS) player.c

clean: 
	rm -f chess chess.o tournament.o game.o player.o 

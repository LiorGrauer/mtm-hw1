CC = gcc
EXE = chess
MAP_EXE = map
OBJS = player.o game.o tournament.o chess.o
TEST_OBJ = chessSystemTestsExample.o
MAP_OBJS = map.o map_node.o
CFLAGS = -std=c99 -Wall -pedantic-errors -Werror -DNDEBUG
DEBUG_FLAG = -g 
LIB = -L. -lmap


$(EXE): $(OBJS) $(TEST_OBJ)
	$(CC) $(DEBUG_FLAG) $(CFLAGS) $(OBJS) $(TEST_OBJ) $(LIB) -o $(EXE)
    
chess.o: chessSystem.c chessUtil.h chessSystem.h tournament.h	\
			mtm_map/map.h chessUtil.h game.h player.h
	$(CC) -c $(DEBUG_FLAG) $(CFLAGS) -o chess.o chessSystem.c
   
tournament.o: tournament.c tournament.h mtm_map/map.h			\
			chessUtil.h game.h player.h
	$(CC) -c $(DEBUG_FLAG) $(CFLAGS) tournament.c

game.o: game.c game.h mtm_map/map.h chessUtil.h player.h
	$(CC) -c $(DEBUG_FLAG) $(CFLAGS) game.c
	
player.o: player.c player.h mtm_map/map.h chessUtil.h
	$(CC) -c $(DEBUG_FLAG) $(CFLAGS) player.c

$(TEST_OBJ): tests/chessSystemTestsExample.c chessSystem.h		\
			test_utilities.h
	$(CC) -c $(DEBUG_FLAG) $(CFLAGS) tests/chessSystemTestsExample.c
	
map: $(MAP_OBJS)
	$(CC) $(DEBUG_FLAG) $(CFLAGS) $(MAP_OBJS) -o $(MAP_EXE)
	
map.o: mtm_map/map.c mtm_map/map.h mtm_map/map_node.h utils.h
	$(CC) -c $(DEBUG_FLAG) $(CFLAGS) map.c
	
map_node.o: mtm_map/map_node.c mtm_map/map_node.h 				\
			mtm_map/map.h utils.h
	$(CC) -c $(DEBUG_FLAG) $(CFLAGS) mapNode.c

clean: 
	rm -f $(EXE) $(MAP_EXE) $(OBJS) $(TEST_OBJ) $(MAP_OBJS)

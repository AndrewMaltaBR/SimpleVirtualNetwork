#include "protocol.h"

mainSettings initializeProtocol(int ID){
  vertex *dijkstra_table;
  vertex **adjacency_list = NULL;
	mainSettings main_settings;
  int len;

  adjacency_list = readVertexesFromFile(adjacency_list, &len);
  showAdjacencyList(adjacency_list, len);

  dijkstra_table = createDijkstraTable(adjacency_list, len, ID);
  showDijkstraTable(dijkstra_table, len);

  main_settings = createMainSettings(dijkstra_table, len);
  showMainSettings(main_settings);

  freeAdjacencyList(adjacency_list, len);
  freeDijkstraTable(dijkstra_table);

	createSocket(&main_settings);

  return main_settings;
}

void die(char *s){
	perror(s);
	exit(1);
}
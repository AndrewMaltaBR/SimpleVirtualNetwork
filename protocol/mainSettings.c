#include "protocol.h"

mainSettings createMainSettings(vertex *dijkstra_table, int len){
  path *path_table = (path*) malloc(sizeof(path)*(len-1));
	mainSettings main_settings;
  int j = 0;
	FILE *file;

	if(!(file = fopen("configs/roteador.config", "r")))
    die("fopen()");
		
  proc procs[len];
  for(int i=0;i<len;i++)
		fscanf(file, "%d %d %s", &procs[i].ID, &procs[i].socket, procs[i].IP);
	fclose(file);

  for(int k=0;k<len;k++){
    if(dijkstra_table[k].cost != 0){
      int i = k;
      while(dijkstra_table[dijkstra_table[i].ID].cost != 0)
        i = dijkstra_table[i].ID;
      
      path_table[j].destination = procs[k];
      path_table[j].cost = dijkstra_table[k].cost;
      path_table[j].next_proc = procs[i];

      j++;
    }
		else
			main_settings.me = procs[k];
  }

	main_settings.path_table = path_table;
	main_settings.len = len-1;
  return main_settings;
}

void showMainSettings(mainSettings main_settings){
	path *path_table = main_settings.path_table;
  printf("\n\nMain Settings");
	printf(" (%d, %s, %d)", main_settings.me.ID, main_settings.me.IP, main_settings.me.socket);
  for(int i=0;i<main_settings.len;i++)
    printf("\nDestination: (%d, %s, %d) Cost: %d Next: (%d, %s, %d)", path_table[i].destination.ID, path_table[i].destination.IP, path_table[i].destination.socket, path_table[i].cost, path_table[i].next_proc.ID, path_table[i].next_proc.IP, path_table[i].next_proc.socket);
}
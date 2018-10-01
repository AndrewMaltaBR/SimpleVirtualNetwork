#include "protocol.h"

vertex *createDijkstraTable(vertex **adjacency_list, int len, int ID){
  int i, cost;
  // Create dijkstra_table
  vertex *dijkstra_table = (vertex *) malloc(sizeof(vertex)*len);
  for(i=0;i<len;i++)
    if(i == ID){
      dijkstra_table[i].ID = 0;
      dijkstra_table[i].cost = 0;
    }
    else
      dijkstra_table[i].cost = INF;

  // Set current_vertex
  int current_vertex = ID;

  // Create opened_vertexes list
  int opened_vertexes[len], opened_vertexes_len = len;
  for(i=0;i<len;i++)
    opened_vertexes[i] = FALSE;
  
  // Find edges and calc them
  while(opened_vertexes_len-- > 0){
    // Update opened_vertexes list
    opened_vertexes[current_vertex] = TRUE;
    
    // For each adjacent opened_vertexes of current_vertex
    for(vertex *p=adjacency_list[current_vertex];p!=NULL;p=p->next){
      cost = p->cost;

      // Update dijkstra_table if it's lower cost
      int cumulative_cost = dijkstra_table[current_vertex].cost+cost;
      if(cumulative_cost < dijkstra_table[p->ID].cost){
        dijkstra_table[p->ID].ID = current_vertex;
        dijkstra_table[p->ID].cost = cumulative_cost;
      }
    }

    // Set the next vertex
    cost = INF;
    for(i=0;i<len;i++)
      // Verify if it's is the lower opened vertex
      if(dijkstra_table[i].cost < cost && !opened_vertexes[i]){
        current_vertex = i;
        cost = dijkstra_table[i].cost;
      }
  }

  return dijkstra_table;
}


void showDijkstraTable(vertex *dijkstra_table, int len){
  printf("\n\nDijkstra Table");
  for(int i=0;i<len;i++){
    printf("\nDestination: %d Previous: ", i);
    if(dijkstra_table[i].ID == dijkstra_table[i].cost && dijkstra_table[i].cost == 0)
      printf("x ");
    else
      printf("%d ", dijkstra_table[i].ID);
    printf("Cost: %d", dijkstra_table[i].cost);
  }
}

void freeDijkstraTable(vertex *dijkstra_table){
  free(dijkstra_table);
}
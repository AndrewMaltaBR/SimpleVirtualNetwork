#include "protocol.h"

vertex *addVertex(vertex *adjacency_list, int ID, int cost){
  vertex *new = (vertex *) malloc(sizeof(vertex));
  new->ID = ID;
  new->cost = cost;
  new->next = NULL;

  if(adjacency_list == NULL)
    return new;
  
  for(vertex *c=adjacency_list;c!=NULL;c=c->next)
    if(c->next == NULL){
      c->next = new;
      break;
    }

  return adjacency_list;
}

vertex **readVertexesFromFile(vertex **adjacency_list, int *len){
  FILE *file;
  *len = 0;

  if(!(file = fopen("configs/enlaces.config", "r")))
    die("fopen()");
  
  int vertex_a, vertex_b, cost, malloc_flag=0;
  while(fscanf(file, "%d %d %d", &vertex_a, &vertex_b, &cost) != EOF){
    short int realloc_flag = 0;
    if(vertex_a >= *len){
      *len = vertex_a+1;
      realloc_flag = 1;
    }
    if(vertex_b >= *len){
      *len = vertex_b+1; 
      realloc_flag = 1;
    }

    if(++malloc_flag == 0)
      adjacency_list = (vertex **) malloc(sizeof(vertex *)*(*len));
    else if(realloc_flag == 1)
      adjacency_list = (vertex **) realloc(adjacency_list, sizeof(vertex *)*(*len));

    adjacency_list[vertex_a] = addVertex(adjacency_list[vertex_a], vertex_b, cost);
    adjacency_list[vertex_b] = addVertex(adjacency_list[vertex_b], vertex_a, cost);
  }
  fclose(file);
  return adjacency_list;
}

void showAdjacencyList(vertex **adjacency_list, int len){
  printf("\nAdjacency List");
  for(int i=0;i<len;i++){
    printf("\nVertex %d:", i);
    for(vertex *p=adjacency_list[i];p!=NULL;p=p->next)
      printf(" (%d, %d)", p->ID, p->cost);
  }
}

void freeAdjacencyList(vertex **adjacency_list, int len){
  for(int i=0;i<len;i++){
    vertex *p=adjacency_list[i];
    while(p!=NULL){
      vertex *aux = p->next;
      free(p);
      p = aux;
    }
  }
  free(adjacency_list);
}
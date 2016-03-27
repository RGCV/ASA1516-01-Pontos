#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

#include "graph.h"

int main() {
  int V, E;
  int ap_data[3];
  graph_t *graph;
  
  assert(scanf("%d %d", &V, &E) == 2);
  assert(V > 0 && E > 0);

  graph = graph_new(V);
  assert(graph != NULL);

  while(E-- > 0) {
    int u, v;
    assert(scanf("%d %d", &u, &v) == 2);
    graph_add_edge(graph, u, v);
  }

  AP_Tarjan(graph, ap_data);

  printf("%d\n%d %d\n", ap_data[0], ap_data[1], ap_data[2]);
  graph_free(graph);

  return EXIT_SUCCESS;
}

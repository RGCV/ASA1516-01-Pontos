#include <stdlib.h>
#include <stdbool.h>

#include "graph.h"

struct _graph_node {
  graph_vertex_t vertex;
  graph_node_t *next;
};

struct _graph {
  int V, E;
  graph_node_t **adjlist;
};

graph_t *graph_new(int V) {
  graph_t *graph = NULL;
  
  if(V > 0 &&
      (graph = (graph_t*)malloc(sizeof(struct _graph))) != NULL) {
    graph->V = V;
    graph->E = 0;
    graph->adjlist = 
      (graph_node_t**)malloc(V * sizeof(struct _graph_node*));
  }
  
  return graph;
}

void graph_free(graph_t *graph) {
  int i;
  
  for(i = 0; i < graph->V; ++i) {
    graph_node_t *head = graph->adjlist[i];
    while(head != NULL) {
      graph_node_t *temp = head;
      head = head->next;
      free(temp);
    }
  }
  free(graph->adjlist);
  free(graph);
}

int graph_vertices(graph_t *graph) {
  return graph->V;
}

int graph_edges(graph_t *graph) {
  return graph->E;
}

void graph_add_edge(graph_t *graph, graph_vertex_t u, graph_vertex_t v) {
  if((u > 0 && u <= graph->V) && (v > 0 && v <= graph->V)) {
    graph_node_t *w = (graph_node_t*)malloc(sizeof(struct _graph_node));
    graph_node_t *x = (graph_node_t*)malloc(sizeof(struct _graph_node));

    if(w != NULL && x != NULL) {
      w->vertex = u;
      x->vertex = v;
      w->next = graph->adjlist[v - 1];
      x->next = graph->adjlist[u - 1];
      
      graph->adjlist[v - 1] = w;
      graph->adjlist[u - 1] = x;
    }
    else {
      exit(EXIT_FAILURE);
    }
    
    graph->E++;
  }
}

void AP_Tarjan(graph_t *graph, int ap_data[3]) {
  int **info, i;

  ap_data[AP_COUNT] = 0;
  ap_data[MIN_AP] = ap_data[MAX_AP] = NIL;

  info = (int**)malloc(4 * sizeof(int*));
  if(info != NULL) {
    info[TIME] = (int*)malloc(sizeof(int));
    info[D] = (int*)malloc(graph->V * sizeof(int));
    info[LOW] = (int*)malloc(graph->V * sizeof(int));
    info[PI] = (int*)malloc(graph->V * sizeof(int));
    if(!info[TIME] || !info[D] || !info[LOW] || !info[PI]) {
      exit(EXIT_FAILURE);
    }
  }
  else {
    exit(EXIT_FAILURE);
  }

  info[TIME][0] = 0;
  for(i = 0; i < graph->V; i++) {
    info[D][i] = NIL;
    info[PI][i] = NIL;
  }

  i = 0;
  while(info[D][i++] == NIL)
    DFS_Tarjan(graph, i, info, ap_data);

  free(info[TIME]);
  free(info[D]);
  free(info[LOW]);
  free(info[PI]);
  free(info);
}

void DFS_Tarjan(graph_t *graph, graph_vertex_t u, int **info, int ap_data[3]) {
  graph_node_t *node = graph->adjlist[u - 1];
  graph_vertex_t adj;
  bool isArticulationPoint = false;
  int childCount = 0;
  
  info[D][u - 1] = info[TIME][0];
  info[LOW][u - 1] = info[TIME][0];
  ++info[TIME][0];
  
  while(node != NULL) {
    adj = node->vertex;
    /* If adj wasn't discovered yet, visit it */
    if(info[D][adj - 1] == NIL) {
      info[PI][adj - 1] = u;
      ++childCount;
      DFS_Tarjan(graph, adj, info, ap_data);

      /* If the following is true, there isn't a back edge connecting adj to
        the rest of the graph where u comes from, making u fundamental to adj */
      if(info[D][u - 1] <= info[LOW][adj - 1]) {
        isArticulationPoint = true;
      }
      else {
        /* Propagate the lowtime */
        info[LOW][u - 1] = min(info[LOW][u - 1], info[LOW][adj - 1]);
      }
    }
    /* If the adjacent has been visited and isn't the parent, a back edge
      was found */
    else if(adj != info[PI][u - 1]) {
      /* As such, update lowtime of the vertex being visited */
      info[LOW][u - 1] = min(info[LOW][u - 1], info[D][adj - 1]);
    }
    /* Move on to the next adjacency */
    node = node->next;
  }

  /* If (1) u's the root vertex of the DFS and has 2+ independant children
    or, (2) if it isn't the root, but was marked as an articulation point */
  if((info[PI][u - 1] == NIL && childCount >= 2)
      || (info[PI][u - 1] != NIL && isArticulationPoint)) {
    ++ap_data[AP_COUNT]; /* Increment number of articulation points */
    /* Update the minimum and maximum articulation points */
    if(ap_data[MIN_AP] == NIL || ap_data[MAX_AP] == NIL) {
      ap_data[MIN_AP] = ap_data[MAX_AP] = u;
    }
    else {
      ap_data[MIN_AP] = min(ap_data[MIN_AP], u);
      ap_data[MAX_AP] = max(ap_data[MAX_AP], u);
    }
  }
}
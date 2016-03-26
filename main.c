#include <assert.h>
#include <limits.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

/* --- def macros/constants --- */
/* NIL value representation */
#define NIL -1

/* Minimum between a and b */
#define min(a, b) ((a) < (b) ? (a) : (b))

/* Maximum between a and b */
#define max(a, b) ((a) < (b) ? (b) : (a))
/* --- end macros/constants --- */

/* --- def globals --- */
/* Vertex visit time */
int g_time;

/* Min articulation point */
int g_min_ap;

/* Max articulation point */
int g_max_ap;

/* Articulation point count */
int g_ap_count;
/* --- end globals --- */

/* --- def _GRAPH_H --- */
/* Useless, but a good way to wrap code (aesthetically pleasing) */
#ifndef _GRAPH_H
#define _GRAPH_H
  /* Defines the type of a vertex (simple int) */
  typedef int                graph_vertex_t;

  /* Adjacency list node */
  typedef struct _graph_node {
    graph_vertex_t vertex;
    struct _graph_node *next;
  } graph_node_t;

  /* Graph structure */
  typedef struct _graph {
    int V, E;
    graph_node_t **adjlist;
  } graph_t;

  /* Create a new graph */
  graph_t *graph_new       (int V);
  /* Free memory allocated associated with the given graph */
  void     graph_free      (graph_t *graph);
  /* Add an edge to the graph */
  void     graph_add_edge  (graph_t *graph, graph_vertex_t u, graph_vertex_t v);

  /* DFS function */
  void     DFS             (graph_t *graph, graph_vertex_t vertex);
  /* DFS recursive subroutine */
  void     DFSr            (graph_t *graph, graph_vertex_t u, int *d, int *low,
                             int *pi);
#endif /* _GRAPH_H */
/* --- end _GRAPH_H --- */

/* --- main program --- */
int main() {
  int V, E;
  graph_t *graph = NULL;
  
  assert(scanf("%d %d", &V, &E) == 2);
  assert(V > 0);
  assert(E > 0);
  
  graph = graph_new(V);
  assert(graph != NULL);

  while(E-- > 0) {
    int u, v;
    assert(scanf("%d %d", &u, &v) == 2);
    graph_add_edge(graph, u, v);
  }

  DFS(graph, 1);

  graph_free(graph);
  printf("%d\n%d %d\n", g_ap_count, g_min_ap, g_max_ap);

  return EXIT_SUCCESS;
}
/* --- end main program --- */

/* --- def protos --- */
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

void DFS(graph_t *graph, graph_vertex_t vertex) {
  int i, *d, *low, *pi;

  g_ap_count = 0;
  g_min_ap = g_max_ap = NIL;

  assert((d = (int*)malloc(graph->V*sizeof(int))) != NULL);
  assert((low = (int*)malloc(graph->V*sizeof(int))) != NULL);
  assert((pi = (int*)malloc(graph->V*sizeof(int))) != NULL);

  for(i = 0; i < graph->V; i++) {
    d[i] = NIL;
    pi[i] = NIL;
  }

  i = 0;
  while(d[i++] == NIL)
    DFSr(graph, vertex, d, low, pi);

  free(d);
  free(low);
  free(pi);
}

void DFSr(graph_t *graph, graph_vertex_t u, int *d, int *low, int *pi) {
  graph_node_t *node = graph->adjlist[u - 1];
  graph_vertex_t adj;
  bool isArticulationPoint = false;
  int childCount = 0;
  
  d[u - 1] = g_time;
  low[u - 1] = g_time;
  ++g_time;
  
  while(node != NULL) {
    adj = node->vertex;
    /* If adj wasn't discovered yet, visit it */
    if(d[adj - 1] == NIL) {
      pi[adj - 1] = u;
      ++childCount;
      DFSr(graph, adj, d, low, pi);

      /* If the following is true, there isn't a back edge connecting adj to
        the rest of the graph where u comes from, making u fundamental to adj */
      if(d[u - 1] <= low[adj - 1]) {
        isArticulationPoint = true;
      }
      else {
        /* Propagate the lowtime */
        low[u - 1] = min(low[u - 1], low[adj - 1]);
      }
    }
    /* If the adjacent has been visited and isn't the parent, a back edge
      was found */
    else if(adj != pi[u - 1]) {
      /* As such, update lowtime of the vertex being visited */
      low[u - 1] = min(low[u - 1], d[adj - 1]);
    }
    /* Move on to the next adjacency */
    node = node->next;
  }

  /* If (1) u's the root vertex of the DFS and has 2+ independant children
    or, (2) if it isn't the root, but was marked as an articulation point */
  if((pi[u - 1] == NIL && childCount >= 2)
      || (pi[u - 1] != NIL && isArticulationPoint)) {
    ++g_ap_count; /* Increment number of articulation points */
    /* Update the minimum and maximum articulation points */
    if(g_min_ap == NIL || g_max_ap == NIL) {
      g_min_ap = g_max_ap = u;
    }
    else {
      g_min_ap = min(g_min_ap, u);
      g_max_ap = max(g_max_ap, u);
    }
  }
}
/* --- end protos --- */

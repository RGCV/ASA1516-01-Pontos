/*
   Define a basic implementation of a graph.
   Uses adjacency lists. Each list represents a vertex's
   adjacencies. For example:
   1: 3 -> 4 -> 7
   Means vertex 1 shares an edge with vertices 3, 4 and 7.

   Being a basic implementation, it isn't meant for generic
   applications given the specific type definition, for example.
*/

#ifndef _GRAPH_H_
#define _GRAPH_H_

/* NIL value representation. */
#define NIL -1

/* DFS info matrix index macros. */
/* Visiting time. */
#define TIME 0

/* Discovery time. */
#define D    1

/* Low time. */
#define LOW  2

/* Predecessors. */
#define PI   3

/* Articulation point vector index macros. */
/* Articulation point count. */
#define AP_COUNT 0

/* Minimum articulation point id. */
#define MIN_AP   1

/* Maximum articulation point id. */
#define MAX_AP   2

/* Macro functions. */
/* Minimum between a and b. */
#define min(a, b) ((a) < (b) ? (a) : (b))

/* Maximum between a and b. */
#define max(a, b) ((a) < (b) ? (b) : (a))

/* Defines the type of a vertex (simple int). */
typedef int                graph_vertex_t;

/* Adjacency list node. */
typedef struct _graph_node graph_node_t;

/* Graph structure. */
typedef struct _graph      graph_t;

/* Create a new graph. */
graph_t *graph_new       (int V);

/* Free memory allocated associated with the given graph. */
void     graph_free      (graph_t *graph);

/* Returns the number of vertices in the graph. */
int      graph_vertices  (graph_t *graph);

/* Returns the number of edges in the graph. */
int      graph_edges     (graph_t *graph);

/* Add an edge to the graph */
void     graph_add_edge  (graph_t *graph, graph_vertex_t u, graph_vertex_t v);

/* DFS function. Stores articulation point data in ap_data:
   ap_data[0]: Articulation Points count;
   ap_data[1]: Minimum Articulation Point id;
   ap_data[2]: Maximum Articulation Point id. */
void     AP_Tarjan       (graph_t *graph, int ap_data[3]);

/* DFS recursive subroutine. Carries an info matrix that stores data relevant
   to the DFS and the ap_data vector (being u a vertex in [1,V]):
   info[0][0]:     Vertex visiting time;
   info[1][u - 1]: u's discovery time;
   info[2][u - 1]: u's low time;
   info[3][u - 1]: u's predecessor. */
void     DFS_Tarjan      (graph_t *graph, graph_vertex_t u, int **info, 
                            int ap_data[3]);
#endif /* _GRAPH_H_ */
#define _POSIX_C_SOURCE 200809L // Required for strdup()
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NODES 1024

typedef struct Node {
    char *name;
    int visited;
    int num_edges;
    struct Node *edges[MAX_NODES];
} Node;

Node nodes[MAX_NODES];
int node_count = 0;

// Find a node by name, or create it if it doesn't exist
Node *get_node(const char *name) {
    for (int i = 0; i < node_count; i++) {
        if (strcmp(nodes[i].name, name) == 0) {
            return &nodes[i];
        }
    }

    // Create a new node
    if (node_count >= MAX_NODES) {
        fprintf(stderr, "tsort: too many nodes\n");
        exit(1);
    }

    Node *node = &nodes[node_count++];
    node->name = strdup(name); // Properly declared now
    node->visited = 0;
    node->num_edges = 0;
    return node;
}

// Add an edge from `from` to `to`
void add_edge(Node *from, Node *to) {
    if (from->num_edges >= MAX_NODES) {
        fprintf(stderr, "tsort: too many edges\n");
        exit(1);
    }
    from->edges[from->num_edges++] = to;
}

// Perform depth-first search and detect cycles
int dfs(Node *node, Node **stack, int stack_size) {
    if (node->visited == 2) {
        return 0; // Already processed
    }
    if (node->visited == 1) {
        // Cycle detected
        fprintf(stderr, "tsort: cycle detected involving '%s'\n", node->name);
        exit(1);
    }

    node->visited = 1; // Mark as visiting
    stack[stack_size++] = node;

    for (int i = 0; i < node->num_edges; i++) {
        if (dfs(node->edges[i], stack, stack_size)) {
            return 1;
        }
    }

    node->visited = 2; // Mark as visited
    printf("%s\n", node->name);
    return 0;
}

// Topological sort
void tsort() {
    Node *stack[MAX_NODES];
    int stack_size = 0;

    for (int i = 0; i < node_count; i++) {
        if (nodes[i].visited == 0) {
            dfs(&nodes[i], stack, stack_size);
        }
    }
}

int main(int argc, char **argv) {
    if (argc > 2) {
        fprintf(stderr, "Usage: tsort [FILE]\n");
        fprintf(stderr, "Perform a topological sort on FILE.\n");
        fprintf(stderr, "With no FILE, read from standard input.\n");
        return 1;
    }

    FILE *file = stdin;
    if (argc == 2) {
        file = fopen(argv[1], "r");
        if (!file) {
            perror("tsort: fopen");
            return 1;
        }
    }

    char from[1024], to[1024];

    while (fscanf(file, "%s %s", from, to) == 2) {
        Node *from_node = get_node(from);
        Node *to_node = get_node(to);
        add_edge(from_node, to_node);
    }

    tsort();

    if (file != stdin) {
        fclose(file);
    }

    return 0;
}

# Author: Eli Scheer
# Open the input file and read the first line to get n, p, and c
with open('chocolates.in1', 'r') as f:
    n, p, c = map(int, f.readline().split())

# Read the spots of the children
    spots = []
    for i in range(n):
        spot = int(f.readline().strip())
        spots.append(spot)

# Create an adjacency matrix to represent the graph
    adj_matrix = [[float('inf')] * p for i in range(p)]
    for i in range(p):
        adj_matrix[i][i] = 0

# Filling the adjacency matrix
    for i in range(c):
        u, v, w = map(int, f.readline().split())
        adj_matrix[u-1][v-1] = w
        adj_matrix[v-1][u-1] = w

# Compute the shortest pair paths
for k in range(p):
    for i in range(p):
        for j in range(p):
            adj_matrix[i][j] = min(adj_matrix[i][j], adj_matrix[i][k] + adj_matrix[k][j])

# Compute the total distance for each spot and find the minimum
min_distance = float('inf')
for i in range(p):
    total_distance = sum(adj_matrix[i][spot-1] for spot in spots)
    min_distance = min(min_distance, total_distance)

# Output the result
with open('chocolates.out1', 'w') as f:
    f.write(str(min_distance))
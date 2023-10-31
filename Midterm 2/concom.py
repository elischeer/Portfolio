#Author: Eli Scheer
with open('concom.in1', 'r') as f:
    n = int(f.readline().strip())
    own = [[0] * 100 for _ in range(100)]
    control = [[False] * 100 for _ in range(100)]
    for _ in range(n):
        i, j, p = map(int, f.readline().strip().split())
        own[i-1][j-1] = p

    for i in range(100):
        for j in range(100):
            if i != j:
                if own[i][j] > 50:
                    control[i][j] = True
                else:
                    for k in range(100):
                        if control[i][k] and own[k][j] > 0:
                            control[i][j] = True
                            break
                if control[i][j]:
                    for k in range(100):
                        own[i][k] += own[j][k]

with open('concom.out1', 'w') as f:
    for i in range(100):
        for j in range(100):
            if i != j and control[i][j]:
                f.write(f"{i+1} {j+1}\n")
# Astarix-PairEnd
author: Stefan Stoyanov
mentor: Pesho Ivanov
Task: Fast and optimal alignment of short paired-end reads using A*
Metric: ed(q_left) + ed(q_right) + fragment length penalty
State space: <u,v,i> — aligning q_left[:i] until ref[u] and q_right[:i] until ref[v] 
A* heuristic: seed heuristic with crumbs
Optimizations: greedy matching (aka sliding), bit compression

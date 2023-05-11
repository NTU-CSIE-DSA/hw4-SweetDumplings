## Description

There are $N$ types of sweet dumplings for sale in DSA (Deluxe Supermarket Alliance) each day. For each type, there are two packs available at different prices.

Alexander and Bomboo both want to buy one pack of each type of sweet dumpling for the following $M$ days. Because both of them want to minimize their costs, they agreed to buy sweet dumplings with the following rules:

1. They will buy exactly one pack of sweet dumplings in turns, and Alexander goes first.
2. Both of them will have exactly one pack per type at the end of the purchasing process for the $M$ days
3. Sweet dumplings are restocked to two packs every day.
4. Both of them are genius, hence they will use the optimal strategy to choose the pack to buy.

However, the prices of sweet dumplings are changing. Every day, the price of exactly one type of sweet dumpling will permanently change.

Can you help Alexander to calculate his cost each day?

## Input

The first line contains two integers, $N$ and $M$, representing the number of kinds of sweet dumplings and the length of Alexander's challenge.

The next $N$ lines contain two integers each, $a_i$ and $b_i$, representing the price of two packs of sweet dumplings of the $i$-th type.

The following $M-1$ lines contain three integers each, $t_i$, $c$, and $d$, indicating that the price of the $t_i$-th type of sweet dumpling will change to $c$ and $d$ on day $i+1$.

## Output

The output should consist of $M$ lines. The $i$-th line should contain a single integer representing the minimum cost for Alexander to buy all kinds of sweet dumplings on the $i$-th day.

## Constraint

- $1\le N\le 10^5$
- $1\le M\le 10^5$
- $1\le a_i, b_i, c_i, d_i \le 10^5$
- $1 \le t_i\le N$

### Subtask 1 (5pts)

- $M = 1$

### Subtask 1 (10pts)

- $1\le N, M \le 10^3$

### Subtask 3 (15pts)

- $a_i, b_i, c_i, d_i$ are generated randomly.

### Subtask 4 (70pts)

No other constraint

## Sample Testcases

### Sample Input 1

```
4 6
2 4
5 7
1 7
2 1
4 5 2
1 6 2
4 4 3
2 1 3
3 6 6
```

### Sample Output 1

```
12
15
16
16
12
14
```

### Sample Input 2

```
5 1
1 1
2 3
4 6
7 10
11 15
```

### Sample Output 2

```
29
```

## Hint

Here are the details of sample 1:

- First day, Alexander can buy each type with price $4, 5, 1, 2$ respectively to minimize his cost.
- Second day, The price of each type is $(2, 4), (5, 7), (1, 7), (5, 2)$ respectively. And Alexander can buy each type with price $2, 7, 1, 5$ respectively to minimize his cost.
- Third day, The price of each type is $(6, 2), (5, 7), (1, 7), (5, 2)$ respectively. And Alexander can buy each type with price $6, 7, 1, 2$ respectively to minimize his cost.
- and so on.

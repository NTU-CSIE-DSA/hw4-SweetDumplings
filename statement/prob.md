## Description

There are $N$ types of sweet dumplings for sale in DSA (Deluxe Supermarket Alliance) each day. For each type, there are two packs available at different prices.

Alexander and Bomboo both want to buy one pack of each type of sweet dumpling for the following $M$ days. Because both of them want to minimize their costs, they agreed to buy sweet dumplings with the following rules:

1. They will buy exactly one pack of sweet dumplings in turns, and Alexander goes first.
2. Both of them will have exactly one pack per type at the end of the purchasing process for the $M$ days
3. Sweet dumplings are restocked to two packs every day.
4. Both of them are genius, hence they will use the optimal strategy to choose the pack to buy.

However, the prices of sweet dumplings are changing. Every day except the first day, the price of exactly one type of sweet dumpling will permanently change.

Can you help Alexander to calculate his cost each day?

## Input

The first line contains two integers, $N$ and $M$, representing the number of kinds of sweet dumplings and the length of Alexander's challenge.

The next $N$ lines contain two integers each, $a_i$ and $b_i$, representing the price of two packs of sweet dumplings of the $i$-th type.

In the following $M-1$ lines, the $i$-th line contains three integers $t_i, c_i, d_i, e_i, f_i$. Indicating that the price of $t_i$-th sweet dumplings will change to $(c_i \times P + d_i \mod 1000000007)$ and $(e_i \times P + f_i \mod 1000000007)$ on day $i + 1$, where $P$ is the minimum cost for Alexander on the previous day.

(This problem has nothing to do with the mathematical properties of the price-updating formula.)

## Output

The output should consist of $M$ lines. The $i$-th line should contain a single integer representing the minimum cost for Alexander to buy all kinds of sweet dumplings on the $i$-th day.

## Constraint

- $1\le N\le 10^5$
- $1\le M\le 10^5$
- $1\le a_i, b_i \le 10^9$
- $0\le c_i, d_i, e_i, f_i \le 10^9$
- $1\le t_i\le N$

### Subtask 1 (5pts)

- $M = 1$

### Subtask 2 (10pts)

- $1\le N, M \le 10^3$

### Subtask 3 (15pts)

- $a_i, b_i$ are generated randomly.

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
4 0 5 0 2
1 0 6 0 2
4 0 4 0 3
2 0 1 0 3
3 0 6 0 6
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
5 4
1 1
2 3
4 6
7 10
11 15
1 0 1 0 6
3 1 4 1 5
5 4 3 2 1
```

### Sample Output 2

```
29
31
61
174
```

## Hint

Here are the details of sample 1:

- First day, Alexander can buy each type with price $4, 5, 1, 2$ respectively to minimize his cost.
- Second day, the price of type $4$ is changes to $12\times 0 + 5$ and $12\times 0 + 2$. Hence the price of each type is $(2, 4), (5, 7), (1, 7), (5, 2)$ respectively. And Alexander can buy each type with price $2, 7, 1, 5$ respectively to minimize his cost.
- Third day, the price of type $1$ is changes to $12\times 0 + 6$ and $12\times 0 + 2$. Hence the price of each type is $(6, 2), (5, 7), (1, 7), (5, 2)$ respectively. And Alexander can buy each type with price $6, 7, 1, 2$ respectively to minimize his cost.
- and so on.

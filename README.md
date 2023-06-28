# DSA 2023 HW4 - Alexander Loves Sweet Dumplings

## Chinese Tutorial

針對每一種湯圓，我們一定會優先買比較便宜的那包；而對所有種類的湯圓而言，我們會想要優先買價差比較大的那一種。

先從只有一天的狀況開始討論：

我們可以不失一般性的假設 $a_i - b_i > a_{i + 1} - b_{i + 1}$ 且 $a_i > b_i$，那麼 Alexander 一定會先買 $b_1$、接著 Bamboo 一定會買 $b_2$，以此類推，所以 Alexander 最終的花費會是 $b_1 + a_2 + b_3 + a_4 \dots$

接下來推廣到好幾天的狀況：

我們會發現每次假錢改動就會破壞原本對於價差的排序、所以我們就會需要重新排序並重新求和，但這麼做顯然太慢了。

仔細觀察之後、我們會發現我們其實每天只會動一個點，所以我們這裡可以使用紅黑樹，如此一來，更改價錢就只是把原本的價錢從樹上`remove`、並把新的價錢 `insert` 進去而已，所以我們就可以在 $O(\log N)$ 的時間內維護各種湯圓按照價差的排序了。

那麼總價和呢？觀察我們紅黑樹的結構、會發現每一顆子樹都代表著我們湯圓序列的一個區間，那也許我們可以在每個節點都紀錄他下面的那顆子樹的總價和，然後一層一層的回推到 root 就可以得到所有湯圓的總價和了。

但這麼做會遇到一個問題：由於每個節點的位置的奇偶性會隨著時間變化，舉例來說，原本 Alexander 買 $b_1, a_2, b_3$，但某一天 type 3 的價差突然變得比 type 1 還大了、那麼 Alexander 買的就會變成 $b_3, a_1, b_2$。

為了解決這個問題、我們在每個節點要紀錄兩個資訊：

1. Alexander 在這個區間先手的花費
2. Alexander 在這個區間後手的花費

如此一來、我們每次更改價錢時就可以根據各個區間新的先/後手狀態來靈活的決定當下的花費了。而每天的答案就會是根節點中、Alexander 的先手花費了。

## English Tutorial

For each type of Sweet Dumplings, we always prioritize buying the cheaper pack. And among all types of sweet dumplings, we prefer to buy the one with the largest price difference.

Let's start by discussing the case of only one day:

Generality, let's assume $a_i - b_i > a_{i + 1} - b_{i + 1}$ and $a_i > b_i$. In this case, Alexander will definitely buy $b_1$ first, then Bamboo will buy $b_2$, and so on. Therefore, Alexander's final expenditure will be $b_1 + a_2 + b_3 + a_4 \dots$

Now let's extend this to multiple days:

We observe that each time a price change occurs, it disrupts the original sorting based on price differences. Therefore, we need to re-sort and recalculate the sum, but this approach is obviously too slow.

After careful observation, we notice that we only move one point each day. So, we can use a red-black tree here. By doing so, changing prices simply involves removing the original prices from the tree and inserting the new prices. This way, we can maintain the sorting of sweet dumplings types based on price differences in O(log N) time.

What about the total cost? By examining the structure of our red-black tree, we find that each subtree represents a range of tangyuan in our sequence. Perhaps we can record the total cost of each subtree below each node, and then recursively calculate the total cost up to the root to obtain the total cost of all sweet dumplings.

However, this approach encounters a problem: the parity of each node's position changes over time. For example, originally Alexander buys $b_1, a_2, b_3$, but one day the price difference of type 3 suddenly becomes larger than type 1. Then Alexander's purchases will change to $b_3, a_1, b_2$.

To solve this problem, we need to keep track of two pieces of information at each node:

1. The cost of Alexander as the first player in this segment.
2. The cost of Alexander as the second player in this segment.

By doing so, each time we change the prices, we can flexibly determine the current expenditure based on the updated first/second player status in each range. And the answer for each day will be the first player expenditure at the root node.

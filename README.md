# decision-tree-regression
\datedsubsection{CART决策树回归复杂度优化实现}{}
\begin{itemize}

\item 假设能够在每个树结点平均分割数据项，得到一颗完美二叉树，则树深为 O(logN) 层（N 为数据
项数目），树结点数为 O(N)。
\item version 1(python 实现)： 对回归树损失函数进行展开
$$
\sum_{x_i^{(d)}<s}(y_i-\hat c_1)^2+\sum_{x_i^{(d)}>s}(y_i-\hat c_2)^2=\sum_i^n y_i^2 - \frac{(\sum_{i=1}^m y_i)^2}{m}- \frac{(\sum_{i=m+1}^{n}y_i)^2}{n-m+1}
				$$
				对于每个树结点，将数据项按 x(d) i 排序，对 yi 和 yi2 前缀求和预处理后，枚举切分点时可以 O(1)
查表得到损失函数值。每层都会对 N 个数据项的进行 D 次排序, 共 O(logN) 层，若使用快速排
序，总复杂度为 O(DN log2 N) （D 为属性维度）。
\item version 2 (c++ 实现)：利用性质 1）父结点已排序。2）子结点数据项是父结点子集。在对某结点
排序后，计算数组 rank[d][id]（d 维上从数据项序号 id 到排序排名的映射），在子结点中通过 id
查询父结点 rank 表进行基数排序，基数排序复杂度为 O(父结点数据项数)。每层排序总复杂度为
O(上一层数据项总数)，复杂度为 O(DN log N)。
\item version 3 (c++ 实现)：时空优化，树结点数目为 O(N), 若每个结点都维护一个 rank[D][N]，version2
实际时空复杂度为 O(DN2)。利用性质 1）dfs 过程中，在栈中形成的 dfs 链，链长为 depth。2）
父结点 rank 数组在子结点递归返回后信息不再有用。我们可以构造全局数组 rank[depth][D][N],
只需要记录当前 dfs 链中各个树结点的 rank 映射表，空间复杂度降为 O(DN log N)。
\item version 4(c++ 实现) ：空间优化，建树时使用 bfs，每一层各树结点在一个 rank[D][N] 上建立一个双射（数据项序号-> 排名 +树结点区间左界），则子结点获取上层排名时，可查询上一层 rank表，减去父结点区间左界即可得到真实排名。利用全局滚动数组，空间减少到 O(DN)。



    \begin{table}[h]
		\centering
		\begin{tabular}{|c|c|c|c|c|c|c|}\hline
			(N,D)&(500,13)&(3000,20)&(10000,20)&(50000,20)&(100000,20)&(200000,20)\\\hline
			version1&0.152&3.242&11.853&&&\\
			version2&0.012&0.272&2.248&&&\\
			sklearn&0.003&0.073&0.644&23.005&137.024&\\
			version4&0.005&0.077&0.385&2.667&6.645&15.670\\\hline
			version3&0.005&0.067&0.274&2.179&4.573&12.254\\\hline
		\end{tabular}
		\caption{ 训练时间比较：version1-4、scikit-learn（单位：s）max_depth=32,min_samples_split=4}
		\label{tab:Margin_settings}
	\end{table}


\end{itemize}

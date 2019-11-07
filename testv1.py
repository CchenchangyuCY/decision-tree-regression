import numpy as np
from sklearn import datasets
from sklearn.model_selection import train_test_split
from sklearn import tree
import time


def build_tree(id, node_set, dep):
    isleaf[id] = 0
    sum = 0
    loss = 0
    ave = 0
    for e in node_set:
        sum = sum + y[e]
    ave = sum / len(node_set)
    tree_ave[id] = ave
    for e in node_set:
        loss = loss + (ave - y[e]) ** 2
    tree_loss[id] = loss
    if (len(node_set) <= 3):
        isleaf[id] = 1
        return 0
    if(dep == 32):
        isleaf[id] = 1
        return 0
    minLoss = 1e18
    next_cut = -1
    next_d = -1
    for d in range(D):
        L = []
        for e in node_set:
            L.append([x[e][d], y[e]])

        L.sort()
        n = len(L)

        linearsum = [0] * (n + 1);
        sqrsum = 0.0;
        for i in range(1, n + 1):
            linearsum[i] = linearsum[i - 1] + L[i - 1][1];
            sqrsum += L[i - 1][1] * L[i - 1][1];

        for i in range(n - 1):
            if(L[i][0] == L[i + 1][0]):
                continue;
            cut = (L[i][0] + L[i + 1][0]) / 2;
            newLoss = sqrsum - (linearsum[i + 1] * linearsum[i + 1]) / (i + 1) - (linearsum[n] - linearsum[i + 1]) * (linearsum[n] - linearsum[i + 1]) / (n - i - 1);
            if (newLoss < minLoss and newLoss < 0.90 * loss):
                next_d = d
                next_cut = cut
                minLoss = newLoss

    if (next_cut == -1):
        isleaf[id] = 1
        return 0
    L_node_set = []
    R_node_set = []

    for e in node_set:
        if (x[e][next_d] <= next_cut):
            L_node_set.append(e)
        else:
            R_node_set.append(e)
    tree_d[id] = next_d
    tree_cut[id] = next_cut
    build_tree(id * 2, L_node_set, dep + 1)
    build_tree(id * 2 + 1, R_node_set, dep + 1)
    return 0



def test(id, cur_x):
    if(isleaf[id] == 1):
        return tree_ave[id]
    cur_d = tree_d[id]
    cur_cut = tree_cut[id]
    if(cur_x[cur_d] <= cur_cut):
        return test(id * 2, cur_x)
    else:
        return test(id * 2 + 1, cur_x)

def cut_branch(id):
    if(isleaf[id] == 1):
        return
    cut_branch(id * 2)
    cut_branch(id * 2 + 1)
    loss = 0
    for i in range(cut_brunch_n):
        ret = test(1, cut_branch_x[i])
        loss = loss + (ret - cut_branch_y[i])**2
    isleaf[id] = 1
    may_loss = 0
    for i in range(cut_brunch_n):
        ret = test(1, cut_branch_x[i])
        may_loss = may_loss + (ret - cut_branch_y[i])**2
    if(may_loss > loss * 1.005):
        isleaf[id] = 0

tree_loss = {}
tree_ave = {}
tree_d = {}
tree_cut = {}
isleaf = {}
#data = datasets.load_boston()
#data = datasets.make_regression(n_samples=1000, n_features=15, n_targets=1)
data = datasets.make_friedman1(200000, 20, 0)
#data = datasets.load_diabetes()
#data = datasets.load
#x, testx, y, testy = train_test_split(data.data, data.target, test_size=0.2,random_state=3)
x, testx, y, testy = train_test_split(data[0], data[1], test_size=0.1,random_state=3)
#x, cut_branch_x, y, cut_branch_y = train_test_split(x, y, test_size=0.25,random_state=3)
#cut_brunch_n = len(cut_branch_x)

#data = datasets.make_friedman1(100, 5, 0)
#x = [[1], [2], [3], [4], [5], [6], [7], [8], [9], [10]]
#y = [5.56, 5.7, 5.91, 6.4, 6.8, 7.05, 8.9, 8.7, 9, 9.05]



N = len(x)
D = len(x[0])
testN = len(testx)
'''
start = time.time();
build_tree(1, range(N), 0)
end = time.time();
print(tree_d)
print(tree_cut)
print(tree_ave)
print(tree_loss)


rate = 0
ret = 0.0

print("mytime", end-start)

#cut_branch(1)

v = ((testy - testy.mean())**2).sum()
u = 0.0
for i in range(testN):
    ret = test(1, testx[i])
    rate = rate + abs(ret - testy[i]) / testy[i]
    u += (ret - testy[i])**2

print("myscore:", 1-u/v)
    #print(ret)
    #print(testy[i])
    #print(rate)
print("myerror:", rate / testN)
'''
start = time.time();
clf = tree.DecisionTreeRegressor(min_samples_split=4, max_depth=32, presort=True);

end = time.time();
clf.fit(x, y);
end = time.time();
print("score:", clf.score(testx, testy))

predicy = clf.predict(testx);

print("time:", end-start)
rate = 0;
u = 0
for i in range(testN):
    rate = rate + abs(predicy[i] - testy[i]) / abs(testy[i]);

print("error:", rate / testN)
'''
f = open("data.txt", "w+")
print(N, file=f)
print(D, file=f)
M = testN
print(M, file=f)
for d in range(D):
    for i in range(N):
        print(x[i][d], file=f)
for i in range(N):
    print(y[i], file=f)

for d in range(D):
    for i in range(M):
        print(testx[i][d], file=f)
for i in range(M):
    print(testy[i], file=f)
f.close()
'''
#include<bits/stdc++.h>
using namespace std;
const int maxn = 2e5 + 5;
const int maxd = 22;
const int maxdep = 33;
vector<vector<double> > data, testx;
vector<double> value, testy;
vector<int> node_set;
int leafcnt;
#define rank aak
int rank[maxdep][maxd][maxn];
int ranksize[maxdep][maxd];
int tmp[maxn];
double linearsum[maxn];
struct treenode {
	treenode * lc, * rc, *fa;
	double ave, loss, sum, cut;
	int cut_d;
	bool isleaf;
	//vector<unordered_map<int, int> > rank;
	treenode() {
		lc = rc = nullptr;
		cut_d = -1;
		cut = -1;
		sum = ave = loss = isleaf = 0;
	}
	treenode(treenode * fat, int D) {
		fa = fat;
		
		cut_d = -1;
		cut = -1;
		sum = ave = loss = isleaf = 0;
	}
	void sort(int l, int r, int d, int dep) {
		for(int i = 0; i < ranksize[dep - 1][d] + 5; i++) {
			tmp[i] = -1;
		}
		//vector<int> tmp((fa -> rank[d]).size() + 5, -1);
		for(int i = l; i <= r; i++) {
			tmp[rank[dep - 1][d][node_set[i]]] = node_set[i];
		}
		int cur = 0;
		for(int i = 0; i < ranksize[dep - 1][d]; i++) {
			if(tmp[i] == -1) {
				continue;
			}
			node_set[l + cur] = tmp[i];
			rank[dep][d][tmp[i]] = cur++;
		}
		ranksize[dep][d] = cur;
	}
	
};

struct decisionTree {
	
	treenode * root;
	int D, N;
	
	void init(int d, int n, treenode * fa) {
		D = d;
		N = n;
		root = build(0, n - 1, fa, 1);
	}
	
	
	treenode* build(int l, int r, treenode * fa, int dep) {
	
		treenode * u = new treenode(fa, D);
		u -> isleaf = 0;
		int cnt = r - l + 1;
		
		for(int i = l; i <= r; i++) {
			u -> sum = (u -> sum) + value[node_set[i]];
		//	cout << value[node_set[i]] << "\n";
		}
		u -> ave = u -> sum / cnt;
		for(int i = l; i <= r; i++) {
			u -> loss = u -> loss + (u -> ave - value[node_set[i]]) * (u -> ave - value[node_set[i]]);
		}
		if(cnt <= 3) {
			u -> isleaf = 1;
			leafcnt++;
			//cout << (u -> cut) << " " << (u -> cut_d) << "\n";
	    	//cout << (u -> ave) << " " << (u -> loss) << "\n";
			return u;
		}
		if(dep > 32) {
			u -> isleaf = 1;
			leafcnt++;
			//cout << (u -> cut) << " " << (u -> cut_d) << "\n";
	    	//cout << (u -> ave) << " " << (u -> loss) << "\n";
			return u;
		}	
		
		double minLoss = 1e18;
		double next_cut = -1;
		int next_d = -1;

		for(int d = 0; d < D; d++) {
			
			u -> sort(l, r, d, dep);
		
			linearsum[0] = 0.0;
        	double sqrsum = 0.0;
        	
        	for(int i = l; i <= r; i++) {
        		linearsum[i - l + 1] = linearsum[i - l] + value[node_set[i]];
				sqrsum += value[node_set[i]] * value[node_set[i]];
			}
        	//cout << "d" << d << "\n";
	        for (int i = l; i < r; i++) {
	            if(data[d][node_set[i]] == data[d][node_set[i + 1]])
	                continue;
	            double cut = (data[d][node_set[i]] + data[d][node_set[i + 1]]) / 2;
	            double newLoss = sqrsum - (linearsum[i - l + 1] * linearsum[i - l + 1]) / (i - l + 1) - (linearsum[r - l + 1] - linearsum[i - l + 1]) * (linearsum[r - l + 1] - linearsum[i - l + 1]) / (r - i);
	           	//cout << "cut" << cut << " " << newLoss << "\n";
			    if (newLoss < minLoss && newLoss < 0.999999 * (u -> loss)) {
	                next_d = d;
	                next_cut = cut;
	                minLoss = newLoss;
				}
			}
		}
	    if (next_cut == -1) {
	        u -> isleaf = 1;
	        leafcnt++;
	        return u;
		}
		
	    int mid = l - 1;
	    for (int i = l; i <= r; i++) {
	        if (data[next_d][node_set[i]] <= next_cut) {
	        	int tmp = node_set[i];
	        	node_set[i] = node_set[mid + 1];
	        	node_set[mid + 1] = tmp;
	        	//swap(node_set[i], node_set[mid + 1]);
	        	mid++;
			}
			
		}
		
	    u -> cut_d = next_d;
	    u -> cut = next_cut;
	    u -> lc = build(l, mid, u, dep + 1);
	    u -> rc = build(mid + 1, r, u, dep + 1);
	    //cout << (u -> cut) << " " << (u -> cut_d) << "\n";
	   // cout << (u -> ave) << " " << (u -> loss) << "\n";
	   	
	   	
		/*for(int j = 0; j < maxd; j++) {
			for(int k = 0; k < ranksize[dep][j]; k++) {
				rank[dep][j][k] = 0;
			}
			//rank[i][j] = new int[maxn]{};
		}*/
		return u;	
	}
				
			
	
	
	double predict(treenode * u, int id) {
		if(u -> isleaf == 1)
        	return u -> ave;
    	int cut_d = u -> cut_d;
    	double cut = u -> cut;
	    if(testx[cut_d][id] <= cut)
	        return predict(u -> lc, id);
	    else
	        return predict(u -> rc, id);
	}
}dt;
pair<double, int> pre[maxd][maxn];
int main() {
	freopen("C:/Users/makej/PycharmProjects/untitled/data.txt", "r", stdin);
	int N, D, M;
	cin >> N >> D >> M; 
	treenode * rootfa = new treenode(nullptr, D);
	/*for(int i = 0; i < maxdep; i++) {
		for(int j = 0; j < maxd; j++) {
			rank[i][j] = new int[maxn]{};
		}
	}*/
	data.resize(D);
	value.resize(N);
	
	for(int i = 0; i < D; i++) {
		data[i].resize(N);
		for(int j = 0; j < N; j++) {
			scanf("%lf", &data[i][j]);
			//cout << data[i][j] << "\n";
			pre[i][j] = make_pair(data[i][j], j);	
		}
			
	}
	
	for(int i = 0; i < N; i++) {
		scanf("%lf", &value[i]);
	}
	
	for(int i = 0; i < D; i++) {
		sort(pre[i], pre[i] + N);
		ranksize[0][i] = N;
		for(int j = 0; j < N; j++) {
			rank[0][i][pre[i][j].second] = j;
			//cout << tmp[i][j].second << " " << j << "\n";
		}
	}
	
	for(int i = 0; i < N; i++) {
		
		node_set.push_back(i);
	} 
	testy.resize(M);
	testx.resize(D);
	for(int i = 0; i < D; i++) {
		testx[i].resize(M);
		for(int j = 0; j < M; j++) {
			scanf("%lf", &testx[i][j]);
		
		}
			
	}
	
	for(int i = 0; i < M; i++) {
		scanf("%lf", &testy[i]);
	}
	
	clock_t startTime,endTime;
    startTime = clock();//计时开始
	dt.init(D, N, rootfa);
	endTime = clock();//计时结束
    cout << "The run time is: " <<(double)(endTime - startTime) / CLOCKS_PER_SEC << "s" << endl;
	double sum = 0, v = 0, u = 0, rate = 0;
	for(int i = 0; i < M; i++) {
		sum += testy[i];
	}
	for(int i = 0; i < M; i++) {
		v += pow(testy[i] - sum / M, 2);
	}
	for(int i = 0; i < M; i++) {
		double ret = dt.predict(dt.root, i);
		rate = rate + abs(ret - testy[i]) / testy[i];
		u += pow(ret - testy[i], 2);
	}
	
	cout << "score：" << 1 - u / v << "\n";
    cout << "error：" << rate / M << "\n";
     cout << "leaf：" << leafcnt;

}

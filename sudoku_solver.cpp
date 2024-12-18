#include <bits/stdc++.h>
using namespace std;
#define endl '\n'
template<class T, class repr>
struct simulated_annealing {
    T val;
    double get_time(){ return chrono::duration<double>(chrono::high_resolution_clock::now().time_since_epoch()).count(); }
    double P(double old_val, double new_val, double temp) { return exp(((old_val-new_val))/temp); }
    simulated_annealing(double time_limit, double t_i, double t_f, repr & q){
        q.init();
        double st = get_time(); val = q.eval();
        while (1){
            double elapsed = get_time()-st;
            if (elapsed > time_limit) break;
            double ela_frac = elapsed/time_limit;
            double temp = t_i*pow(t_f/t_i,ela_frac);
            q.get_neighbor();
            T new_val = q.eval();
            if (new_val < val || ((double)rand())/RAND_MAX < P(val,new_val,temp)){
                val = new_val;
            } else {
                q.rollback();
            }
        }
    }
};
struct ds {
    vector<int> cnt; int diff;
    ds(){ cnt.resize(9,0); diff = 0; }
    void add(int x){
        if (cnt[x-1] == 0) diff++;
        cnt[x-1]++;
    }
    void elim(int x){
        cnt[x-1]--;
        if (cnt[x-1] == 0) diff--;
    }
};
struct sudoku {
    vector<vector<int>> grid, arr, v1;
    int ans; vector<int> last;
    vector<ds> row, col;
    sudoku(){
        grid.assign(9,vector<int>());
        arr.assign(9,vector<int>(9,0));
        v1.assign(9,vector<int>(9));
        row.resize(9); col.resize(9);
        for (int i = 0; i < 9; i++)
            iota(v1[i].begin(),v1[i].end(),1);
        for (int i = 0; i < 9; i++)
            for (int j = 0; j < 9; j++){
                char c; cin >> c; 
                int d = c-'0', pos = 3*(i/3)+(j/3); arr[i][j] = d;
                if (d != 0) v1[pos].erase(remove(v1[pos].begin(),v1[pos].end(),d),v1[pos].end());
                else grid[pos].push_back(3*(i%3)+(j%3));
            }
    }
    pair<int,int> get_pos(int gr, int pl){ return make_pair(3*(gr/3)+(pl/3),3*(gr%3)+(pl%3)); }
    void init(){
        last = {-1,-1,-1};
        for (int i = 0; i < 9; i++){
            int m = v1[i].size();
            auto seed = chrono::system_clock::now().time_since_epoch().count();
            shuffle(v1[i].begin(),v1[i].end(),default_random_engine(seed));
            for (int j = 0; j < m; j++){
                int pos = grid[i][j], val = v1[i][j];
                auto [py,px] = get_pos(i,pos);
                arr[py][px] = val;
            }
        }
        ans = 0;
        for (int i = 0; i < 9; i++)
            for (int j = 0; j < 9; j++){
                row[i].add(arr[i][j]);
                col[j].add(arr[i][j]);
            }
        for (int i = 0; i < 9; i++)
            ans += (9-row[i].diff)+(9-col[i].diff);
    }
    int eval(){ return ans; }
    void get_neighbor(){
        if (ans == 0) return;
        int gr = -1;
        while (gr == -1 || grid[gr].size() < 2)
            gr = rand()%9;
        int m = grid[gr].size(), p2 = -1;
        int p1 = rand()%m;
        while (p2 == -1 || p2 == p1)
            p2 = rand()%m;
        p1 = grid[gr][p1], p2 = grid[gr][p2];
        auto [py1,px1] = get_pos(gr,p1); auto [py2,px2] = get_pos(gr,p2);
        ans -= (9-row[py1].diff)+(9-row[py2].diff)+(9-col[px1].diff)+(9-col[px2].diff);
        row[py1].elim(arr[py1][px1]); row[py2].elim(arr[py2][px2]);
        col[px1].elim(arr[py1][px1]); col[px2].elim(arr[py2][px2]);
        swap(arr[py1][px1],arr[py2][px2]);
        row[py1].add(arr[py1][px1]); row[py2].add(arr[py2][px2]);
        col[px1].add(arr[py1][px1]); col[px2].add(arr[py2][px2]);
        ans += (9-row[py1].diff)+(9-row[py2].diff)+(9-col[px1].diff)+(9-col[px2].diff);
        last = {gr,p1,p2};
    }
    void rollback(){
        int gr = last[0], p1 = last[1], p2 = last[2];
        auto [py1,px1] = get_pos(gr,p1); auto [py2,px2] = get_pos(gr,p2);
        ans -= (9-row[py1].diff)+(9-row[py2].diff)+(9-col[px1].diff)+(9-col[px2].diff);
        row[py1].elim(arr[py1][px1]); row[py2].elim(arr[py2][px2]);
        col[px1].elim(arr[py1][px1]); col[px2].elim(arr[py2][px2]);
        swap(arr[py1][px1],arr[py2][px2]);
        row[py1].add(arr[py1][px1]); row[py2].add(arr[py2][px2]);
        col[px1].add(arr[py1][px1]); col[px2].add(arr[py2][px2]);
        ans += (9-row[py1].diff)+(9-row[py2].diff)+(9-col[px1].diff)+(9-col[px2].diff);
        last = {-1,-1,-1};
    }
    void print(){
        for (auto v : arr){
            for (int c : v)
                cout << c;
            cout << endl;
        }
        cout << ans << endl;
    }
};
void solve(){
    double rep = 20, time_limit = 5.0;
    sudoku v1;
    for (int i = 0; i < 5; i++){
        simulated_annealing<int,sudoku> sim(time_limit/rep,100,0.01,v1);
        if (sim.val == 0){
            v1.print();
            break;
        }
    }
}
signed main(){
    ios_base::sync_with_stdio(0);
    cin.tie(0), cout.tie(0);
    srand(time(0));
    int t = 1;
    // cin >> t;
    while (t--)
        solve();
    return 0;
}
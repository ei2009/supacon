#include <bits/stdc++.h>
using namespace std;
int n,m;
int dy[4]={1,0,-1,0};
int dx[4]={0,1,0,-1};
int ps=0;
vector<vector<bool>> check(250, vector<bool>(250, false));
vector<vector<bool>> check2(250, vector<bool>(250, false));
vector<vector<char>> ma(250, vector<char>(250, ' '));
vector<string> s(2000);
vector<int> sn(2000, 0);
vector<pair<int,int>> go(2000);
vector<vector<pair<int, int>>> sr(2000, vector<pair<int, int>>(0));
vector<vector<pair<int, int>>> sr2(2000, vector<pair<int, int>>(0));
vector<pair<int,int>> ss(2000);
vector<bool> su(2000, false);
vector<bool> su2(2000, false);
vector<bool> sc(2000, true);
void dfs(int y, int x);
void sdfs(int y, int x);
signed main(){
    cin.tie(nullptr);
    ios_base::sync_with_stdio(false);
    cin >> n;
    vector<int> y(n), x(n);
    for(int i = 0; i < n; ++i){
        cin >> y[i] >> x[i];
        y[i]--;
        x[i]--;
        ma[y[i]][x[i]] = '#';
    }
    cin >> m;
    for(int i = 0; i < m; ++i){
        cin >> s[i];
        sn[i] = s[i].size();
        int ry = 0, rx = 0;
        for(int j = 0; j < s[i].size(); ++j){
            if(sc[i]){
                if(s[i][j] == 'U'){
                    ry--;
                }else if(s[i][j] == 'D'){
                    ry++;
                }else if(s[i][j] == 'L'){
                    rx--;
                }else if(s[i][j] == 'R'){
                    rx++;
                }
                if(ry <= -n || n <= ry || rx <= -n || n <= rx){
                    sc[i] = false;
                }
                sr[i].push_back(make_pair(ry, rx));
                if(j == s[i].size()-1){
                    go[i].first = ry;
                    go[i].second = rx;
                }
            }
        }
        if(sc[i]){
            sort(sr[i].begin(), sr[i].begin() + s[i].size());
            for(int j = s[i].size() - 1; j >= 1; --j){
                if(sr[i][j].first == sr[i][j - 1].first && sr[i][j].second == sr[i][j - 1].second){
                    sr[i].erase(sr[i].begin() + j - 1, sr[i].begin() + j - 1 + 1);
                    sn[i]--;
                }
            }
        }
    }
    for(int i = 0; i < m; ++i){
        if(sc[i]){
            ss[i].first = sn[i];
            ss[i].second = i;
        }else{
            ss[i].first = INT_MAX;
            ss[i].second = i;
        }
    }

    sort(ss.begin(), ss.begin() + m);
    // for(int i=0;i<m;i++){
    //     for(int j=0;j<sr[i].size();j++){
    //         cout<<sr[i][j].first<<' '<<sr[i][j].second<<'\n';
    //     }
    //     cout<<'\n';
    // }
    // cout<<'\n';
    // for(int i=0;i<m;++i){
    //     for(int j=0;j<sn[i];++j){
    //         cout<<sr[i][j].first<<' '<<sr[i][j].second<<'\n';
    //     }
    //     cout<<'\n';
    // }
    dfs(0, 0);
    // for(int i=0;i<n;++i){
    //     for(int j=0;j<n;++j){
    //         if(check[i][j]==true){
    //             cout<<1;
    //         }else{
    //             cout<<0;
    //         }
    //     }
    //     cout<<'\n';
    // }
    sdfs(0, 0);
    // for(int i=0;i<n;++i){
    //     for(int j=0;j<n;++j){
    //         if(check[i][j]==true){
    //             cout<<1;
    //         }else{
    //             cout<<0;
    //         }
    //     }
    //     cout<<'\n';
    // }
    for(int i = 0; i < n; ++i){
        for(int j = 0; j < n; ++j){
            if(check[i][j] == true){
                cout << "NO\n";
                return 0;
            }
        }
    }
    
    cout << "YES\n";
    int mc = 0;
    for(int i = 0; i < m; ++i){
        // cout<<su[i]<<" ";
        if(su[i] == true){
            mc++;
        }
    }
    cout << mc << '\n';
    for(int i = 0; i < m; ++i){
        if(su[i] == true){
            cout << i + 1 <<' ';
        }
    }
    cout << '\n';
    // for(int i=0;i<m;i++){
    //     for(int j=0;j<sn[i];j++){
    //         sr2[i].push_back(make_pair(sr[i][j].first-go[i].first,sr[i][j].second-go[i]));
    //     }
    // }
    return 0;
}

void dfs(int y, int x){
    check[y][x] = true;
    check2[y][x] = true;
    ps++;
    for(int i = 0; i < 4; ++i){
        int ny = y + dy[i], nx = x + dx[i];
        if(0 <= ny && ny < n && 0 <= nx && nx < n && check[ny][nx] == false && ma[ny][nx] != '#'){
            dfs(ny, nx);
        }
    }

    return;
}

void sdfs(int y, int x){
    check[y][x] = false;
    ps--;
    if(ps == 0){
        return;
    }
    for(int i = 0; i < m; ++i){
        int j, ny, nx, i2 = ss[i].second;
        if(sc[i2]){
            for(j = 0; j < sn[i2]; ++j){
                ny = y;
                nx = x;
                ny += sr[i2][j].first;
                nx += sr[i2][j].second;
                if(ny < 0 || n <= ny || nx < 0 || n <= nx || ma[ny][nx] == '#'){
                    break;
                }
            }
            ny = y + go[i2].first;
            nx = x + go[i2].second;
            if(j == sn[i2] && 0 <= ny && ny < n && 0 <= nx && nx < n && check[ny][nx] == true){
                // cout<<i2<<'\n';
                // cout<<y<<' '<<x<<'\n';
                su[i2] = true;
                sdfs(ny, nx);
            }
        }
    }
    return;
}
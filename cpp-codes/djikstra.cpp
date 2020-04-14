priority_queue<pii> q;
dis[0] = 0;
for (int i = 1; i < N; i++) dis[i] = inf;
q.push({0, 0});    // {-dis, node}
// g[i] -> {node, weight}
while (!q.empty()) {
    int t = q.top().second;
    q.pop();
    if (vis[t]) continue;
    vis[t] = 1;
    for (auto i: g[t]) {
        int w = i.second, a = i.first;
        if (dis[t] + w < dis[a]) {
            dis[a] = dis[t] + w;
            q.push({-dis[a], a});
        }
    }
}

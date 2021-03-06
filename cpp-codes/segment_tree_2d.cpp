struct SegTree2D
{
    struct data
    {
        int mn, val;

        // ----------------------
        data() : mn(1e9), val(0) {};
        template <typename T>
        void apply(T v) {
            val = v;
        }
    };

    int N, M;
    vector<vector<data>> st;

    SegTree2D() {}

    SegTree2D(int _N, int _M) : N(_N), M(_M) {
        st.resize(4 * N + 5);
        for (int i = 0; i < 4 * N + 5; i++) {
            st[i].resize(4 * M + 5);
        }
    }

    inline void merge(data &cur, const data &l, const data &r) 
    {
        // ----------------------
        // cur.mn = min(l.mn, r.mn);
        cur.val = l.val + r.val;
    }

    template <typename T>
    void buildy(int nodex, int lx, int rx, int nodey, int ly, int ry, vector<vector<T>> &v) {
        if (ly == ry) {
            if (lx == rx) st[nodex][nodey].apply(v[lx][ly]);
            else merge(st[nodex][nodey], st[nodex*2][nodey], st[nodex*2 + 1][nodey]);
            return;
        }
        int mid = (ly + ry) / 2;
        buildy(nodex, lx, rx, nodey*2, ly, mid, v);
        buildy(nodex, lx, rx, nodey*2+1, mid+1, ry, v);
        merge(st[nodex][nodey], st[nodex][nodey*2], st[nodex][nodey*2 + 1]);
    }

    template <typename T>
    void buildx(int nodex, int lx, int rx, vector<vector<T>> &v)
    {
        if(lx == rx)
        {
            buildy(nodex, lx, rx, 1, 1, M, v);
            return;
        }
        int mid = (lx + rx) / 2;
        buildx(nodex*2, lx, mid, v);
        buildx(nodex*2 + 1, mid + 1, rx, v);
        buildy(nodex, lx, rx, 1, 1, M, v);
    }

    data queryy(int nodex, int nodey, int ly, int ry, int sty, int endy) {
        if (endy < ly || sty > ry)
            return data();
        if (sty <= ly && ry <= endy)
            return st[nodex][nodey];
        int mid = (ly + ry) / 2;
        data left = queryy(nodex, nodey*2, ly, mid, sty, endy);
        data right = queryy(nodex, nodey*2+1, mid+1, ry, sty, endy);
        data cur;
        merge(cur, left, right);
        return cur;
    }

    data queryx(int nodex, int lx, int rx, int stx, int endx, int sty, int endy)
    {
        if (endx < lx || stx > rx)
            return data();
        if (stx <= lx && rx <= endx)
            return queryy(nodex, 1, 1, M, sty, endy);
        int mid = (lx + rx)/2;
        data left = queryx(nodex*2, lx, mid, stx, endx, sty, endy);
        data right = queryx(nodex*2 + 1, mid + 1, rx, stx, endx, sty, endy);
        data cur;
        merge(cur, left, right);
        return cur;
    }

    data pqueryy(int nodex, int nodey, int ly, int ry, int posy) {
        if (ly == ry)
            return st[nodex][nodey];
        int mid = (ly + ry) / 2;
        if (posy <= mid)
            return pqueryy(nodex, nodey*2, ly, mid, posy);
        else
            return pqueryy(nodex, nodey*2+1, mid+1, ry, posy);
    }

    data pqueryx(int nodex, int lx, int rx, int posx, int posy)
    {
        if(lx == rx)
            return pqueryy(nodex, 1, 1, M, posy);
        int mid = (lx + rx) / 2;
        if(posx <= mid)
            return pqueryx(nodex*2, lx, mid, posx, posy);
        else
            return pqueryx(nodex*2+1, mid+1, rx, posx, posy);
    }    

    void pupdatey(int nodex, int lx, int rx, int nodey, int ly, int ry, int posy, int val) {
        if (ly == ry) {
            // --------------------
            if (lx == rx) st[nodex][nodey].apply(val);
            else merge(st[nodex][nodey], st[nodex*2][nodey], st[nodex*2+1][nodey]);
            return;
        }
        int mid = (ly + ry) / 2;
        if (posy <= mid)
            pupdatey(nodex, lx, rx, nodey*2, ly, mid, posy, val);
        else
            pupdatey(nodex, lx, rx, nodey*2+1, mid+1, ry, posy, val);
        merge(st[nodex][nodey], st[nodex][nodey*2], st[nodex][nodey*2+1]);
    }

    void pupdatex(int nodex, int lx, int rx, int posx, int posy, int val)
    {
        if(lx == rx) {
            pupdatey(nodex, lx, rx, 1, 1, M, posy, val);
            return;
        }
        int mid = (lx + rx)/2;
        if(posx <= mid)
            pupdatex(nodex*2, lx, mid, posx, posy, val);
        else
            pupdatex(nodex*2 + 1, mid + 1, rx, posx, posy, val);
        pupdatey(nodex, lx, rx, 1, 1, M, posy, val);
    }

    template <typename T>
    void build(vector<vector<T>> &v) {
        buildx(1, 1, N, v);
    }

    data query(int stx, int endx, int sty, int endy)
    {
        return queryx(1, 1, N, stx, endx, sty, endy);
    }

    data query(int posx, int posy)
    {
        return pqueryx(1, 1, N, posx, posy);
    }

    void update(int posx, int posy, int val)
    {
        pupdatex(1, 1, N, posx, posy, val);
    }
};

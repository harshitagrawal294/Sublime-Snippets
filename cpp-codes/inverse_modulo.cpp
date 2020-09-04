
// Works when number and MOD are coprime

int gcdExtended(int a, int b, int &x, int &y)
{
    if(a == 0)
    {
        x = 0;
        y = 1;
        return b;
    }
    int x1, y1;
    int d = gcdExtended(b%a, a, x1, y1);
    x = y1 - (b/a)*x1;
    y = x1;
    return d;
}

int power(int x, int y) 
{ 
    int res = 1;
    x = x%MOD;
    while(y!=0) {
        if(y&1)
            res = (res*x)%MOD;
        y = y>>1;
        x = (x*x)%MOD;
    }
    return res; 
}

int inverse(int a)
{
    int x, y;
    int g = gcdExtended(a, MOD, x, y);
    if(g!=1) return -1;
    else return (x % MOD + MOD) % MOD;
}

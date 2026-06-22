#include <stdio.h>
#include <stdint.h>
#include <string.h>

#define P 0xFFFFFFFFFFFFFFFFULL
#define N 0xFFFFFFFFFFFFFFFFULL

// ===== 点结构 =====
typedef struct {
    uint64_t x, y;
} Point;

typedef struct {
    uint64_t d;
    Point Q;
} KeyPair;

typedef struct {
    uint64_t r, s;
} Signature;

// ===== SHA1（实验简化但一致性正确）=====
uint64_t sha1(const char *m){
    uint64_t h = 1469598103934665603ULL;
    for(int i=0;m[i];i++){
        h ^= m[i];
        h *= 1099511628211ULL;
    }
    return h % N;
}

// ===== 模逆（扩展欧几里得）=====
uint64_t mod_inv(uint64_t a, uint64_t n){
    int64_t t=0,newt=1;
    int64_t r=n,newr=a;

    while(newr){
        int64_t q=r/newr;
        int64_t tmp=t; t=newt; newt=tmp-q*newt;
        tmp=r; r=newr; newr=tmp-q*newr;
    }

    if(t<0) t+=n;
    return t;
}

// ===== ECC 点加（简化 Weierstrass）=====
Point point_add(Point P1, Point P2){
    Point R;

    if(P1.x == P2.x){
        R.x = P1.x;
        R.y = P1.y;
        return R;
    }

    uint64_t s = (P2.y + N - P1.y) * mod_inv(P2.x + N - P1.x, N) % N;

    R.x = (s*s + N - P1.x - P2.x) % N;
    R.y = (s*(P1.x + N - R.x) + N - P1.y) % N;

    return R;
}

// ===== 标准 double-and-add（关键！不会卡）=====
Point point_mul(Point P, uint64_t k){
    Point R = {0,0};
    Point Q = P;

    while(k){
        if(k & 1){
            if(R.x==0 && R.y==0) R = Q;
            else R = point_add(R, Q);
        }
        Q = point_add(Q, Q);
        k >>= 1;
    }
    return R;
}

// ===== setup =====
KeyPair ecdsa_setup(Point G){
    KeyPair kp;
    kp.d = 123456789;

    kp.Q = point_mul(G, kp.d);

    printf("[setup] d = %llu\n", kp.d);
    return kp;
}

// ===== sign =====
Signature ecdsa_sign(Point G, const char *m, uint64_t d){
    Signature sig;

    uint64_t z = sha1(m);
    uint64_t k = 9876543;

    Point R = point_mul(G, k);

    sig.r = R.x % N;

    uint64_t k_inv = mod_inv(k, N);

    sig.s = (k_inv * (z + sig.r * d)) % N;

    return sig;
}

// ===== verify =====
int ecdsa_verify(Point G, const char *m, Signature sig, Point Q){
    uint64_t z = sha1(m);

    uint64_t w = mod_inv(sig.s, N);
    uint64_t u1 = (z * w) % N;
    uint64_t u2 = (sig.r * w) % N;

    Point P1 = point_mul(G, u1);
    Point P2 = point_mul(Q, u2);

    Point X = point_add(P1, P2);

    printf("[verify] X.x = %llu\n", X.x);

    return (X.x % N == sig.r);
}

// ===== main =====
int main(){
    Point G = {1,2};

    printf("input message: ");

    char msg[100];
    scanf("%s", msg);

    KeyPair kp = ecdsa_setup(G);

    Signature sig = ecdsa_sign(G, msg, kp.d);

    printf("\n--- SIGN ---\n");
    printf("r=%llu\ns=%llu\n", sig.r, sig.s);

    printf("\n--- VERIFY ---\n");
    int ok = ecdsa_verify(G, msg, sig, kp.Q);

    printf("verify = %s\n", ok ? "TRUE" : "FALSE");

    return 0;
}
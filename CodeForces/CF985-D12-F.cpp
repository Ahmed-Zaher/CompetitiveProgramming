/*
 * For the given string find occur[i][j], which is 1 if s[j] = i
 * and 2 if s[j] != i, i.e. the array occur[i][0..n-1] marks the positions
 * where we get the character i. Now two strings S, T are isomorphic iff
 * we find occur_S, occur_T and we find a one to one mapping between
 * arrays in occur_S, arrays in occur_T, we can check that by sorting occur_S,
 * occur_T (we put bigger arrays before smaller arrays, using lexicographical
 * comparison, but the order of elements inside each array doesn't change)
 * and check if occur_S = occur_T. This can be done efficiently by hashing
 * occur[i][x..x+len-1], occur[i][y..y+len-1] into a single number, and do
 * that for all i's, hence we get a set 26 numbers from x and a set 26 numbers from y,
 * we then check if the 2 sets are equal
 */
#include <bits/stdc++.h>

using namespace std;

const int OO = 1e9;
const double EPS = 1e-9;

#define ndl cout << '\n'
#define sz(v) int(v.size())
#define pb push_back
#define mp make_pair
#define fs first
#define sc second
#define present(a, x) (a.find(x) != a.end())
#ifdef LOCAL
#define db(...) ({cout << "> Line " << __LINE__  \
		<< ": "; _db(#__VA_ARGS__, __VA_ARGS__);})
#define RNG() rng()
#else
#define db(...) true
#define RNG() true
#endif

template<class T>
void _db(const char *dbStr, T e) {
	cout << dbStr << " = " << e << endl;
}
template<class T, class... L>
void _db(const char *dbStr, T e, L... r) {
  while(*dbStr != ',') cout << *dbStr++;
  cout << " = " << e << ','; _db(dbStr + 1, r...);
}
template<class S, class T>
ostream& operator<<(ostream& o, const map<S, T>& v) {
	o << "["; int i = 0;
	for (const pair<S, T>& pr : v) o << (!i++ ? "" : ", ") << "{"
	<< pr.fs << " : " << pr.sc << "}"; return o << "]";
}
template<template <class, class...> class S, class T, class... L>
ostream& operator<<(ostream& o, const S<T, L...>& v) {
	o << "["; int i = 0;
	for (const auto& e : v) o << (!i++ ? "" : ", ") << e;
	return o << "]";
}
template<class S, class T>
ostream& operator<<(ostream& o, const pair<S, T>& pr) {
	return o << "(" << pr.fs << ", " << pr.sc << ")";
}
ostream& operator<<(ostream& o, const string& s) {
	for (const char& c : s) o << c;
	return o;
}

template<class T> using V = vector<T>;
template<class T> using VV = V<V<T>>;
template<class T> using VVV = VV<V<T>>;
using ll = long long;
using pii = pair<int, int>;
using vi = V<int>;
using vii = V<pii>;
using vvi = VV<int>;
using mii = map<int, int>;
using umii = unordered_map<int, int>;
using si = set<int>;
using usi = unordered_set<int>;

struct hsh {
	int h0, h1, h2;
	hsh() {
		h0 = h1 = h2 = 0;
	}
	int& operator[](int I) {
		return !I ? h0 : (I == 1 ? h1 : h2);
	}
	bool operator==(const hsh& oth) {
		return h0 == oth.h0 && h1 == oth.h1 && h2 == oth.h2;
	}
};

const int MX = 2e5 + 5, HASH_CNT = 3;
int p[] = {3, 5, 7}, pPw[MX][HASH_CNT], pPwInv[MX][HASH_CNT];
int M[] = {1000000009, 1000000007, 998244353};
VV<hsh> prefHash(26, V<hsh>(MX));


template<class T>
vector<hsh> computePrefHash(T& S) {
	int n = sz(S);
	vector<hsh> prefHash(n);
	for (int i = 0; i < sz(S); ++i)
			for (int I = 0; I < HASH_CNT; ++I)
				prefHash[i][I] = ((i ? prefHash[i - 1][I] : 0) +
						S[i] * 1LL * pPw[i][I]) % M[I];
    return prefHash;
}

ll inv(ll a, ll b) {
	return 1 < a ? b - inv(b % a, a) * b / a : 1;
}


int n, m;
string s;
vvi v;


int main() {
	#ifdef LOCAL
	auto stTime = clock();
	freopen("in.txt", "r", stdin);
	mt19937_64 rng(chrono::steady_clock::now().time_since_epoch().count());
	#endif
	ios::sync_with_stdio(false);
	cout.precision(10);
	cin.tie(0);


	for (int I = 0; I < HASH_CNT; ++I) {
		pPw[0][I] = 1;
		pPwInv[0][I] = inv(1, M[I]);
	}
	for (int i = 1; i < MX; ++i)
		for (int I = 0; I < HASH_CNT; ++I) {
			pPw[i][I] = (p[I] * 1LL * pPw[i - 1][I]) % M[I];
			pPwInv[i][I] = inv(pPw[i][I], M[I]);
		}

	cin >> n >> m >> s;

	v.assign(26, vi(n, 1));


	for (int i = 0; i < n; ++i)
		v[s[i] - 'a'][i] = 2;

	for (int i = 0; i < 26; ++i)
		prefHash[i] = computePrefHash(v[i]);


	for (int i = 1; i <= m; ++i) {
		int x, y, len;
		cin >> x >> y >> len;
		--x, --y;
		vvi sx(3), sy(3);


		for (int I = 0; I < 3; ++I) {
			for (int ch = 0; ch < 26; ++ch) {
				int xx = ((prefHash[ch][x + len - 1][I] - (x ? prefHash[ch][x - 1][I] : 0)) * 1LL *	pPwInv[x][I]) % M[I];
				int yy = ((prefHash[ch][y + len - 1][I] - (y ? prefHash[ch][y - 1][I] : 0)) * 1LL *	pPwInv[y][I]) % M[I];
				while (xx < 0)
					xx += M[I];
				while (yy < 0)
					yy += M[I];
				sx[I].pb(xx);
				sy[I].pb(yy);
			}
			sort(sx[I].begin(), sx[I].end());
			sort(sy[I].begin(), sy[I].end());
		}

		cout << (sx == sy ? "YES\n" : "NO\n");
	}

	#ifdef LOCAL
	cout << "\n\n\nExecution time: " <<
		(clock() - stTime) * 1e3 / CLOCKS_PER_SEC << " ms" << endl;
	#endif
	return 0;
}

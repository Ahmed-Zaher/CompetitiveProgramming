/*
 * The outcomes in this experiment is a the set of 'states' for every tree,
 * which is either fall left or right or not fall at all.
 * Let's define a random variable P_j which denotes the power gained by j'th
 * mushroom, clearly if some tree crushes j'th mushroom then P_j = 0, otherwise P_j = z_j
 * Our ans = Exp(sum of P_j for j in [1,m]) = sum of Exp(P_j)
 * It's clear that Exp(P_j) = z_j * s_j, where s_j is the probability that j'th mushroom isn't crushed
 * s_j = Prob(1st tree doesn't crush j'th mushroom
 * 		  and 2nd tree doesn't crush j'th mushroom
 * 		  	  ..
 * 		  and n'th tree doesn't crush j'th mushroom)
 * Since trees are independent, we get
 * s_j = Prob(1st tree doesn't crush j'th mushroom)
 * 	   * Prob(2nd tree doesn't crush j'th mushroom)
 * 	   ..
 * 	   = Product of t_ij for i in [1, n]
 * 	   where t_ij is probability that i'th tree won't crush j'th mushroom
 * If i'th tree can crush j'th mushroom by falling to the right then t_ij = 1 - r[i]
 * Similarly for the falling to the left case
 * We can do a left to right scan, maintaining the set of currently active trees trees that
 * will crush current mushroom if it falls to the right, and add those t_ij to s_j, then in
 * the same way do another right to left scan.
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

struct Tree {
	int a, h, l, r;

	bool operator<(Tree oth) {
		return a < oth.a;
	}

};

struct Mushroom {
	int b, z;

	bool operator<(Mushroom oth) {
		return b < oth.b;
	}
};

int n, m;
V<double> s;


int main() {
	#ifdef LOCAL
	auto stTime = clock();
//	freopen("in.txt", "r", stdin);
	mt19937_64 rng(chrono::steady_clock::now().time_since_epoch().count());
	#endif
	ios::sync_with_stdio(false);
	cout.precision(20);
	cin.tie(0);

	cin >> n >> m;

	V<Tree> tr(n + 1);

	for (int i = 1; i <= n; ++i)
		cin >> tr[i].a >> tr[i].h >> tr[i].l >> tr[i].r;

	V<Mushroom> mu(m + 1);


	for (int i = 1; i <= m; ++i)
		cin >> mu[i].b >> mu[i].z;

	sort(tr.begin() + 1, tr.end());
	sort(mu.begin() + 1, mu.end());

	s.assign(m + 1, 0);

	{
		int p = 1;

		set<pair<ll, int>> L;

		double totR = 0;
		int bad = 0;

		for (int j = 1; j <= m; ++j) {

			db(j, p, mu[j].b, tr[p].a);
			while (p <= n && tr[p].a < mu[j].b) {
				L.insert(mp(tr[p].a * 1LL + tr[p].h, p));
				if (tr[p].r == 100)
					++bad;
				else
					totR += log2(1 - tr[p].r / 100.0);
				db(totR);
				++p;
			}

			while (!L.empty() && L.begin()->fs < mu[j].b) {
				if (tr[L.begin()->sc].r == 100)
					--bad;
				else
					totR -= log2(1 - tr[L.begin()->sc].r / 100.0);
				L.erase(L.begin());
			}

			db(totR);

			if (bad)
				s[j] = -1e18;
			else
				s[j] += totR;

		}
	}

	{
		int p = n;

		set<pair<ll, int>> R;

		double totL = 0;
		int bad = 0;

		for (int j = m; j >= 1; --j) {
			while (p >= 1 && tr[p].a > mu[j].b) {
				R.insert(mp(tr[p].a * 1LL - tr[p].h, p));
				if (tr[p].l == 100)
					++bad;
				else
					totL += log2(1 - tr[p].l / 100.0);
				--p;
			}

			while (!R.empty() && R.rbegin()->fs > mu[j].b) {
				if (tr[R.rbegin()->sc].l == 100)
					--bad;
				else
					totL -= log2(1 - tr[R.rbegin()->sc].l / 100.0);
				R.erase(--R.end());
			}

			if (bad)
				s[j] = -1e18;
			else
				s[j] += totL;

		}
	}


	double ans = 0;

	for (int j = 1; j <= m; ++j)
		if (s[j] > -1e10)
			ans += mu[j].z * pow(2, s[j]);

	cout << fixed << ans << '\n';

	#ifdef LOCAL
	cout << "\n\n\nExecution time: " <<
		(clock() - stTime) * 1e3 / CLOCKS_PER_SEC << " ms" << endl;
	#endif
	return 0;
}

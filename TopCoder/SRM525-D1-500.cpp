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



class Rumor {
public:
	int getMinimum(string knowledge, vector<string> graph) {
		int n = sz(knowledge);
		int ans = OO;
		for (int msk = 0; msk < (1 << n); ++msk) {
			vvi got(n, vi(2, 0));
			for (int i = 0; i < n; ++i) {
				if (knowledge[i] == 'Y')
					got[i][0] = got[i][1] = 1;
			}
			vvi done(n, vi(2, 0));
			int score = 0;
//			db(got, done);
			while (true) {
				bool change = false;
				vvi gotNew = got;
				for (int i = 0; i < n; ++i) {
					if (done[i][0] && done[i][1])
						continue;

					int toSpread = (msk & (1 << i)) > 0;

					if (done[i][toSpread])
						toSpread = !toSpread;

					if (!got[i][toSpread])
						continue;

					for (int j = 0; j < n; ++j) {
						if (graph[i][j] == 'Y') {
							if (gotNew[j][toSpread])
								continue;

							gotNew[j][toSpread] = 1;
							change = true;
						}
					}

					done[i][toSpread] = 1;
				}
				got = gotNew;

				if (!change)
					break;

				++score;
			}

			for (int i = 0; i < n; ++i) {
				if (!got[i][0] || !got[i][1])
					return -1;
			}

			ans = min(ans, score);
		}
		if (ans == OO)
			return -1;
		return ans;
	}
};


int main() {
	#ifdef LOCAL
	auto stTime = clock();
//	freopen("in.txt", "r", stdin);
	mt19937_64 rng(chrono::steady_clock::now().time_since_epoch().count());
	#endif
	ios::sync_with_stdio(false);
	cout.precision(10);
	cin.tie(0);



	#ifdef LOCAL
	cout << "\n\n\nExecution time: " <<
		(clock() - stTime) * 1e3 / CLOCKS_PER_SEC << " ms" << endl;
	#endif
	return 0;
}

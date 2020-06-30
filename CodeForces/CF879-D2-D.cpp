/*
 * Note that if k >= n then if the sequence a has all elements the same,
 * then the answer is (n*m) % k, otherwise it is 0, now assume k < n
 * Let's think of the sequence a as a sequence of segments, each
 * segment has a value length. We note that any length of length l
 * can be replaced with a segment of length l % k
 * We can simulate this process of taking a sequence of length n,
 * and return the sequence in which we can no longer find k consecutive
 * elements of the same value. We can do that using a stack in O(n)
 * After we do that to a, now let's consider the cases where the k elements
 * we remove a suffix from a sequence and a prefix from the next
 * We can use the same idea one suffixes and prefixes and note that
 * the resulting sequences would one of special cases that are easy to handle
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

int n, k, m;
vi a;

vii segs(vi v) {
	int n = sz(v);

	vii ret;

	int j = 0;
	while (j < n) {
		ret.pb(mp(v[j++], 1));
		while (j < n && v[j] == ret.back().fs)
			++ret.back().sc, ++j;
	}

	return ret;

}

deque<pii> fix(vii s) {
	deque<pii> dq;
	for (int i = 0; i < sz(s); ++i) {
		if (s[i].sc % k == 0)
			continue;
		if (!dq.empty() && dq.back().fs == s[i].fs) {
			int f = dq.back().sc;
			dq.pop_back();
			f = (f + s[i].sc) % k;
			if (f)
				dq.push_back(mp(s[i].fs, f));
		} else
			dq.push_back(mp(s[i].fs, s[i].sc % k));
	}
	return dq;
}

int len(deque<pii> dq) {
	int ret = 0;
	for (auto& elem : dq)
		ret += elem.sc;
	return ret;
}

int main() {
	#ifdef LOCAL
	auto stTime = clock();
	freopen("in.txt", "r", stdin);
	mt19937_64 rng(chrono::steady_clock::now().time_since_epoch().count());
	#endif
	ios::sync_with_stdio(false);
	cout.precision(10);
	cin.tie(0);

	cin >> n >> k >> m;

	vi a(n);

	for (int i = 0; i < n; ++i)
		cin >> a[i];


	deque<pii> dq = fix(segs(a));

	if (dq.empty()) {
		cout << "0\n";
		return 0;
	}

	n = len(dq);


	if (m == 1) {
		cout << n << '\n';
		return 0;
	}

	vii pref, suff;

	while (sz(dq) >= 2 && dq.front().fs == dq.back().fs && (dq.front().sc + dq.back().sc) % k == 0)
		dq.pop_front(), dq.pop_back();

	assert(!dq.empty());

	int nn = len(dq);

	ll ans = nn * 1LL * m + (n - nn);

	if (sz(dq) >= 2) {

		if (sz(dq) >= 2 && dq.front().fs == dq.back().fs && (dq.front().sc + dq.back().sc) >= k)
			ans -= (m - 1) * 1LL * k;

	} else {
		if ((nn * 1LL * m) % k == 0)
			ans = 0;
		else
			ans = (nn * 1LL * m) % k + (n - nn);
	}

	cout << ans << '\n';


	#ifdef LOCAL
	cout << "\n\n\nExecution time: " <<
		(clock() - stTime) * 1e3 / CLOCKS_PER_SEC << " ms" << endl;
	#endif
	return 0;
}

/*
 * Consider one wall of the polygon, which positions of the camera
 * would cover that wall? these are all positions in the polygon that
 * are to the right of that wall (assuming wall is oriented in CW order)
 * So initially we have valid camera positions which are all of the polygon,
 * then we remove parts that aren't valid by cutting the current valid area with
 * every wall. If the valid area becomes zero then we know we have no valid camera position.
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


template<class T>
struct Point {
	T x, y;
	Point(T _x = 0, T _y = 0) : x(_x), y(_y) {}
	bool operator<(Point<int> oth) const {
		return mp(x, y) < mp(oth.x, oth.y);
	}
	bool operator<(Point<double> oth) const {
		if (fabs(x - oth.x) > EPS)
			return x < oth.x;
		return y < oth.y;
	}
	bool operator>(Point<T> oth) const {
		return oth < *this;
	}
	bool operator<=(Point<int> oth) const {
		return mp(x, y) <= mp(oth.x, oth.y);
	}
	bool operator>=(Point<int> oth) const {
		return oth <= *this;
	}
	bool operator==(Point<int> oth) const {
		return mp(x, y) == mp(oth.x, oth.y);
	}
	bool operator==(Point<double> oth) const {
		return fabs(x - oth.x) < EPS
				&& fabs(y - oth.y) < EPS;
	}
	bool operator!=(Point<T> oth) const {
		return !(*this == oth);
	}
	// + and - convert to type of LHS
	template<class S>
	Point<T> operator+(Point<S> oth) const {
		return Point<T>(x + oth.x, y + oth.y);
	}
	template<class S>
	Point<T> operator-(Point<S> oth) const {
		return Point<T>(x - oth.x, y - oth.y);
	}
	Point<double> scale(double sc) {
		return Point<double>(x * sc, y * sc);
	}
};

template<class T>
Point<T> toVec(Point<T> p1, Point<T> p2) {
	return p2 - p1;
}

ll cross(Point<int> a, Point<int> b) {
	return a.x * 1LL * b.y - a.y * 1LL * b.x;
}

ll toLeft(Point<int> p, Point<int> q, Point<int> s) {
	return cross(toVec(p, q), toVec(p, s));
}

template<class T>
vector<Point<int>> cutPolygon(Point<T> a, Point<T> b, vector<Point<T>>& Q) {
	vector<Point<int>> P;
	Q.pb(Q[0]);
	for (int i = 0; i < sz(Q) - 1; i++) {
		ll left = toLeft(a, b, Q[i]);
		ll nextLeft = toLeft(a, b, Q[i + 1]);
		if (left <= 0)
			P.pb(Q[i]);
		else if (nextLeft < 0) {
			if (a.y == b.y)
				P.pb(Point<int>(Q[i].x, a.y));
			else
				P.pb(Point<int>(a.x, Q[i].y));
		}
	}
	Q.pop_back();
	return P;
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

	int n;
	int tc = 0;

	while (cin >> n && n) {
		V<Point<int>> Q(n);

		for (int i = 0; i < n; ++i)
			cin >> Q[i].x >> Q[i].y;

		V<Point<int>> P = Q;

		Q.pb(Q[0]);

		bool bad = false;

		for (int i = 0; i < n; ++i) {
			P = cutPolygon(Q[i], Q[i + 1], P);
			if (P.empty()) {
				bad = true;
				break;
			}
		}

		cout << "Floor #" << ++tc << '\n';

		cout << (bad ? "Surveillance is impossible.\n\n" : "Surveillance is possible.\n\n");
	}



	#ifdef LOCAL
	cout << "\n\n\nExecution time: " <<
		(clock() - stTime) * 1e3 / CLOCKS_PER_SEC << " ms" << endl;
	#endif
	return 0;
}

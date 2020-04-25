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

#define PI acos(-1.0)

double DEG_to_RAD(double d) {
	return d * PI / 180.0;
}

double RAD_to_DEG(double r) {
	return r * 180.0 / PI;
}

struct Vector {
	double x, y;
	Vector(double _x = 0, double _y = 0) : x(_x), y(_y) {}
	Vector operator+(Vector oth) {
		return Vector(x + oth.x, y + oth.y);
	}
	Vector operator/(double c) {
		return Vector(x / c, y / c);
	}
};

struct Node {
	Vector v;
	double add;
	int len;
	Node(Vector _v = Vector(0, 0), double _add = 0, int _len = 0) :
			v(_v), add(_add), len(_len) {}
};


Vector rotate(Vector v, double theta) {
	double rad = DEG_to_RAD(theta);
	return Vector(v.x * cos(rad) - v.y * sin(rad),
			v.x * sin(rad) + v.y * cos(rad));
}

vector<int> l;

struct SegmentTree {
	vector<Node> t;
	int n;

	void push(int v) {
		t[v << 1].add += t[v].add;
		t[v << 1].v = rotate(t[v << 1].v, t[v].add);

		t[v << 1 | 1].add += t[v].add;
		t[v << 1 | 1].v = rotate(t[v << 1 | 1].v, t[v].add);
		t[v].add = 0;
	}
	Node combine(Node& valL, Node& valR) {
		return Node(valL.v + valR.v,
					0,
					valL.len + valR.len);
	}
	Node upd(Node val, double add) {
		val.v = rotate(val.v, add);
		val.add += add;
		return val;
	}
	void build(int v, int tl, int tr) {
		if (tl == tr)
			t[v] = Node(Vector(0, l[tl]), 0, l[tl]);
		else {
			int tm = (tl + tr) >> 1;
			build(v << 1, tl, tm);
			build(v << 1 | 1, tm + 1, tr);
			Node valL = t[v << 1], valR = t[v << 1 | 1];
			t[v] = combine(valL, valR);
		}
	}
	Node query(int v, int tl, int tr, int l, int r) {
		if (l > r)
			return Node();
		if (l == tl && r == tr)
			return t[v];
		push(v);
		int tm = (tl + tr) >> 1;
		Node valL = query(v << 1, tl, tm, l, min(r, tm));
		Node valR = query(v << 1 | 1, tm + 1, tr, max(l, tm + 1), r);
		return combine(valL, valR);
	}
	void lazyUpdate(int v, int tl, int tr, int l, int r, double newVal) {
		if (l > r)
			return;
		if (l == tl && tr == r)
			t[v] = upd(t[v], newVal);
		else {
			push(v);
			int tm = (tl + tr) >> 1;
			lazyUpdate(v << 1, tl, tm, l, min(r, tm), newVal);
			lazyUpdate(v << 1 | 1, tm + 1, tr, max(l, tm + 1), r, newVal);
			Node valL = t[v << 1], valR = t[v << 1 | 1];
			t[v] = combine(valL, valR);
		}
	}
	void init(int _n) {
		n = _n;
		t.assign(n << 2, Node());
		build(1, 0, n - 1);
	}
	SegmentTree(int _n) {
		init(_n);
	}
	Node query(int i, int j) {
		return query(1, 0, n - 1, i, j);
	}
	void lazyUpdate(int l, int r, double newVal) {
		lazyUpdate(1, 0, n - 1, l, r, newVal);
	}
};




int main() {
	#ifdef LOCAL
	freopen("in.txt", "r", stdin);
	#endif
	ios::sync_with_stdio(false);
	cout.precision(2);
	cin.tie(0);

	cout << fixed;

	int n, q;

	int tc = 0;

	while (cin >> n >> q) {

		if (tc++)
			ndl;

		l.assign(n + 1, 0);

		for (int i = 1; i <= n; ++i)
			cin >> l[i];

		if (n == 1) {
			continue;
		}

		SegmentTree ST(n + 1);


		for (int i = 1; i <= q; ++i) {
			int idx, a;
			cin >> idx >> a;
			a -= 180;
			Vector v1, v2;

			v1 = ST.query(idx, idx).v;

			v2 = ST.query(idx + 1, idx + 1).v;

			double s1 = atan2(v1.y, v1.x);
			double s2 = atan2(v2.y, v2.x);

			ST.lazyUpdate(idx + 1, n, (a - RAD_to_DEG(s2 - s1)));


			Vector ans = ST.query(1, n).v;
			cout << ans.x << ' ' << ans.y << '\n';
		}

	}


	return 0;
}

/*
 * Let (x,y) be the coordinates of the house and let's consider only
 * the first restriction, which states that dist(house, Kimberly) = a * dist(house, Bob)
 * We can derive from this equation a second degree equation in x and y, it has the form
 * x^2 + y^2 + cx * x + cy * y = cc, because coefficient of x^2 and y^2 are the same,
 * we can modify this equation to be a circle equation in the form (x - h)^2 + (y - g)^2 = r^2
 * And because know for sure that there's a valid house location on the line segment between Kimberly and Bob
 * we know that it can't be the case that r^2 is < 0 and hence we can find the circle's center and radius
 * And we get the center and radius of the circle corresponding to the restriction between Jack and Janet
 * and find the intersection of the two circles.
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
double dist(Point<T> p1, Point<T> p2) {
	return hypot(p1.x - p2.x, p1.y - p2.y);
}
template<class T>
Point<T> toVec(Point<T> p1, Point<T> p2) {
	return p2 - p1;
}

template<class T>
struct Circle {
public:
	Point<T> p;
	T r;
	Circle(Point<T> _p, double _r) : p(_p), r(_r) {}

};

template<class T>
pair<pair<Point<double>, Point<double>>, bool> cirCirIntersection(Circle<T> c1, Circle<T> c2) {
	if (dist(c1.p, c2.p) < EPS) {	// if c1 and c2 share the same center
		if (fabs(c1.r - c2.r) < EPS)	// c1 = c2, return leftmost point
			return mp(mp(Point<double>(c1.p.x - c1.r, c1.p.y), Point<double>(c1.p.x - c1.r, c1.p.y)), 1);
		return mp(mp(Point<double>(), Point<double>()), 0);
	}
	if (dist(c1.p, c2.p) > c1.r + c2.r + EPS)
		return mp(mp(Point<double>(), Point<double>()), 0);
	if (fabs(dist(c1.p, c2.p) - (c1.r + c2.r)) < EPS) {
		Point<double> ret = toVec(c1.p, c2.p).scale(c1.r / (c1.r + c2.r)) + c1.p;
		return mp(mp(ret, ret), 1);
	}
	double d, a, h;
	d = dist(c1.p, c2.p);
	a = (c1.r * c1.r - c2.r * c2.r + d * d) / (2 * d);
	h = sqrt(c1.r * c1.r - a * a);
	Point<double> p2 = (c2.p - c1.p).scale(a / d) + c1.p;
	double x3, y3, x4, y4;
	x3 = p2.x + h * (c2.p.y - c1.p.y) / d;
	y3 = p2.y - h * (c2.p.x - c1.p.x) / d;
	x4 = p2.x - h * (c2.p.y - c1.p.y) / d;
	y4 = p2.y + h * (c2.p.x - c1.p.x) / d;
	return mp(mp(Point<double>(x3, y3), Point<double>(x4, y4)), 1);
}


int main() {
	#ifdef LOCAL
	auto stTime = clock();
//	freopen("in.txt", "r", stdin);
	mt19937_64 rng(chrono::steady_clock::now().time_since_epoch().count());
	#endif
	ios::sync_with_stdio(false);
	cout.precision(2);
	cin.tie(0);

	int a, b, xk, yk, xb, yb, xjack, yjack, xjanet, yjanet;

	cin >> a >> b >> xk >> yk >> xb >> yb >> xjack >> yjack >> xjanet >> yjanet;

	double cx1, cy1, r1, cx2, cy2, r2;

	{
		double cx = (-2 * xk + 2 * a * a * xb) / (1.0 - a * a);
		double cy = (-2 * yk + 2 * a * a * yb) / (1.0 - a * a);
		double cc = (a * a * 1.0 * xb * xb + a * a * 1.0 * yb * yb - xk * xk - yk * yk) / (1.0 - a * a);
		cx1 = -cx / 2;
		cy1 = -cy / 2;
		double ccc = cc + cx * cx / 4 + cy * cy / 4;
		assert(ccc > EPS);
		r1 = sqrt(ccc);
	}

	{
		double cx = (-2 * xjack + 2 * b * b * xjanet) / (1.0 - b * b);
		double cy = (-2 * yjack + 2 * b * b * yjanet) / (1.0 - b * b);
		double cc = (b * b * 1.0 * xjanet * xjanet + b * b * 1.0 * yjanet * yjanet - xjack * xjack - yjack * yjack) / (1.0 - b * b);
		cx2 = -cx / 2;
		cy2 = -cy / 2;
		double ccc = cc + cx * cx / 4 + cy * cy / 4;
		assert(ccc > EPS);
		r2 = sqrt(ccc);
	}

	db(cx1, cy1, r1, cx2, cy2, r2);

	auto pr = cirCirIntersection(Circle<double>(Point<double>(cx1, cy1), r1), Circle<double>(Point<double>(cx2, cy2), r2));

	if (!pr.sc)
		cout << "Impossible!\n";
	else {
		auto p = min(pr.fs.fs, pr.fs.sc);
		cout << fixed << p.x << ' ' << p.y << '\n';
	}

	#ifdef LOCAL
	cout << "\n\n\nExecution time: " <<
		(clock() - stTime) * 1e3 / CLOCKS_PER_SEC << " ms" << endl;
	#endif
    return 0;
}

/*
 * answer = area of first polygon + area of second polygon - 2 * area of their intersection
 * Because the two polygons are convex, their intersection (if any) is convex as well,
 * A vertex in the intersection polygon is either:
 * 1. A vertex in one polygon that lies insider/on the border of the other polygon
 * 2. A result of pair of edge intersection, one edges from each polygon
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


#define INF 1e9
#define EPS 1e-9
#define PI acos(-1.0)

double DEG_to_RAD(double d) {
	return d * PI / 180.0;
}

double RAD_to_DEG(double r) {
	return r * 180.0 / PI;
}

// Points and lines

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
	// rotate p by theta degrees CCW w.r.t origin (0, 0)
	Point<double> rotate(double theta) {
		double rad = DEG_to_RAD(theta);
		return Point<double>(x * cos(rad) - y * sin(rad),
				x * sin(rad) + y * cos(rad));
	}
};

template<class T>
ostream& operator<<(ostream& o, const Point<T>& pt) {
	return o <<  mp(pt.x, pt.y);
}

ll distSq(Point<int> p1, Point<int> p2) {
	return (p1.x - p2.x) * 1LL * (p1.x - p2.x)
			+ (p1.y - p2.y) * 1LL * (p1.y - p2.y);
}

template<class S, class T>
double distSq(Point<S> p1, Point<T> p2) {
	return (p1.x - p2.x)  * (p1.x - p2.x)
			+ (p1.y - p2.y) * (p1.y - p2.y);
}

template<class S, class T>
double dist(Point<S> p1, Point<T> p2) {
	return hypot(p1.x - p2.x, p1.y - p2.y);
}

template<class T>
struct Segment {
	Point<T> st, en;
	Segment(Point<T> _st, Point<T> _en) : st(_st), en(_en) {
		if (st > en)
			swap(st, en);
	}
	// checks if a point on the line st-en lies within the seg.
	bool onSeg(Point<int> p) {
		ll a = distSq(st, p), b = distSq(en, p), c = distSq(st, en);
		return a <= c && b <= c;
	}
	bool onSeg(Point<double> p) {
		double a = dist(st, p), b = dist(en, p), c = dist(st, en);
		return a < c + EPS && b < c + EPS;
	}
};

template<class T>
ostream& operator<<(ostream& o, const Segment<T>& seg){
	return o <<  mp(seg.st, seg.en);
}

struct Line {
	// a line with equation ax + by + c = 0
	double a, b, c;
	template<class T>
	Line(Point<T> p1, Point<T> p2) {
		if (fabs(p1.x - p2.x) < EPS) {	// vertical line
			a = 1.0;
			b = 0.0;
			c = -p1.x;
		} else {
			// b is fixed to 1.0
			a = -(p1.y - p2.y) * 1.0 / (p1.x - p2.x);
			b = 1.0;
			c = -(a * p1.x) - p1.y;
		}
	}
	template<class T>
	Line(Point<T> p, double m) {
		if (fabs(m - INF) < EPS) {
			a = 1.0;
			b = 0;
			c = -p.x;
		} else {
			a = -m;
			b = 1.0;
			c = -(a * p.x) - p.y;
		}
	}
	template<class T>
	Line(Segment<T> s) {
		Line(s.st, s.en);
	}
	Line() {}
	// returns a unit vector lying in right half plan
	// in the direction of the line
	Point<double> unitVec() {
		if (fabs(a) < EPS)
			return Point<double>(1, 0);
		double vy = a * sqrt(1 / (a * a + b * b));
		double vx = -b * vy / a;
		return Point<double>(vx, vy);
	}
	template<class T>
	bool onLine(Point<T> p) {
		return fabs(a * p.x + b * p.y + c) < EPS;
	}
	double slope() {
		if (fabs(b) < EPS)
			return INF;
		return -a / b;
	}
	double perpSlope() {
		if (fabs(a) < EPS)
			return INF;
		return b / a;
	}

};

// returns (dy, dx) in their irreducible form
// in dy / dx, making dx always positive
pii slope(Point<int> p1, Point<int> p2) {
	if (!(p1.x - p2.x))	// vertical (slope = INF)
		return mp(1, 0);
	if (!(p1.y - p2.y)) // horizontal (slope = 0)
		return mp(0, 1);
	int dy = p1.y - p2.y, dx = p1.x - p2.x;
	int g = abs(__gcd(dx, dy));
	dy /= g, dx /= g;
	if (dx < 0)
		dx = -dx, dy = -dy;
	return mp(dy, dx);
}

pii perpSlope(pii slope) {
	pii ret = mp(slope.sc, slope.fs);
	if (!slope.fs || !slope.sc)
		return ret;
	ret.fs = -ret.fs;
	if (ret.sc < 0)
		ret.fs = -ret.fs, ret.sc = -ret.sc;
	return ret;
}

bool areParallel(Line l1, Line l2) {
	return (fabs(l1.a - l2.a) < EPS)
			&& (fabs(l1.b - l2.b) < EPS);
}

bool areSame(Line l1, Line l2) {
	return areParallel(l1, l2)
			&& (fabs(l1.c - l2.c) < EPS);
}

pair<Point<double>, bool> lineIntersect(Line l1, Line l2) {
	Point<double> ret;
	if (areParallel(l1, l2))
		mp(ret, 0);	// no intersection
	ret.x = (l2.b * l1.c - l1.b * l2.c)
			/ (l2.a * l1.b - l1.a * l2.b);
	if (fabs(l1.b) > EPS)
		ret.y = -(l1.a * ret.x + l1.c);
	else
		ret.y = -(l2.a * ret.x + l2.c);
	return mp(ret, 1);
}


template<class T, class S>
pair<Point<double>, bool> segIntersect(Segment<T> s1, Segment<S> s2) {
	pair<Point<double>, bool> ret = lineIntersect(Line(s1.st, s1.en), Line(s2.st, s2.en));
	if (!(ret.sc && s1.onSeg(ret.fs) && s2.onSeg(ret.fs)))
		ret.sc = false;
	return ret;


}

template<class T>
Point<double> closestPoint(Line l, Point<T> p) {
	if (fabs(l.b) < EPS)	// vertical
		return Point<double>(-l.c, p.y);
	if (fabs(l.a) < EPS)	// horizontal
		return Point<double>(p.x, -l.c);
	Line perp = Line(p, 1 / l.a);
	return lineIntersect(l, perp).fs;
}

template<class T>
Point<T> toVec(Point<T> p1, Point<T> p2) {
	return p2 - p1;
}

template<class T>
Point<double> reflectionPoint(Line l, Point<T> p) {
	Point<double> pD(p.x, p.y);
	Point<double> b = closestPoint(l, pD);
	Point<double> v = toVec(pD, b);
	return b + v.scale(2);
}

ll dot(Point<int> a, Point<int> b) {
	return a.x * 1LL * b.x + a.y * 1LL * b.y;
}

double dot(Point<double> a, Point<double> b) {
	return a.x * b.x + a.y * b.y;
}

ll cross(Point<int> a, Point<int> b) {
	return a.x * 1LL * b.y - a.y * 1LL * b.x;
}

double cross(Point<double> a, Point<double> b) {
	return a.x * b.y - a.y * b.x;
}

ll normSq(Point<int> v) {
	return v.x * v.x + v.y * v.y;
}

double normSq(Point<double> v) {
	return v.x * v.x + v.y * v.y;
}

// return unsigned angle aob (in range [0, pi]) in rad
template<class T>
double angle(Point<T> a, Point<T> o, Point<T> b) {
	Point<T> oa = toVec(o, a), ob = toVec(o, b);
	return acos(dot(oa, ob) / sqrt(normSq(oa) * 1.0 * normSq(ob)));
}
// return signed angle aob (in range [0, 2 * pi]) in rad
template<class T>
double angleSigned(Point<T> a, Point<T> o, Point<T> b) {
	a = a - o, b = b - o;
	double rot = -atan2(a.y, a.x);
	b.rotate(RAD_to_DEG(rot));
	double ret = atan2(b.y, b.x);
	if (ret < 0)
		ret += 2 * PI;
	return ret;
}
// returns +ve if s is to the left of pq, -ve if right, 0 if colinear
// also returns 0 if pqs forms a degenerate triangle (with area = 0)
ll toLeft(Point<int> p, Point<int> q, Point<int> s) {
	return cross(toVec(p, q), toVec(p, s));
}

double toLeft(Point<double> p, Point<double> q, Point<double> s) {
	return cross(toVec(p, q), toVec(p, s));
}

// Circles

template<class T>
struct Circle {
public:
	Point<T> p;
	T r;
	Circle(Point<T> _p, double _r) : p(_p), r(_r) {}

};

// returns the two points where two circles intersect (if any)
template<class T>
pair<pair<Point<double>, Point<double>>, bool> cirCirIntersection(Circle<T> c1, Circle<T> c2) {
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

// 0 -> in, 1 -> on, 2 -> out
template<class T>
int insideCircle(Point<T> p, Circle<T> c) {
	double d = sqDist(p, c.p), rSq = c.r * c.r;
	return d > rSq + EPS ? 0 : (fabs(d - rSq) < EPS ? 1 : 2);
}

// Triangles
double perimeter(double p12, double p23, double p31) {
	return p12 + p23 + p31;
}

double area(double p12, double p23, double p31) {
	double s = 0.5 * perimeter(p12, p23, p31);
	return sqrt(s) * sqrt(s - p12) * sqrt(s - p23) * sqrt(s - p31);
}

double rInCircle(double p12, double p23, double p31) {
	return area(p12, p23, p31) / (0.5 * perimeter(p12, p23, p31));
}

double rCircumCircle(double p12, double p23, double p31) {
	return p12 * p23 * p31 / (4.0 * area(p12, p23, p31));
}

template<class T>
struct Triangle {
	Point<T> p1, p2, p3;
	double p12, p23, p31;
	Triangle (Point<T> _p1, Point<T> _p2, Point<T> _p3)
		: p1(_p1), p2(_p2), p3(_p3) {
		p12 = dist(p1, p2);
		p23 = dist(p2, p3);
		p31 = dist(p3, p1);
	}
	double perimeter() {
		return p12 + p23 + p31;
	}
	double area() {
		return ::area(p12, p23, p31);
	}
	double rInCircle() {
		return ::rInCircle(p12, p23, p31);
	}
	double rCircumCircle() {
		return ::rCircumCircle(p12, p23, p31);
	}
};

template<class T>
Circle<double> inCircle(Triangle<T> t) {
	double r = t.rInCircle();
	Line l1, l2;
	double ratio = dist(t.p1, t.p2) / dist(t.a, t.c);
	Point<double> p = t.b + toVec(t.p2, t.p3).scale(ratio / (1 + ratio));
	l1 = Line(t.p1, p);
	ratio = dist(t.p2, t.p1) / dist(t.p2, t.p3);
	p = t.p1 + toVec(t.p1, t.p3).scale(ratio / (1 + ratio));
	l2 = Line(t.p2, p);
	return Circle<double>(lineIntersect(l1, l2).fs, r);
}

template<class T>
Circle<double> circumCircle(Triangle<T> t) {
	if (fabs(toLeft(t.p1, t.p2, t.p3)) < EPS)
		return Circle<double>(Point<double>(), -1);
	Point<double> mid1 = (t.p1 + t.p2).scale(.5);
	Point<double> mid2 = (t.p1 + t.p3).scale(.5);
	Line l1, l2;
	l1 = Line(t.p1, t.p2);
	l2 = Line(t.p1, t.p3);
	double m1, m2;
	if (fabs(l1.b) < EPS)
		m1 = 0;
	else if (fabs(l1.a) > EPS)
		m1 = 1.0 / l1.a;
	else
		m1 = INF;
	if (fabs(l2.b - 0) < EPS)
		m2 = 0;
	else if (fabs(l2.a) > EPS)
		m2 = 1.0 / l2.a;
	else
		m2 = INF;
	l1 = Line(mid1, m1);
	l2 = Line(mid2, m2);
	Point<double> ctr = lineIntersect(l1, l2).fs;
	double r = dist(t.p1, ctr);
	return Circle<double>(ctr, r);
}

bool canFormTriangle(double a, double b, double c) {
	return (a + b > c) && (a + c > b) && (b + c > a);
}

// Polygons
template<class T>
double perimeter(vector<Point<T>>& P) {
	P.pb(P[0]);
	double result = 0.0;
	for (int i = 0; i < sz(P) - 1; i++)
		result += dist(P[i], P[i + 1]);
	P.pop_back();
	return result;
}

// recall picks theorem: A = i + b / 2 - 1
// where A is polygon area, i is number of points inside
// b is number of points on boundary
template<class T>
double area(vector<Point<T>>& P) {
	double result = 0.0, x1, y1, x2, y2;
	P.pb(P[0]);
	for (int i = 0; i < sz(P) - 1; i++)
		result += cross(P[i], P[i + 1]);
//		b += __gcd(abs(P[i + 1].x - P[i].x),
//				abs(abs(P[i + 1].y - P[i].y)));
	P.pop_back();
	// result is an integer
	return fabs(result) / 2.0;
}

template<class T>
bool isConvex(vector<Point<T>>& P) {
	P.pb(P.back());
	int size = sz(P);
	if (size <= 3) {
		P.pop_back();
		return false;
	}
	bool isLeft = toLeft(P[0], P[1], P[2]) > 0;
	bool ret = true;
	for (int i = 1; i < size - 1; i++)
		if ((toLeft(P[i], P[i+1],P[(i + 2) == size ? 1 : i + 2]) > 0)
				!= isLeft) {
			ret = false;
			break;
		}
	P.pop_back();
	return ret;
}

template<class T>
bool inPolygon(Point<T> pt, vector<Point<T>>& P) {
	P.pb(P[0]);
	double sum = 0;
	for (int i = 0; i < sz(P) - 1; i++) {
		if (toLeft(pt, P[i], P[i + 1]) > 0)
			sum += angle(P[i], pt, P[i + 1]);
		else
			sum -= angle(P[i], pt, P[i + 1]);
	}
	P.pop_back();
	return fabs(fabs(sum) - 2 * PI) < EPS;
}

// cuts polygon and return the part to the LEFT of A->B
// To get the other side you can swap A,B
template<class T>
vector<Point<double>> cutPolygon(Point<T> a, Point<T> b, vector<Point<T>>& Q) {
	vector<Point<double>> P;
	Q.pb(Q[0]);
	for (int i = 0; i < sz(Q) - 1; i++) {
		double left1 = cross(toVec(a, b), toVec(a, Q[i])),
				left2 = 0;
		if (left1 > -EPS)
			P.pb(Q[i]);
		if (left1 * left2 < -EPS)
			P.pb(lineIntersect(Line(Q[i], Q[i + 1]), Line(a, b)).fs);
	}
	Q.pop_back();
	return P;
}

// returns list of indices (w.r.t P) of CH in CW dir.
// assuming integer coordinates, no duplicate points
// handles co-linear points and if they exist on CH, they're included

template<class T>
vi CH(vector<Point<T>> P) {
	int n = sz(P);
	vi ret(n);

	if (n <= 2) {
		ret[0] = 0;
		ret.back() = n - 1;
		return ret;
	}

	vector<pair<Point<T>, int>> Q(n);

	for (int i = 0; i < n; ++i)
		Q[i] = mp(P[i], i);

	sort(Q.begin(), Q.end());

	vi U;

	U.pb(Q[0].sc);
	U.pb(Q[1].sc);

	for (int i = 2; i < n; ++i) {	// if dealing with integer coordinates, make the EPS 0
		while (sz(U) >= 2 && toLeft(P[U[U.size() - 2]], P[U[U.size() - 1]], Q[i].fs) > EPS)
			U.pop_back();
		U.pb(Q[i].sc);
	}

	vi L;
	L.pb(Q[n - 1].sc);
	L.pb(Q[n - 2].sc);

	for (int i = n - 3; i >= 0; --i) {
		while (sz(L) >= 2 && toLeft(P[L[L.size() - 2]], P[L[L.size() - 1]], Q[i].fs) > EPS)
			L.pop_back();
		L.pb(Q[i].sc);
	}

	if (sz(L) == n)	// case where all points are co-linear
		return L;

	for (int i = 1; i < sz(U) - 1; ++i)
		L.pb(U[i]);

	return L;
}


// Closest pair

template<class T>
struct cmpY {
	bool operator()(const Point<T>& p1, const Point<T>& p2) {
		return Point<T>(p1.y, p1.x) < Point<T>(p2.y, p2.x);
	}
};

template<class T>
double closestPair(vector<Point<T>>& P) {
	double d = INF;
	multiset<Point<T>, cmpY<T>> actWin;
	sort(P.begin(), P.end());
	int L = 0;
	for (int R = 0; R < sz(P); ++R) {
		while (L < R && P[R].x - P[L].x > d)
			actWin.erase(actWin.find(P[L++]));
		auto itSt = actWin.lower_bound(Point<T>(-INF, P[R].y - d));
		auto itEn = actWin.upper_bound(Point<T>(-INF, P[R].y + d));
		for (; itSt != itEn; itSt++)
			d = min(d, dist(P[R], *itSt));
		actWin.insert(P[R]);
	}
	return d;
}

// Minimum enclosing circle

vector<Point<double>> pts;
Point<double> r[3], cen;
double rad;
int ps, rs;	// ps = n, rs = 0, initially

// Pre condition
// random_shuffle(pts.begin(), pts.end());
void MEC() {
	if (ps == 0 && rs == 2) {
		cen = Point<double>((r[0].x + r[1].x) / 2, (r[0].y + r[1].y) / 2);
		rad = dist(r[0], cen);
	} else if (rs == 3) {
		Circle<double> c = circumCircle(Triangle<double>(r[0], r[1], r[2]));
		cen = c.p, rad = c.r;
	} else if (ps == 0) {
		cen = r[0];
		rad = 0;
	} else {
		ps--;
		MEC();
		if (dist(pts[ps], cen) > rad) {
			r[rs++] = pts[ps];
			MEC();
			rs--;
		}
		ps++;
	}
}

// returns a set of points sorted by increasing angle around P[idx]
template<class T>
vector<Point<T>> radialSweep(vector<Point<T>> P, int idx) {
	vector<Point<T>> ret = P;
	Point<T> pivot = P[idx], pivotR = Point<T>(pivot.x + 1, pivot.y);
	// ret[0] = P[idx]
	sort(ret.begin(), ret.end(), [pivot, pivotR](Point<T>& p1, Point<T>& p2) {
		if (p1 == pivot)
			return true;
		if (p2 == pivot)
			return false;
		double ang1 = angleSigned(pivotR, pivot, p1);
		double ang2 = angleSigned(pivotR, pivot, p2);
		// since we're dealing with values in range [0, 2 * PI],
		// we use very small EPS
		if (fabs(ang1 - ang2) < 1e-18)
			return dist(pivot, p1) < dist(pivot, p2);
		return ang1 < ang2;
	});
	return ret;
}

int main() {
	#ifdef LOCAL
	auto stTime = clock();
	freopen("in.txt", "r", stdin);
	#endif
	ios::sync_with_stdio(false);
	cout.precision(2);
	cin.tie(0);

	cout << fixed;

	int n, m;

	while (cin >> n && n) {
		V<Point<int>> A(n);

		for (int i = 0; i < n; ++i)
			cin >> A[i].x >> A[i].y;

		cin >> m;

		V<Point<int>> B(m);

		for (int i = 0; i < m; ++i)
			cin >> B[i].x >> B[i].y;

		A.pb(A[0]);
		B.pb(B[0]);

		V<Segment<int>> s1, s2;

		set<Point<double>> C;

		for (int i = 0; i < n; ++i)
			s1.pb(Segment<int>(A[i], A[i + 1]));

		for (int i = 0; i < m; ++i)
			s2.pb(Segment<int>(B[i], B[i + 1]));

		for (int i = 0; i < n; ++i)
			for (int j = 0; j < m; ++j) {
				auto pr = segIntersect(s1[i], s2[j]);
				if (pr.sc)
					C.insert(pr.fs);
			}

		for (int i = 0; i < n; ++i)
			if (inPolygon(A[i], B))
				C.insert(Point<double>(A[i].x, A[i].y));

		for (int i = 0; i < m; ++i)
			if (inPolygon(B[i], A))
				C.insert(Point<double>(B[i].x, B[i].y));

		if (C.empty()) {
			cout << setw(8) << area(A) + area(B);
			continue;
		}

		V<Point<double>> Cv;

		for (auto& pt : C)
			Cv.pb(pt);

		vi C_ch = CH(Cv);

		V<Point<double>> CC;

		for (auto& idx : C_ch)
			CC.pb(Cv[idx]);

		A.pop_back();
		B.pop_back();

		cout << setw(8) << area(A) + area(B) - 2 * area(CC);

	}

	ndl;


	#ifdef LOCAL
	cout << "\n\n\nExecution time: " <<
		(clock() - stTime) * 1e3 / CLOCKS_PER_SEC << " ms\n";
	#endif
    return 0;
}

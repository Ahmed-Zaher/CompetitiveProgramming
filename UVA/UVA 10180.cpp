/*
 * Let's fix p1, and consider the line (let it be L) that goes through p1
 * that is also perpendicular to the line going from origin to p1
 * If p2 lies on a side of L that is different from the origin,
 * then we know that we can connect p1 with p2 with a straight line.
 * If not, then L either doesn't intersect the circle at all,
 * in which case we can use a straight line. Or L does intersect
 * the circle and in this case the segment p1p2 must intersect the circle too.
 * In that case we go from p1 to one of the tangent points it makes with the
 * circle via a straight line, then move over the circle circumference
 * until we reach a tangent point that p2 makes with the circle, then
 * go to p2 via a straight line.
 */

#include <bits/stdc++.h>

using namespace std;

const int OO = 1e7;
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


const double PI = acos(-1.0);


struct Point {
	double x, y;
	Point(double _x = 0, double _y = 0) : x(_x), y(_y) {}
};

double dist(Point p, Point q) {
	return hypot(p.x - q.x, p.y - q.y);
}
double cross(Point a, Point b) {
	return a.x * b.y - a.y * b.x;
}
Point toVec(Point p1, Point p2) {
	return Point(p2.x - p1.x, p2.y - p1.y);
}
ll toLeft(Point p, Point q, Point s) {
	return cross(toVec(p, q), toVec(p, s));
}

Point p1, p2;
double R;


V<double> getTangents(Point p) {
	double alpha = atan2(p.y, p.x);
	double theta = acos(R / dist(p, Point(0, 0)));
	return V<double>{alpha - theta, alpha + theta};
}


int main() {
	#ifdef LOCAL
	auto stTime = clock();
	freopen("in.txt", "r", stdin);
	mt19937_64 rng(chrono::steady_clock::now().time_since_epoch().count());
	#endif
	ios::sync_with_stdio(false);
	cout.precision(3);
	cin.tie(0);

	db(PI);

	cout << fixed;

	Point orig = Point(0, 0);

	int T;
	cin >> T;


	for (int tc = 1; tc <= T; ++tc) {

		cin >> p1.x >> p1.y >> p2.x >> p2.y >> R;

		double dx = p2.x - p1.x, dy = p2.y - p1.y;

		// if p1 = p2
		{

			if (fabs(dx) < EPS && fabs(dy) < EPS) {
				cout << "0.000\n";
				continue;
			}
		}

		{

		}

		// check if p2 is one different side then origin w.r.t. L
		// using toLeft tests

		bool good = false;

		for (int i = 0; i < 2; ++i) {
			ll sgn1 = toLeft(p1, Point(p1.x + p1.y, p1.y - p1.x), p2);
			ll sgn2 = toLeft(p1, Point(p1.x + p1.y, p1.y - p1.x), orig);
			if (sgn1 > -EPS && sgn2 < 0)
				good = true;
			if (sgn1 <= EPS && sgn2 > 0)
				good = true;
			swap(p1, p2);
		}

		if (good) {
			cout << dist(p1, p2) << '\n';
			continue;
		}

		// check if L intersect the circle, by solving the line
		// equation with the circle equation

		if (fabs(dx) < EPS)
			swap(p1.x, p1.y), swap(p2.x, p2.y), swap(dx, dy);

		double m = dy / dx;
		double c = -p1.x * m + p1.y;

		double d = 4 * m * m * c * c
				- 4 * (1 + m * m) * (-R * R + c * c);

		if (d < EPS)
			cout << dist(p1, p2) << '\n';
		else {
			double ans = 1e9;
			V<double> tangents1 = getTangents(p1);
			V<double> tangents2 = getTangents(p2);
			for (auto& alpha : tangents1) {
				for (auto& beta : tangents2) {
					double gamma = fabs(alpha - beta);
					while (gamma > 2 * PI)
						gamma -= 2 * PI;
					gamma = min(gamma, 2 * PI - gamma);
					Point p11(R * cos(alpha), R * sin(alpha));
					Point p22(R * cos(beta), R * sin(beta));
					ans = min(ans, dist(p1, p11) + dist(p2, p22) + R * gamma);
				}
			}

			cout << ans << '\n';

		}

	}




	#ifdef LOCAL
	cout << "\n\n\nExecution time: " <<
		(clock() - stTime) * 1e3 / CLOCKS_PER_SEC << " ms" << endl;
	#endif
	return 0;
}

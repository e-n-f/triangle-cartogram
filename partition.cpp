#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <vector>
#include <map>
#include <set>

double getarea(std::vector<size_t> const &t, std::vector<double> const &x, std::vector<double> const &y) {
	size_t n = t.size();
	double area = 0;
	for (size_t i = 0; i < n; i++) {
		area += x[t[i]] * y[t[(i + 1 + n) % n]];
		area -= y[t[i]] * x[t[(i + 1 + n) % n]];
	}
	area /= 2;
	return area;
}

struct point {
	double x;
	double y;
	ssize_t id;
	std::set<size_t> neighbors;
	std::vector<size_t> triangles;
	double nx;
	double ny;
};

struct hcmp {
        bool operator()(const point &a, const point &b) {
                return a.x < b.x;
        }
} hcmp;

struct vcmp {
        bool operator()(const point &a, const point &b) {
                return a.y < b.y;
        }
} vcmp;

void split(std::vector<point> points, double x1, double y1, double x2, double y2, int direction, std::vector<point> &original) {
	if (points.size() < 1) {
		return;
	}

	if (points.size() == 1) {
		original[points[0].id].nx = (x1 + x2) / 2;
		original[points[0].id].ny = (y1 + y2) / 2;

		printf("%.6f %.6f 1 0 360 arc fill\n", (x1 + x2) / 2, (y1 + y2) / 2);
		return;
	}

	if (direction == 0) {
		std::sort(points.begin(), points.end(), hcmp);
	} else {
		std::sort(points.begin(), points.end(), vcmp);
	}

	std::vector<point> p1, p2;
	for (size_t i = 0; i < points.size() / 2; i++) {
		p1.push_back(points[i]);
	}
	for (size_t i = points.size() / 2; i < points.size(); i++) {
		p2.push_back(points[i]);
	}

	if (direction == 0) {
		split(p1, x1, y1, (x1 + x2) / 2, y2, !direction, original);
		split(p2, (x1 + x2) / 2, y1, x2, y2, !direction, original);
	} else {
		split(p1, x1, y1, x2, (y1 + y2) / 2, !direction, original);
		split(p2, x1, (y1 + y2) / 2, x2, y2, !direction, original);
	}
}

int main() {
	char s[2000];
	std::vector<point> points;
	std::vector<std::vector<size_t>> tris;

	while (fgets(s, 2000, stdin)) {
		int l1, l2, l3;
		double p1x, p1y, p2x, p2y, p3x, p3y;

		if (sscanf(s, "%d %lf,%lf %d %lf,%lf %d %lf,%lf",
			   &l1, &p1y, &p1x,
			   &l2, &p2y, &p2x,
			   &l3, &p3y, &p3x) != 9) {
			fprintf(stderr, "??? %s\n", s);
			continue;
		}

		size_t max = l1;
		if (l2 > max) {
			max = l2;
		}
		if (l3 > max) {
			max = l3;
		}
		if (points.size() <= max) {
			points.resize(max + 1);
		}

		points[l1].x = p1x;
		points[l1].y = p1y;

		points[l2].x = p2x;
		points[l2].y = p2y;

		points[l3].x = p3x;
		points[l3].y = p3y;

		points[l1].neighbors.insert(l2);
		points[l1].neighbors.insert(l3);

		points[l2].neighbors.insert(l1);
		points[l2].neighbors.insert(l3);

		points[l3].neighbors.insert(l1);
		points[l3].neighbors.insert(l2);

		std::vector<size_t> t;
		t.push_back(l1);
		t.push_back(l2);
		t.push_back(l3);
		tris.push_back(t);

		points[l1].triangles.push_back(tris.size() - 1);
		points[l2].triangles.push_back(tris.size() - 1);
		points[l3].triangles.push_back(tris.size() - 1);
	}

	std::vector<point> points2;

	for (size_t i = 0; i < points.size(); i++) {
		if (points[i].neighbors.size() != 0) {
			points[i].id = i;
			points2.push_back(points[i]);
		}
	}

	split(points2, 0, 0, 612, 612, 0, points);

	printf("0 setlinewidth\n");
	for (size_t i = 0; i < points.size(); i++) {
		for (auto n = points[i].neighbors.begin(); n != points[i].neighbors.end(); ++n) {
			printf("%.6f %.6f moveto %.6f %.6f lineto stroke\n",
				points[i].nx, points[i].ny,
				points[*n].nx, points[*n].ny);
		}
	}
}

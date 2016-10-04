#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <map>
#include <set>

double area(std::vector<size_t> const &t, std::vector<double> const &x, std::vector<double> const &y) {
	size_t n = t.size();
	double area = 0;
	for (size_t i = 0; i < n; i++) {
		area += x[t[i]] * y[t[(i + 1 + n) % n]];
		area -= y[t[i]] * x[t[(i + 1 + n) % n]];
	}
	area /= 2;
	return area;
}

int main() {
	char s[2000];
	std::vector<double> x;
	std::vector<double> y;
	std::vector<std::vector<size_t>> tris;
	std::vector<std::vector<size_t>> triangles_of;
	std::vector<std::set<size_t>> neighbor;

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
		if (x.size() <= max) {
			x.resize(max + 1);
			y.resize(max + 1);
			neighbor.resize(max + 1);
			triangles_of.resize(max + 1);
		}

		x[l1] = p1x;
		y[l1] = p1y;
		x[l2] = p2x;
		y[l2] = p2y;
		x[l3] = p3x;
		y[l3] = p3y;

		neighbor[l1].insert(l2);
		neighbor[l1].insert(l3);
		neighbor[l2].insert(l1);
		neighbor[l2].insert(l3);
		neighbor[l3].insert(l1);
		neighbor[l3].insert(l2);

		std::vector<size_t> t;
		t.push_back(l1);
		t.push_back(l2);
		t.push_back(l3);
		tris.push_back(t);

		triangles_of[l1].push_back(tris.size() - 1);
		triangles_of[l2].push_back(tris.size() - 1);
		triangles_of[l3].push_back(tris.size() - 1);
	}
}

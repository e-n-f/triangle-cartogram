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

	for (size_t outer = 0; 1 || outer < 500; outer++) {
		if (1) {
			double minarea = 999999;
			double maxarea = 0;

			for (size_t i = 0; i < tris.size(); i++) {
				double area = getarea(tris[i], x, y);
				if (area > maxarea) {
					maxarea = area;
				}
				if (area < minarea) {
					minarea = area;
				}
			}

			fprintf(stderr, "%lu: %.6f\n", outer, maxarea / minarea);
		}

		if (outer % 5 == 0) {
			double xmax, xmin, ymax, ymin;
			xmax = ymax = -999999;
			xmin = ymin = 999999;

			for (size_t i = 0; i < x.size(); i++) {
				if (x[i] == 0) {
					continue;
				}

				if (x[i] > xmax) {
					xmax = x[i];
				}
				if (y[i] > ymax) {
					ymax = y[i];
				}
				if (x[i] < xmin) {
					xmin = x[i];
				}
				if (y[i] < ymin) {
					ymin = y[i];
				}
			}

			double scale;
			if (xmax - xmin > ymax - ymin) {
				scale = xmax - xmin;
			} else {
				scale = ymax - ymin;
			}

			printf("0 setlinewidth\n");
			for (size_t i = 0; i < x.size(); i++) {
				if (x[i] == 0) {
					continue;
				}

				for (auto k = neighbor[i].begin(); k != neighbor[i].end(); ++k) {
					printf("%.6f %.6f moveto %.6f %.6f lineto stroke\n",
					       (x[i] - xmin) * 612 / scale,
					       (y[i] - ymin) * 612 / scale,
					       (x[*k] - xmin) * 612 / scale,
					       (y[*k] - ymin) * 612 / scale);
				}
			}
			printf("showpage\n");
		}

		for (size_t i = 0; i < x.size(); i++) {
			if (x[i] == 0) {
				continue;
			}
			if (neighbor[i].size() == 0) {
				continue;
			}

			double xsum = 0;
			double ysum = 0;
			double count = 0;

			for (auto k = neighbor[i].begin(); k != neighbor[i].end(); ++k) {
				double dmin = 9999999;
				for (auto k2 = neighbor[*k].begin(); k2 != neighbor[*k].end(); ++k2) {
					double xd = x[*k] - x[*k2];
					double yd = y[*k] - y[*k2];
					double d = sqrt(xd * xd + yd * yd);
					if (d < dmin) {
						dmin = d;
					}
				}

				xsum += x[*k] * dmin;
				ysum += y[*k] * dmin;
				count += dmin;
			}

			double wasx = x[i];
			double wasy = y[i];

			x[i] = xsum / count;
			y[i] = ysum / count;

			bool again = true;
			while (again) {
				again = false;

				for (auto t: triangles_of[i]) {
					double area = getarea(tris[t], x, y);
					if (area <= 0) {
						x[i] = (wasx + x[i]) / 2;
						y[i] = (wasy + y[i]) / 2;
						again = 1;
						break;
					}
				}
			}
		}
	}
}

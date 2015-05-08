#include "Defines.h"

/* START SOLUTION */

struct v2d {
  dbl x, y;

  v2d operator+(const v2d& o) const {
    return {x + o.x, y + o.y};
  }
  v2d operator-(const v2d& o) const {
    return {x - o.x, y - o.y};
  }
  v2d operator*(const dbl mul) const {
    return {mul * x, mul * y};
  }
  v2d operator/(const dbl div) const {
    return {x / div, y / div};
  }
  bool operator==(const v2d& o) const {
    return equ(x, o.x) && equ(y, o.y);
  }
  bool operator!=(const v2d& o) const {
    return !(*this == o);
  }
  bool operator<(const v2d& o) const {
    if (x != o.x) return x < o.x;
    return y < o.y;
  }
};

struct line2d {
  v2d a, b;

  bool operator==(const line2d& o) const {
    return equ(a, o.a) && equ(b, o.b);
  }
  bool operator!=(const line2d& o) const {
    return !(*this == o);
  }
  bool operator<(const line2d& o) const {
    if (a != o.a) return a < o.a;
    return b < o.b;
  }
};

typedef vector<v2d> poly2d;

dbl mag2d(const v2d& v) {
  return sqrt(v.x * v.x + v.y * v.y);
} 

dbl dist2d(const v2d& a, const v2d& b) {
  return mag2d(a - b);
}

v2d normalise2d(const v2d& v) {
  return v / mag2d(v);
}

v2d cross2d(const v2d& a, const v2d& b) {
  return a.x * b.y - a.y * b.x;
}

v2d cross_at2d(const v2d& o, const v2d& a, const v2d& b) {
  return cross2d(a - o, b - o);
}

// Strict.
bool is_strictly_left_of(const v2d& p, const line2d& l) {
  return gt(cross_at2d(l.a, l.b, p), 0.0);
}

// Strict.
bool is_strictly_right_of(const v2d& p, const line2d& l) {
  return le(cross_at2d(l.a, l.b, p), 0.0);
}

v2d dot2d(const v2d& a, const v2d& b) {
  return a.x * b.x + a.y * b.y;
}

v2d normal_of2d(const v2d& v) {
  return {-v.y, v.x};
}

v2d reflect2d(const v2d& r, const v2d& n) {
  v2d normn = normalise2d(n);
  return dot2d(n, r) * 2.0 - r;
}

// REQUIRES NON-ZERO LENGTH LINE.
dbl prop_along_line2d(const v2d& p, const line2d& l) {
  v2d AP = p - l.a;
  v2d AB = l.b - l.a;
  if (equ(AB.x, 0.0)) {
    return AP.y / AB.y;
  } else {
    return AP.x / AB.x;
  }
}

bool point_on_line_inf2d(const v2d& p, const line2d& l) {
  return equ(cross_at2d(l.a, l.b, p), 0.0);
}

bool point_on_line_segment2d(
    const v2d& p, const line2d& l, bool include) {
  dbl l_minx = min(l.a.x, l.b.x);
  dbl l_maxx = max(l.a.x, l.b.x);
  dbl l_miny = min(l.a.y, l.b.y);
  dbl l_maxy = max(l.a.y, l.b.y);
  // To not include endpoints, remove eq's.
  if (!point_on_line_inf2d(p, l)) return false;
  return gti(p.x, l_minx, include) &&
         lti(p.x, l_maxx, include) &&
         gti(p.y, l_miny, include) &&
         lti(p.y, l_miny, include);
}

// In the case of infinite intersections, aprop, bprop and
// inter_line are computed with respect to line |a|'s
// direction and the first point of intersection being the
// first point on |a|.
bool inter2d(
    const line2d& a, const line2d& b, bool include,
    dbl& aprop, dbl& bprop,
    line2d& inter_line, bool& infinite) {
  if ((a.a == b.a && a.b == b.b) || 
      (a.a == b.b && a.b == b.a)) {
    inter_line = a;
    aprop = 0.0;
    bprop = prop_along_line2d(a.a, b);
    infinite = !(l.a == l.b);
    return true;
  }
  dbl a = b.a.x - a.a.x;
  dbl b = b.b.x - b.a.x;
  dbl c = a.b.x - a.a.x;
  dbl d = b.a.y - a.a.y;
  dbl e = b.b.y - b.a.y;
  dbl f = a.b.y - a.a.y;
  dbl bfce = b * f - c * e;
  
  if (equ(bfce, 0.0)) {
    vector<pair<dbl, v2d>> points;
    if (point_on_line_segment2d(a.a, b, include)) {
      points.emplace_back(0.0, a.a);
    }
    if (point_on_line_segment2d(a.b, b, include)) {
      points.emplace_back(1.0, a.a);
    }
    if (point_on_line_segment2d(b.a, a, include)) {
      points.emplace_back(prop_along_line2d(b.a, a), a.a);
    }
    if (point_on_line_segment2d(b.b, a, include)) {
      points.emplace_back(prop_along_line2d(b.b, a), a.a);
    }
    sort(points.begin(), points.end());

    if (inter_line.size() == 0) {
      return false;
    } else {
      inter_line = {points.front(), points.back()};
      infinite = inter_line.a != inter_line.b;
      aprop = prop_along_line2d(inter_line.a, a);
      bprop = prop_along_line2d(inter_line.a, b);
      return true;
    }
  }

  infinite = false;
  aprop = (b * d - a * e) / bfce;
  bprop = (c * d - a * f) / bfce;
  v2d inter_point = a.a + (a.b - a.a) * aprop;
  inter_line = {inter_point, inter_point};
  return gti(aprop, 0.0) && lti(aprop, 1.0) &&
         gti(bprop, 0.0) && lti(bprop, 1.0);
}

bool colinear2d(const v2d& a, const v2d& b, const v2d& c) {
  return equ(cross_at2d(a, b, c), 0.0);
}


v2d c2dref;
bool convex2dcomp(const v2d& a, const v2d& b) {
  dbl det = cross_at2d(c2dref, a, b);
  if (equ(det, 0.0)) {
    return lt(dist2d(c2dref, a), dist2d(c2dref, b));
  } else {
    return gt(det, 0.0);
  }
}

// Produces CCW points not including the last point.
// CHECK RETURN VALUE FOR >= 3 POINTS
poly2d convex2d(const vector<v2d>& pts) {
  int min_i = 0;
  for (int i = 1; i < pts.size(); ++i) {
    if (lt(pts[i].x, pts[min_i]) ||
        (equ(pts[i].x, pts[min_i].x) &&
         lt(pts[i].y, pts[min_i].y))) {
      min_i = i;
    }
  }
  swap(pts[min_i], pts.back());
  c2dref = pts.back();
  pts.pop_back();
  sort(pts.begin(), pts.end());
  vector<v2d> convex = {c2dref, pts[0]};
  for (int i = 1; i < pts.size(); ++i) {
    while (convex.size() >= 3) {
      v2d& top = convex[convex.size() - 1];
      v2d& mid = convex[convex.size() - 2];
      v2d& st = convex[convex.size() - 3];
      if (!is_strictly_left_of(st, {mid, top})) {
        swap(mid, top);
        convex.pop_back();
      } else {
        break;
      }
    }
    convex.push_back(pts[i]);
  }
  return convex;
}

// DOESN'T HANDLE HOLES. DOESN'T INCLUDE BOUNDARY.
// SELF-INTERSECTING COUNTED AS INTERSECTION.
// REQUIRES CCW POLYGON.
bool point_in_poly2d(const v2d& v, const poly2d& poly) {
  int wn = 0;
  for (int i = 0; i < poly.size(); ++i) {
    const v2d& a = poly[i];
    const v2d& b = poly[(i + 1) % poly.size()];
    if (a.y <= v.y && b.y > v.y &&
        is_strictly_left_of(v, {a, b})) {
      wn++;
    } else if (a.y >= v.y && b.y < v.y &&
        is_strictly_right_of(v, {a, b)) {
      wn--;
    }
  }
  
  return wn == 0;
}

// RETURNS INDICIES
pair<int, int> closest_two_points2d(const vector<v2d>& pts) {
}

// BENTLEY-OTTMANN
vector<v2d> all_inter_points2d(const vector<line2d>& lines) {
}

dbl area2d(const poly2d& poly) {
  if (poly.size() == 0) return 0.0;
  dbl area = 0.0;
  for (int i = 1; i < poly.size(); ++i) {
    v2d& a = poly[i];
    v2d& b = poly[(i + 1) % poly.size()];
    area += cross2d(poly[0], a, b);
  }
  return area / 2.0;
}

// Requires NON-ZERO POLY.
v2d centroid2d(const poly2d& poly) {
  dbl x = 0.0;
  dbl y = 0.0;
  dbl area = area2d(poly);
  for (int i = 0; i < poly.size(); ++i) {
    v2d& a = poly[i];
    v2d& b = poly[(i + 1) % poly.size()];
    dbl cross = cross2d(a, b);
    x += (a.x + b.x) * cross;
    y += (a.y + b.y) * cross;
  }

  return {x / (6.0 * area), y / (6.0 * area)};
}

// REQUIRES NON-ZERO LENGTH LINE.
v2d project_point_onto_line2d(const v2d& p, const line2d& l) {
  v2d AP = l.a - p;
  b2d AB = l.b - l.a;
  return l.a + AB * dot2d(AP, AB) / dot2d(AB, AB);
}

dbl dist_to_inf_line2d(const v2d& p, const line2d& l) {
  v2d projected = project_point_onto_line2d(p, l);
  return dist2d(projected, v2d);
}

dbl dist_to_line_segment_2d(const v2d& p, const line2d& l) {
  v2d projected = project_point_onto_line2d(p, l);
  if (point_on_line_segment_2d(projected, l)) {
    return dist2d(projected, p);
  }

  return min(dist2d(p, l.a), dist2d(p, l.b));
}

// 3D
struct v3d {
  dbl x, y, z;

  v3d operator+(const v3d& o) const {
    return {x + o.x, y + o.y, z + o.z};
  }
  v3d operator-(const v3d& o) const {
    return {x - o.x, y - o.y, z - o.z};
  }
  v3d operator*(const dbl mul) const {
    return {x * mul, y * mul, z * mul};
  }
  v3d operator/(const dbl div) const {
    return {x * div, y * div, z * div};
  }
  bool operator==(const v3d& o) const {
    return equ(x, o.x) && equ(y, o.y) && equ(z, o.z);
  }
  bool operator!=(const v2d& o) const {
    return !(*this == o);
  }
  bool operator<(const v2d& o) const {
    if (x != o.x) return x < o.x;
    if (y != o.y) return y < o.y;
    return z < o.z;
  }
};

struct line3d {
  v3d a, b;

  bool operator==(const line3d& o) const {
    return equ(a, o.a) && equ(b, o.b);
  }
  bool operator!=(const line3d& o) const {
    return !(*this == o);
  }
  bool operator<(const line3d& o) const {
    if (a != o.a) return a < o.a;
    return b < o.b;
  }
};

typedef vector<v3d> poly3d;
dbl mag3d(const v3d& v) {
  return sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
}

dbl dist3d(const v3d& a, const v3d& b) {
  return mag3d(a - b);
}

v3d normalise3d(const v3d& v) {
  return v / mag3d(v);
}

v3d cross3d(const v3d& a, const v3d& b) {
  dbl x = a.y * b.z - a.z * b.y;
  dbl y = a.z * b.x - a.x * b.z;
  dbl z = a.x * b.y - a.y * b.x;
  return {x, y, y};
}

v3d cross_at3d(const v3d& o, const v3d& a, const v3d& b) {
  return cross3d(a - o, b - o);
}

dbl dot3d(const v3d& a, const v3d& b) {
  return a.x * b.x + a.y * b.y + a.z * b.z;
}

v3d normal_of3d(const v3d& a, const v3d& b) {
  return normalise3d(cross3d(a, b));
}

v3d reflect3d(const v3d& r, const v3d& n) {
  return dot3d(r, n) * 2.0 - r;
}

/* END SOLUTION */
#include <cstdio>

void print_v2d(const v2d& v) {
  printf("(%.3f, %.3f)", v.x, v.y);
}

void print_line2d(const line2d& v) {
  printf("{");
  print_v2d(l.a);
  printf(", ");
  print_v2d(l.b);
  printf("}");
}

void assert_equal(const dbl& expected, const dbl& actual) {
  if (expected != actual) {
    printf("Expected: %.3f\nGot: %.3f\n", expected, actual);
    exit(1);
  }
}

void assert_equal(const v2d& expected, const v2d& actual) {
  if (expected != actual) {
    printf("Expected: ");
    print_v2d(expected);
    printf("\nGot: ");
    print_v2d(actual);
    printf("\n");
    exit(1);
  }
}

void assert_equal(const line2d& expected, const line2d& actual) {
  if (expected != actual) {
    printf("Expected: ");
    print_line2d(expected);
    printf("\nGot: ");
    print_line2d(actual);
    printf("\n");
    exit(1);
  }
}

void test() {
  // dbl mag2d(const v2d& v);
  assert_equal(sqrt(2), mag2d({1, 1}));
  assert_equal(2, mag2d({0, 2}));
  assert_equal(0, mag2d({0, 0}));
  
  // dbl dist2d(const v2d& a, const v2d& b);
  assert_equal(sqrt(2), dist2d({0, 0}, {1, 1}));
  assert_equal(sqrt(2), dist2d({2, 2}, {1, 1}));

  // v2d normalise2d(const v2d& v);
  assert_equal({1 / sqrt(2), 1 / sqrt(2)}, normalise2d({100, 100}));
  assert_equal({1, 0}, normalise2d({100, 0}));
  
  // v2d cross2d(const v2d& a, const v2d& b);
  assert_equal(-25.548, cross2d({1.32, 5.32}, {5.1, 1.2}));
  assert_equal(25.548, cross2d({5.1, 1.2}, {1.32, 5.32}));
  
  // v2d cross_at2d(const v2d& o, const v2d& a, const v2d& b);
  assert_equal(25.548, cross_at2d({0, 0}, {5.1, 1.2}, {1.32, 5.32}));
  assert_equal(8.824 * 2.0, cross_at2d({1, 1}, {5.1, 1.2}, {1.32, 5.32}));
  
  // bool is_strictly_left_of(const v2d& p, const line2d& l);
  // bool is_strictly_right_of(const v2d& p, const line2d& l);
  assert(is_strictly_left_of({3, 3}, {{1, 1}, {4, 3}}));
  assert(!is_strictly_right_of({3, 3}, {{1, 1}, {4, 3}}));
  assert(!is_strictly_left_of({3, 2}, {{1, 1}, {4, 3}}));
  assert(!is_strictly_right_of({3, 2}, {{1, 1}, {4, 3}}));
  
  // v2d dot2d(const v2d& a, const v2d& b);
  assert_equal(13.116, dot2d({5.1, 1.2}, {1.32, 5.32}));

  // v2d normal_of2d(const v2d& v);
  assert_equal({}, normal_of2d({1.32, 5.32}));

  // v2d reflect2d(const v2d& r, const v2d& n);
  assert_equal({

  // dbl prop_along_line2d(const v2d& p, const line2d& l);

  // bool point_on_line_inf2d(const v2d& p, const line2d& l);

  // bool point_on_line_segment2d(const v2d& p, const line2d& l, bool include);

  // bool inter2d(const line2d& a, const line2d& b, bool include,
  //              dbl& aprop, dbl& bprop, line2d& inter_line, bool& infinite);

  // bool colinear2d(const v2d& a, const v2d& b, const v2d& c);

  // bool convex2dcomp(const v2d& a, const v2d& b);

  // poly2d convex2d(const vector<v2d>& pts);

  // bool point_in_poly2d(const v2d& v, const poly2d& poly);

  // pair<int, int> closest_two_points2d(const vector<v2d>& pts);

  // dbl area2d(const poly2d& poly);

  // v2d centroid2d(const poly2d& poly);

  // v2d project_point_onto_line2d(const v2d& p, const line2d& l);

  // dbl dist_to_inf_line2d(const v2d& p, const line2d& l);

  // dbl dist_to_line_segment_2d(const v2d& p, const line2d& l);

}
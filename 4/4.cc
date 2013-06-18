#include <inttypes.h>
#include <cstdio>
#include <stdint.h>
#include <algorithm>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <vector>
#include <queue>
using namespace std;

typedef int64_t i64;

#define SET(a, x) memset(a, x, sizeof a)
#define REP(i, n) for (int i = 0; i < (n); i++)
#define REP1(i, n) for (int i = 1; i <= (n); i++)
#define FOR(i, a, b) for (int i = (a); i < (b); i++)
#define ROF(i, a, b) for (int i = (b); --i >= (a); )

#define iter(v) __typeof((v).begin())
#define foreach(it, v) for (iter(v) it = (v).begin(); it != (v).end(); it++)

#define PB push_back
#define MP make_pair
#define fi first
#define se second

typedef vector<int> VI;
typedef pair<int, int> PII;
typedef vector<PII> VPII;

int RI()
{
  int x;
  scanf("%d", &x);
  return x;
}

const int N = 10, ONE = 10, NDST = 20, M = 100;

bool null[N][N];
bool visited[N * N], cross[N*N][N*N];
int dstno[N][N], path[M], optpath[NDST][M], pred[N*N];
struct Point
{ int x, y; } src, dst[N*N];

int m, n, ndst;

void parse()
{
  Point p;
  int t;
  scanf("Dimensions %*d");
  scanf("%d%d", &m, &n);
  scanf("\nFeed %*d\n");
  scanf("%d%d", &src.x, &src.y);

  scanf("\nAntenna %d", &ndst);
  SET(dstno, -1);
  REP(i, ndst) {
    scanf("%d%d", &dst[i].x, &dst[i].y);
    dstno[dst[i].y][dst[i].x] = i;
  }

  scanf("\nNull %d", &t);
  SET(null, 0);
  REP(i, t) {
    scanf("%d%d", &p.x, &p.y);
    null[p.y][p.x] = true;
  }
}

i64 dfs(int k, int x, int y, int one, int sq2)
{
  static int dx[] = {-1,1,1,-1, 0,1,0,-1};
  static int dy[] = {-1,-1,1,1, -1,0,1,0};

  if (visited[y * m + x]) return 0;
  visited[y * m + x] = true;
  path[k] = y * m + x;

  if (one + sq2 == 0) {
    visited[y * m + x] = false;
    if (dstno[y][x] == -1)
      return 0;
    if (optpath[dstno[y][x]][0] == -1)
      copy(path, path + k + 1, optpath[dstno[y][x]]);
    return 1LL << dstno[y][x];
  }

  if (dstno[y][x] != -1) {
    visited[y * m + x] = false;
    return 0;
  }

  i64 res = 0;
  // sq2
  if (sq2 > 0)
    REP(d, 4) {
      int xx = x+dx[d], yy = y+dy[d];
      if (unsigned(xx) < m && unsigned(yy) < n && ! null[yy][xx])
        res |= dfs(k+1, xx, yy, one, sq2-1);
    }
  // one
  if (one > 0)
    FOR(d, 4, 8) {
      int xx = x+dx[d], yy = y+dy[d];
      if (unsigned(xx) < m && unsigned(yy) < n && ! null[yy][xx])
        res |= dfs(k+1, xx, yy, one-1, sq2);
    }

  visited[y * m + x] = false;
  return res;
}

bool bfs(int srcx, int srcy, int dstx, int dsty, VPII &segments)
{
  static int dx[] = {-1,1,1,-1, 0,1,0,-1};
  static int dy[] = {-1,-1,1,1, -1,0,1,0};

  SET(visited, 0);
  for (iter(segments) i = segments.begin(); i != segments.end(); ++i) {
    int x = i->fi % m, y = i->fi / m;
    int xx = i->se % m, yy = i->se / m;
    visited[y * m + x] = visited[yy * m + xx] = true;
  }
  visited[dsty * m + dstx] = false;

  queue<int> q;
  q.push(srcy * m + srcx);
  while (! q.empty()) {
    int x = q.front() % m, y = q.front() / m;
    q.pop();
    REP(d, 8) {
      int xx = x+dx[d], yy = y+dy[d];
      if (unsigned(xx) < m && unsigned(yy) < n && ! null[yy][xx] && ! visited[yy * m + xx] && ! cross[y * m + x][yy * m + xx]) {
        pred[yy * m + xx] = y * m + x;
        visited[yy * m + xx] = true;
        q.push(yy * m + xx);
      }
    }
  }

  if (visited[dsty * m + dstx]) {
    while (dstx != srcx || dsty != srcy) {
      int u = dsty * m + dstx;
      dstx = pred[u] % m;
      dsty = pred[u] / m;
      int v = dsty * m + dstx;
      if (u < v)
        segments.PB(MP(u, v));
      else
        segments.PB(MP(v, u));
    }
    return true;
  }

  return false;
}

void solve(int probid)
{
  i64 full = (1LL << ndst) - 1;
  for (int dd = 0; ; dd++)
    for (int sq2 = 0; sq2 * 3 <= dd; sq2++) {
      int one = dd - sq2 * 3;
      if (one % 2) continue;
      one /= 2;
      //printf("---%d %d\n", one, sq2);
      SET(visited, 0);

      REP(cy, n)
        REP(cx, m) {
          if (null[cy][cx]) continue;

          SET(optpath, -1);
          if (dfs(0, cx, cy, one, sq2) == full) {
            VPII segments;
            SET(cross, 0);

            REP(i, ndst)
              FOR(j, 1, one + sq2 + 1) {
                int u = optpath[i][j-1], v = optpath[i][j];
                if (u > v) swap(u, v);
                segments.PB(MP(u, v));

                int x = u % m, y = u / m;
                int xx = v % m, yy = v / m;
                // cross
                if (x+1 == xx && y+1 == yy || x-1 == xx && y+1 == yy) {
                  cross[y * m + xx][yy * m + x] = true;
                  cross[yy * m + x][y * m + xx] = true;
                }
              }

            if (bfs(cx, cy, src.x, src.y, segments)) {
              sort(segments.begin(), segments.end());
              segments.erase(unique(segments.begin(), segments.end()), segments.end());

              SET(cross, 0);
              for (iter(segments) i = segments.begin(); i != segments.end(); ++i) {
                int u = i->fi, v = i->se;
                int x = u % m, y = u / m;
                int xx = v % m, yy = v / m;
                if (x+1 == xx && y+1 == yy || x-1 == xx && y+1 == yy) {
                  cross[y * m + x][yy * m + xx] = true;
                  cross[yy * m + xx][y * m + x] = true;
                  if (cross[y * m + xx][yy * m + x] || cross[yy * m + x][y * m + xx])
                    goto L1;
                }
              }

              printf("Solution %d\n", probid);
              printf("Line %zd\n", segments.size());
              for (iter(segments) i = segments.begin(); i != segments.end(); ++i) {
                int x = i->fi % m, y = i->fi / m;
                int xx = i->se % m, yy = i->se / m;
                printf("Segment %d %d %d %d\n", x, y, xx, yy);
              }
              return;
            }
          }
L1:;
        }
    }
}

int main()
{
  char line[1000];
  int probid;
  while (gets(line)) {
    if (sscanf(line, "Problem %d", &probid) == 1) {
      parse();
      solve(probid);
      return 0;
    }
  }
}

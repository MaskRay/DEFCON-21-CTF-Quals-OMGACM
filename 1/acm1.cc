#include <algorithm>
#include <cstdio>
#include <functional>
#include <queue>
using namespace std;

const int factorial[] = {1,1,2,6,24,120,720,5040,40320};

#define FOR(i, a, b) for (int i = (a); i < (b); i++)
#define REP(i, n) FOR(i, 0, n)

enum Direction {U, R, D, L};

struct Node
{
  int a[9], g;
  int zero() const {
    return int(find(a, a + 9, 0) - a);
  }
  Node up() {
    int p = zero();
    if (p < 3) throw 0;
    Node res = *this;
    swap(res.a[p], res.a[p-3]);
    res.g = g + 1;
    return res;
  }
  Node right() {
    int p = zero();
    if (p % 3 == 2) throw 0;
    Node res = *this;
    swap(res.a[p], res.a[p+1]);
    res.g = g + 1;
    return res;
  }
  Node down() {
    int p = zero();
    if (p >= 6) throw 0;
    Node res = *this;
    swap(res.a[p], res.a[p+3]);
    res.g = g + 1;
    return res;
  }
  Node left() {
    int p = zero();
    if (p % 3 == 0) throw 0;
    Node res = *this;
    swap(res.a[p], res.a[p-1]);
    res.g = g + 1;
    return res;
  }
  bool operator==(const Node &rhs) const {
    return equal(a, a + 9, rhs.a);
  }
};

int closed[362880];

const Node dst = {{1,2,3,4,5,6,7,8,0}};
int lehmorCode(const Node &x)
{
  int h = 0;
  REP(i, 9)
    REP(j, i)
    if (x.a[j] > x.a[i])
      h += factorial[8-j];
  return h;
}

int bfs(const Node &src)
{
  queue<Node> q;
  fill_n(closed, 362880, -1);
  int dst_code = lehmorCode(dst);

  Node orig = dst;
  q.push(orig);
  closed[dst_code] = -2;

  while (! q.empty()) {
    Node curr = q.front();
    q.pop();
    if (curr == src) return curr.g;
    int dir = 0;
    function<Node(Node &)> fs[] = {&Node::up, &Node::right, &Node::down, &Node::left};
    for (auto f : fs) {
      dir++;
      try {
        Node succ = f(curr);
        int h = lehmorCode(succ);
        if (closed[h] == -1) {
          q.push(succ);
          closed[h] = dir - 1;
        }
      } catch (int) {
      }
    }
  }
  return -1;
}

void print(Node x)
{
  for(;;) {
    switch (closed[lehmorCode(x)]) {
    case U:
      x = x.down();
      puts("up");
      break;
    case R:
      x = x.left();
      puts("right");
      break;
    case D:
      x = x.up();
      puts("down");
      break;
    case L:
      x = x.right();
      puts("left");
      break;
    case -2:
      return;
    }
  }
}

int main()
{
  Node src;
  REP(i, 9)
    scanf("%1d", &src.a[i]);
  int len = bfs(src);
  printf("%d\n", len);
  if (len > 0)
    print(src);
}

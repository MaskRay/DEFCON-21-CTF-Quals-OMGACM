#include <algorithm>
#include <netdb.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <queue>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <utility>
using namespace std;

#define FOR(i, a, b) for (int i = (a); i < (b); i++)
#define REP(i, n) FOR(i, 0, n)
#define SET(i, v) memset(i, v, sizeof i)

const int N = 11, M = 7;
char a[N][M+1], first[N][M], buf[4096];

bool isObstacle(char c)
{
  return c != ' ' && c != 'u' && c != '=';
}

bool cmp(int y, int yy)
{
  return abs(y-M/2) < abs(yy-M/2);
}

char work()
{
  int t = 0, x, y;
  REP(i, N) {
    REP(j, M + 1) {
      a[i][j] = buf[t++];
      if (a[i][j] == 'u')
        y = j;
    }
    a[i][M] = 0;
  }

  const char op[] = "lsr";
  SET(first, 0);
  queue<pair<int, int> > q;
  q.push(make_pair(9, y));
  while (! q.empty()) {
    x = q.front().first;
    y = q.front().second;
    q.pop();
    if (x == 1) break;
    int ys[] = {y-1, y, y+1};
    sort(ys, ys + 3, cmp);
    REP(i, 3) {
      int yy = ys[i];
      if (! first[x-1][yy] && ! isObstacle(a[x-1][yy])) {
        first[x-1][yy] = x == 9 ? op[yy-y+1] : first[x][y];
        q.push(make_pair(x-1, yy));
      }
    }
  }

  const int cols[M-2] = {3,2,4,1,5};
  REP(i, M-2)
    if (first[1][cols[i]])
      return first[1][cols[i]];
  return 's';
}

int main()
{
  int sock_fd;
  struct hostent *host;
  struct sockaddr_in serv_addr = {};
  if ((host = gethostbyname("grandprix.shallweplayaga.me")) == NULL)
    return 1;
  if ((sock_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    return 1;
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_port = htons(2038);
  serv_addr.sin_addr = *((struct in_addr *)host->h_addr);
  if (connect(sock_fd, (struct sockaddr *)&serv_addr, sizeof serv_addr) == -1)
    return 1;

  // Press return to start
  read(sock_fd, buf, sizeof buf);
  puts(buf);
  write(sock_fd, "\n", 1);

  int round = 0, len;
  char choice[2] = {0, '\n'};
  while (len = read(sock_fd, buf, sizeof buf - 1), buf[len] = '\0', len > 40) {
    puts(buf);
    choice[0] = work();
    printf("Round %d %c\n", ++round, choice[0]);
    write(sock_fd, choice, sizeof choice);
  }
}

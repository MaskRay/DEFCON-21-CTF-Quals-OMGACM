require 'socket'
require 'set'

$sock = TCPSocket.new 'diehard.shallweplayaga.me', 4001

def send d
  $sock.puts d
  msg = $sock.recv(1024)
  print msg
  msg
end

def recv
  msg = $sock.recv 1024
  print msg
  msg
end

def pour blue, red, ins
  r = b = c = 0
  while b != ins && r != ins
    c += 1
    if b == blue
      b = 0
    elsif r > 0
      bb = [r+b, blue].min
      r -= bb-b
      b = bb
    else
      r = red
    end
  end
  c
end

#res = send ''
#while res[/Exits: (.*)/]
  #exits = $1.split.select {|c| c != 's' }
  #res = send(exits.member?('n') ? 'n' : exits[0])
#end
send 'nnnnnnenwn'.chars.map {|c| "#{c}\n" }.join.chomp

loop do
  exchange = false

  res = send "get red jug\nget blue jug\nlook red jug\nlook blue jug\nlook inscription"
  res += recv until res[/A red jug holds 0 of (\d+) gallons/]
  red = $1.to_i
  res += recv until res[/A blue jug holds 0 of (\d+) gallons/]
  blue = $1.to_i
  res += recv until res[/To get to the next stage put (\d+) gallons/]
  ins = $1.to_i

  sred, sblue = 'red', 'blue'
  res1 = pour blue, red, ins
  res2 = pour red, blue, ins
  if res1 > res2
    exchange = true
    red, blue = blue, red
    sred, sblue = sblue, sred
  end

  r = b = 0
  msg = ''
  while b != ins && r != ins
    if b == blue
      b = 0
      msg += "empty #{sblue} jug\n"
    elsif r > 0
      bb = [r+b, blue].min
      r -= bb-b
      b = bb
      msg += "pour #{sred} jug into #{sblue} jug\n"
    else
      r = red
      msg += "fill #{sred} jug\n"
    end
  end
  msg += "put #{b == ins ? sblue : sred} jug onto scale\n"
  msg += "drop #{b == ins ? sred : sblue} jug\n"
  msg += "n"

  res = send msg
  res += recv until res[/You (see|find)/]
  if res['You find yourself in a solid granite']
    send "look key\nget key"
    break
  end
end

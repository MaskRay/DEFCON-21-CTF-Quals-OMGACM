require 'socket'

s = TCPSocket.new 'pieceofeight.shallweplayaga.me', 8273

src = ''
rest = 0

while line = s.gets
  puts line
  if line =~ /|  \d/
    xs = line.scan(/\|  ./).map {|x| x[-1] }
    if xs.find {|x| x =~ /\d/ }
      src << xs.map {|x| x == ' ' ? '0' : x }.join
    end
  end
  if line =~ /Press/
    rest = 0
  end
  if src.size == 9 && line =~ /----/
    if rest > 0
      rest -= 1
    else
      IO.popen './acm1', 'r+' do |h|
        h.puts src
        h.close_write
        xs = h.read.lines.to_a
        s.puts xs[1..-1].join
        rest = xs[0].to_i
      end
    end
    src = ''
  end
end

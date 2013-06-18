require 'socket'
require 'expect'
require 'timeout'
require 'set'

sock = TCPSocket.new 'bigbadbob.shallweplayaga.me', 2232
#sock = TCPSocket.new 'localhost', 4001
#sock = IO.popen 'ruby ./fake.rb', 'r+'

send = ->d {
  puts d
  sock.puts d
  sock.flush
}

unless (line = sock.gets).match(/Here we go/)
end

while line = sock.gets
  if line =~ /^Problem \d+/
    msg = line
    until (line = sock.gets).match(/Null (\d+)/)
      msg += line
    end
    msg += line
    $1.to_i.times do
      msg += sock.gets
    end
    puts msg
    h = nil
    begin
      Timeout.timeout 4 do
        h = IO.popen './4', 'r+'
        h.puts msg
        h.close_write
        send[h.read]
      end
    rescue Timeout::Error
      Process.kill 9, h.pid
    end
  elsif line =~ /Congratuation/
    puts "---next mission---"
  elsif line =~ /The key/
    puts line
  else
    puts line
  end
end

sock.close

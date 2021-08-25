include Math

input = gets.split
n = input[0].to_i
print n.to_s + ":"

sq = sqrt(n).floor

(2..sq).each do |i|
  while n % i == 0 do
    print " " + i.to_s
    n /= i
  end
end
puts " " + n.to_s if n != 1
puts if n == 1

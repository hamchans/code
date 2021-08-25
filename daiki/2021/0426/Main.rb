input = gets.split(' ')
m = input[0].to_i
n = input[1].to_i

num = 1
check = n % 10

(0...(n/10)).each do |i|
  num = (num * m) % (10 ** 9 + 7)
end

opt = 1

(0...10).each do |i|
  opt = (opt * num) % (10 ** 9 + 7)
end

(0...check).each do |i|
  opt = (opt * m) % (10 ** 9 + 7)
end

p opt

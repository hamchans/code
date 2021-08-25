s1 = gets.to_s
s2 = gets.to_s

count = 0

DP = Array.new(s2.size+1).map{Array.new(s1.size+1, 0)}

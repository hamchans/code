input = gets.split
n = input[0].to_i
m = input[1].to_i
input = gets.split
coin = input.map do |i|
    i.to_i
end
coin.sort!
L = Array.new(n+1, 10000)
L[0] = 0

coin.each do |v, u|
    
end


L.each_with_index do |a, i|
    if i == 0
        next
    end
    L[i] = L[i-1] + 1 < L[i] ? L[i-1] + 1 : L[i]
end

puts L[n]

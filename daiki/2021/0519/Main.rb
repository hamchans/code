input = gets.split
N = input[0].to_i
W = input[1].to_i
DP = Array.new(N+1).map{Array.new(W+1, 0)}

(1..N).each do |i|
    input = gets.split
    v = input[0].to_i
    w = input[1].to_i
    (1..W).each do |j|
        DP[i][j] = DP[i-1][j] > DP[i][j-1] ? DP[i-1][j] : DP[i][j-1]
        if j - w >= 0
            DP[i][j] = DP[i][j] > DP[i-1][j-w] + v ? DP[i][j] : DP[i-1][j-w] + v
        end
    end
end

p DP[N][W]

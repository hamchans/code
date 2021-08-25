n = gets.to_i
array = Array.new(1).map{Array.new(0, nil)}
DP = []

n.times do |i|
    DP[i] = 1.0 / 0
end

n.times do |i|
    input = gets.to_i
    array.push(input)
    array.length.times do |j|
        if array[j][-1] == input

        else
            if array[j][-1] > input && array[j][-2] > input
                array.push(array[j][-1])
                array[-1][-1] = input
            end
        end
    end
end

input = gets.split(' ')
N = input[0].to_i
K = input[1].to_i

input = gets.split(' ')

if N < K
  p 0
else
  input.each do |n|
    if n <= K
      pass
    else
      
    puts n
  end
end

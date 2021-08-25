def extend_gcd(a, b, x, y)
  if b == 0 then
    $x = 1
    $y = 0
    return a
  else
    d = extend_gcd(b, a % b, $y, $x)
    puts($x, $y)
    puts("")
    $y -= (a / b) * $x
    return d
  end
end

input = gets.split(' ')
a = input[0].to_i
b = input[1].to_i

$x = 0
$y = 0
extend_gcd(a, b, $x, $y)

puts($x, $y)

input = gets.split(' ')
px = input[0].to_f
py = input[1].to_f
input = gets.split(' ')
cx = input[0].to_f
cy = input[1].to_f
r = input[2].to_f

qy1 = ((py - cy) + ((py - cy) ** 2 - ((py - cy) ** 2 + (px - cx) ** 2) * (r ** 4 - r ** 2 * (px - cx) ** 2)) ** 0.5) / ((py - cy) ** 2 + (px - cx) ** 2) + cy
qx1 = (r ** 2 + cx - qy1 * (py - cy) + cy * (py - cy)) / (px - cx)

qy2 = ((py - cy) - ((py - cy) ** 2 - ((py - cy) ** 2 + (px - cx) ** 2) * (r ** 4 - r ** 2 * (px - cx) ** 2)) ** 0.5) / ((py - cy) ** 2 + (px - cx) ** 2) + cy
qx2 = (r ** 2 + cx - qy2 * (py - cy) + cy * (py - cy)) / (px - cx)

if qx1 > qx2
  p qx2.to_s + qy2.to_s
  p qx1.to_s + qy1.to_s
else
  p qx1.to_s + qy1.to_s
  p qx2.to_s + qy2.to_s
end


i = 0

while i < 19 do
   j = 1
   while j <= 19 do
      io.write(string.char(string.byte("A", 1) + i) .. tostring(j) .. ", ")
      j = j + 1;
   end	       
   print();
   i = i + 1;
end

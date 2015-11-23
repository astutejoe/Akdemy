int i;

begin
   writeln, "Escreva um número: ";
   readln, i;
   while i > 1
   begin
      a = a * (i - 1);
      i = i - 1;
   end
   writeln, a;
end
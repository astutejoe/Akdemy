int t1 = 1, t2 = 1, tmp, n, i = 2;
string nome;

begin
   writeln, "Escreva um numero de 5 a 12: ";
   readln, n;
   write, "[", t1, ",", t2, ",";
   while i < n
   begin
       tmp = t2;
       t2 = t1 + t2;
       write, t2;
       if i < (n-1) begin
           write, ",";
       end
       i = i + 1;
       t1 = tmp;
   end
   writeln, "]";
end

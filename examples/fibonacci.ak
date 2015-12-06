int t1 = 1, t2 = 1, tmp, n, i = 2;
string nome;

begin
	writeln "Escreva o numero de termos (maior que 2): ";
	readln n;

	if n > 2 begin
		write "[", t1, ",", t2;

		if n > 2 write ",";

		while i < n
		begin
			tmp = t2;
			t2 = t1 + t2;
			write t2;
			if i < (n-1) begin
				write ",";
			end
			i = i + 1;
			t1 = tmp;
		end

		writeln "]";
	end else writeln "sabe ler?";
end